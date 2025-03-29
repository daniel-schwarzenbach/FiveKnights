#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <components/Components.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai::Components {

Camera::Camera(Vec2<f32> offset, f32 zoom, Rectangle<f32> viewport)
    : offset(offset), zoom(zoom), viewport(viewport) {
   if (viewport.size.x < 0 || viewport.size.y < 0) {
      this->viewport.size = Window::get_size();
   }
   this->size = Vec2<f32>{1920, 1080};
   this->size.y = 1920.0 / Window::get_aspect_ratio();
   this->id = component_type_id<Camera>();
}

Rectangle<f32> Camera::get_rect(Frame<f32> const &tr) const {
   return {.position = offset + tr.position, .size = size / zoom};
}

Rectangle<f32> Camera::get_canvas() const {
   // get the widow size
   Vec2<int> wSize = Window::get_size();
   return {.position = viewport.position * wSize,
           .size = viewport.size * wSize};
}

Transform::Transform(Vec2<f32> translate, Vec2<f32> scale, f32 rotation,
                     Vec2<f32> rotAnchor)
    : frame{translate, scale, rotAnchor, rotation} {
   this->id = component_type_id<Transform>();
}

Info::Info(String name, String tag) : name(name), tag(tag) {
   this->id = component_type_id<Info>();
}

ScriptsHolder::ScriptsHolder(ScriptsHolder const &other) {
   for (auto &script : other.scripts) {
      this->scripts.push_back(script->clone());
   }
}

ScriptsHolder &ScriptsHolder::operator=(ScriptsHolder const &other) {
   for (auto &script : other.scripts) {
      this->scripts.push_back(script->clone());
   }
   return *this;
}

Sprite::Sprite(Ptr<Assets::Texture> assetPtr, Vec2<f32> offset, Set<Flip> flips)
    : texturePtr(assetPtr), offset(offset) {
   this->id = component_type_id<Sprite>();
   for (auto flip : flips) {
      this->flip |= flip;
   }
   if (assetPtr) {
      this->scrArea = {0, 0, texturePtr->width, texturePtr->height};
   }
}

Frame<f32> Sprite::get_frame(Frame<f32> const &tr) const {
   return {
       .position = tr.position + offset,
       .size = Vec2<f32>{this->scrArea.size.x, this->scrArea.size.x} * tr.size,
       .rotAnchor = tr.rotAnchor,
       .rotation = tr.rotation
   };
}

bool once = false;

void Sprite::render(Rectangle<f32> const &cameraRect,
                    Rectangle<f32> const &canvasRect) {
   // get the frame of the sprite
   auto frame = get_frame(entityPtr->get_transform());
   // get the rect of the sprite
   auto projection = frame.project(cameraRect, canvasRect);
   // render the projection
   SDL::RenderFrame(projection, (SDL_Texture *)texturePtr->get_sdl_texture(),
                    flip, SDL_BLENDMODE_BLEND, this->scrArea);
}

// TextUI::TextUI(
//     String text,
//     Color color,
//     Ptr<Assets::Font> fontPtr,
//     Set<Flip> flips,
//     Set<BlendMode> blendModes) : text(text), color(color), fontPtr(fontPtr)
// {
//    this->id = component_type_id<TextUI>();
//    for (auto flip : flips)
//    {
//       this->flip |= flip;
//    }
//    for (auto blendMode : blendModes)
//    {
//       this->blendMode |= blendMode;
//    }
// }

// rememver to free the texture
Tuple<SDL_Texture *, f32, f32>
get_texture_from_text(Components::ButtonUI const &button) {
   auto &font = *button.fontPtr;
   // create font
   bool fontHasToBeFreed = false;
   TTF_Font *sdlFont = (TTF_Font *)font.sdlFont.get();
   // create surface
   SDL_Surface *surface =
       TTF_RenderText_Blended(sdlFont, button.text.c_str(), button.text.size(),
                              {button.textColor.r, button.textColor.g,
                               button.textColor.b, button.textColor.a});
   if (fontHasToBeFreed) {
      TTF_CloseFont(sdlFont);
   }
   if (!surface) {
      debug_log("Couldn't Create sdlsurface");
      return {nullptr, 0, 0};
   }
   // create texture from surface
   SDL_Texture *texture =
       SDL_CreateTextureFromSurface((SDL_Renderer *)Renderer::get(), surface);
   if (!texture) {
      debug_log("Couldn't Create sdltexture");
      return {nullptr, 0, 0};
   }
   f32 width, height;
   SDL_GetTextureSize(texture, &width, &height);
   // destroy surface
   SDL_DestroySurface(surface);
   return {texture, width, height};
}

ButtonUI::ButtonUI(Ptr<Assets::Font> fontPtr, String text, Color textColor,
                   Color buttonColor, Color buttonHoverColor,
                   Vec2<f32> buttonScale)
    : text(text), textColor(textColor), buttonColor(buttonColor),
      buttonHoverColor(buttonHoverColor), buttonScale(buttonScale),
      fontPtr(fontPtr) {
   this->hoverstate = false;
   this->id = component_type_id<ButtonUI>();
}

Rectangle<f32> ButtonUI::get_rect() const {
   Vec2<f32> rs = get_render_scale();
   auto tr = entityPtr->get_transform();
   return Rectangle<f32>{tr.position * rs, buttonScale * tr.size * rs.x};
}

bool ButtonUI::is_on_button(Vec2<f32> point) {
   auto rect = get_rect();
   return rect.contains(point);
}

void ButtonUI::render() {
   // first render the button
   auto rect = get_rect();
   SDL_FRect sdlRect = SDL::to_sdl_dst(rect);
   SDL_SetRenderDrawBlendMode(Renderer::get(), SDL_BLENDMODE_BLEND);
   if (hoverstate) {
      SDL_SetRenderDrawColor(Renderer::get(), buttonHoverColor.r,
                             buttonHoverColor.g, buttonHoverColor.b,
                             buttonHoverColor.a);
   } else {
      SDL_SetRenderDrawColor(Renderer::get(), buttonColor.r, buttonColor.g,
                             buttonColor.b, buttonColor.a);
   }
   SDL_SetRenderDrawBlendMode(Renderer::get(), SDL_BLENDMODE_BLEND);
   if (!SDL_RenderFillRect(Renderer::get(), &sdlRect)) {
      cout << SDL_GetError() << endl;
   }
   // then render the text
   if (!fontPtr || text.empty()) {
      return;
   }
   // create surface
   auto [texture, width, height] = get_texture_from_text(*this);
   auto rs = get_render_scale();
   auto textFrame =
       Frame<f32>{rect.position, Vec2<f32>{width * rs.y, height * rs.y}};
   auto src = PixelArea{0, 0, width, height};
   SDL::RenderFrame(textFrame, texture, this->flip, SDL_BLENDMODE_BLEND, src);
   SDL_SetRenderDrawColor(Renderer::get(), 0, 0, 0, 255);
   SDL_DestroyTexture(texture);
}

bool Animator::flip_frame(f32 dt) {
   lastFlipTime += dt;
   if (lastFlipTime >= flipTime) {
      lastFlipTime -= flipTime;
      return true;
   }
   return false;
}

Animator::Animator(f32 fps, Vector<Ptr<Assets::Animation>> animations,
                   Vector<u32> nextAnimations) : flipTime(1.0/fps), animations(animations) {
   this->id = component_type_id<Animator>();
   nextAnimId.reserve(animations.size());
   for (uint i = 0; i < animations.size(); i++) {
      if (i < nextAnimations.size())
         nextAnimId.push_back(nextAnimations[i]);
      else
         nextAnimId.push_back(i);
   }
}

bool Animator::switch_animation(u32 id) {
   if (id >= animations.size()) {
      debug_log("Animation id out of bounds");
      return false;
   }
   animId = id;
   frameId = 0;
   has_switched = true;
   return true;
}

TileMap::TileMap(Matrix<u16> tiles, Ptr<Assets::TileSet> tileSetPtr)
    : tileSetPtr(tileSetPtr), tiles(tiles) {
   this->id = component_type_id<TileMap>();
}

Tuple<uint, uint> TileMap::get_tile_at_point(Frame<f32> const &transform,
                                             Vec2<f32> point) const {
   // get the effective size of the tiles
   f32 sizeX = tileSetPtr->sizeX * transform.size.x;
   f32 sizeY = tileSetPtr->sizeY * transform.size.y;
   // get the total size of the tiles-matrix
   auto [X, Y] = tiles.shape();
   // calculate the tile drawn at this position
   int x = floor((point.x - transform.position.x) / (sizeX*transform.size.x));
   int y = floor((point.y - transform.position.x) / sizeY*transform.size.y);
   // return the tile closest to the point
   return {min(max(0, x), X - 1), min(max(0, y), Y - 1)};
}

Tuple<Range, Range>
TileMap::get_render_ranges(Frame<f32> const &transform,
                           Rectangle<f32> const &camera) const {
   // get the left upper corner of the camera
   Vec2<f32> left_bottom = camera.left_bottom();
   // get the right lower corner of the camera
   Vec2<f32> right_top = camera.right_top();
   // get the tile at the left upper corner
   auto [x1, y1] = get_tile_at_point(transform, left_bottom);
   // get the tile at the right lower corner
   auto [X, Y] = tiles.shape();
   auto [x2, y2] = get_tile_at_point(transform, right_top);
   if(x2 < X-1) x2++;
   if(y2 < Y-1) y2++;
   if(x1 > 0) x1--;
   if(y1 > 0) y1--;
   if(y1 > 0) y1--;
   // return the range of tiles to render
   return {Range{int(x1), int(x2)}, Range{int(y1), int(y2)}};
}

Frame<f32> TileMap::get_tile(Frame<f32> const &transfom, uint x, uint y) const {
   // get the effective size of the tiles
   f32 sizeX = tileSetPtr->sizeX * transfom.size.x;
   f32 sizeY = tileSetPtr->sizeY * transfom.size.y;
   // calculate the position of the tile
   Vec2<f32> position = {.x = transfom.position.x + x * sizeX,
                         .y = transfom.position.y + y * sizeY};
   // return the frame of the tile
   return {.position = position, .size = Vec2<f32>{sizeX, sizeY}};
}

void TileMap::render(Rectangle<f32> const &camera,
                     Rectangle<f32> const &canvas) {
   // get the effective size of the tiles
   auto [rangeX, rangeY] =
       get_render_ranges(entityPtr->get_transform(), camera);
   for (auto y : rangeY) {
      for (auto x : rangeX) {
         // get the frame of the tile
         Frame<f32> tileFrame = get_tile(entityPtr->get_transform(), x, y);
         // get the tile id
         u16 tileId = tiles[x, y];
         // get source rect
         PixelArea src = tileSetPtr->operator[](tileId);
         // get the projection of the tile
         Frame<f32> projection = tileFrame.project(camera, canvas);
         // render the projection
         SDL::RenderFrame(projection,
                          (SDL_Texture *)tileSetPtr->get_sdl_texture(),
                          this->flip, SDL_BLENDMODE_BLEND, src);
      }
   }
}

Rigidbody::Rigidbody(Vec2<f32> velocity, Vec2<f32> acceleration, f32 mass)
    : velocity(velocity), acceleration(acceleration), mass(mass) {
   this->id = component_type_id<Rigidbody>();
}

RectangleCollider::RectangleCollider(Rectangle<f32> rect, bool isSolid)
    : rect(rect), isSolid(isSolid) {
   this->id = component_type_id<RectangleCollider>();
}

Rectangle<f32> RectangleCollider::get_rect(Frame<f32> const &transform) const {
   return {.position = transform.position + rect.position,
           .size = rect.size * transform.size};
}

ScriptsHolder::ScriptsHolder() {
   this->id = component_type_id<ScriptsHolder>();
}

Light::Light(Ptr<Assets::Texture> assetPtr, Set<Flip> flips)
    : texturePtr(assetPtr) {
   this->id = component_type_id<Light>();
   for (auto flip : flips) {
      this->flip |= flip;
   }
   if (assetPtr) {
      this->scrArea = {0, 0, texturePtr->width, texturePtr->height};
   }
}

Frame<f32> Light::get_frame(Frame<f32> const &tr) const {
   return {
      .position = tr.position + this->offset,
      .size = Vec2<f32>{this->scrArea.size.x, this->scrArea.size.x} * tr.size,
       .rotAnchor = tr.rotAnchor,
       .rotation = tr.rotation
       
   };
}

} // namespace Senpai::Components
