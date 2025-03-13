#include <components/Components.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai {
   Color operator+(const Color& lhs, const Color& rhs) {
      return {
         .r = u8(lhs.r + rhs.r),
         .g = u8(lhs.g + rhs.g),
         .b = u8(lhs.b + rhs.b),
         .a = u8(lhs.a + rhs.a)
      };
   }

   Color operator-(const Color& lhs, const Color& rhs) {
      return {
         .r = u8(lhs.r - rhs.r),
         .g = u8(lhs.g - rhs.g),
         .b = u8(lhs.b - rhs.b),
         .a = u8(lhs.a - rhs.a)
      };
   }

   Color& operator+=(Color& lhs, const Color& rhs) {
      lhs.r += rhs.r;
      lhs.g += rhs.g;
      lhs.b += rhs.b;
      lhs.a += rhs.a;
      return lhs;
   }

   Color& operator-=(Color& lhs, const Color& rhs) {
      lhs.r -= rhs.r;
      lhs.g -= rhs.g;
      lhs.b -= rhs.b;
      lhs.a -= rhs.a;
      return lhs;
   }

}

namespace Senpai::Components {

   Camera::Camera(Vec2<f32> offset, f32 zoom, Rectangle<f32> viewport) : offset(offset), zoom(zoom), viewport(viewport) {
      if(viewport.size.x < 0 || viewport.size.y < 0){
         this->viewport.size = Window::get_size();
      }
      this->size = Vec2<f32>{1920,1080};
      this->size.y = 1920 / Window::get_aspect_ratio();
      this->id = component_type_id<Camera>();
   }

   Rectangle<f32> Camera::get_rect(Frame<f32> const& tr) const {
      return {
         .position = offset + tr.position,
         .size = size / zoom
      };
   }

   Rectangle<f32> Camera::get_canvas() const {
      // get the widow size
      Vec2<u32> wSize = Window::get_size();
      return {
         .position = viewport.position * wSize,
         .size = viewport.size * wSize
      };
   }

   Transform::Transform(
      Vec2<f32> translate, 
      Vec2<f32> scale, 
      f32 rotation,
      Vec2<f32> rotAnchor) : frame{translate, scale, rotAnchor, rotation}{
      this->id = component_type_id<Transform>();
   }

   Info::Info(String name, String tag) : name(name), tag(tag) {
      this->id = component_type_id<Info>();
   }

   Sprite::Sprite(
      Ptr<Assets::Texture> assetPtr,
      Set<Flip> flips, 
      Set<BlendMode> blendModes
   ) : texturePtr(assetPtr) {
      this->id = component_type_id<Sprite>();
      for(auto flip : flips){
         this->flip |= flip;
      }
      for(auto blendMode : blendModes){
         this->blendMode |= blendMode;
      }
   }

   void Sprite::flip_vertical(){
      this->flip ^= Flip::Vertical;
   }

   void Sprite::flip_horizontal(){
      this->flip ^= Flip::Horizontal;
   }

   Frame<f32> Sprite::get_frame(Frame<f32> const& tr) const {
      return {
         .position = tr.position,
         .size = Vec2<f32>{texturePtr->height, texturePtr->width} * tr.size,
      };
   }

   Text::Text(
      String text, 
      Color color, 
      Ptr<Assets::Font> fontPtr,
      Set<Flip> flips, 
      Set<BlendMode> blendModes
   ) : text(text), color(color), fontPtr(fontPtr) {
      this->id = component_type_id<Text>();
      for(auto flip : flips){
         this->flip |= flip;
      }
      for(auto blendMode : blendModes){
         this->blendMode |= blendMode;
      }
   }

   void Text::flip_vertical(){
      this->flip ^= Flip::Vertical;
   }
   void Text::flip_horizontal(){
      this->flip ^= Flip::Horizontal;
   }




   bool Animator::flip_frame(f32 δt) {
      Δt += δt;
      if (Δt >= flipTime) {
         Δt -= flipTime;
         return true;
      }
      return false;
   }
   

   Ptr<Assets::Texture> Animator::next_frame() {
      frameId++;
      if (frameId >= animationPtr->frames.size()) {
         frameId = 0;
      }
      return animationPtr->frames[frameId];
   }

   Animator::Animator(f32 flipTime, Ptr<Assets::Animation> animationPtr) : flipTime(flipTime), animationPtr(animationPtr) {
      this->id = component_type_id<Animator>();
   }

   TileMap::TileMap(Matrix<u8> tiles, Ptr<Assets::TileSet> tileSetPtr) : tileSetPtr(tileSetPtr), tiles(tiles) {
      this->id = component_type_id<TileMap>();
   }

   Tuple<uint, uint> TileMap::get_tile_at_point(Frame<f32> const& transform,Vec2<f32> point) const {
      // get the effective size of the tiles
      f32 sizeX = tileSetPtr->tileSize * transform.size.x;
      f32 sizeY = tileSetPtr->tileSize * transform.size.y;
      // get the total size of the tiles-matrix
      auto [X, Y] = tiles.shape();
      // calculate the tile drawn at this position
      int x = floor((point.x - transform.position.x) / sizeX);
      int y = floor((point.y - transform.position.x) / sizeY);
      // return the tile closest to the point
      return {min(max(0, x),X-1), min(max(0, y),Y-1)};
   }

   Tuple<Range, Range> TileMap::get_render_ranges(Frame<f32> const& transform, Rectangle<f32> const& camera) const {
      // get the left upper corner of the camera
      Vec2<f32> left_top = camera.left_top();
      // get the right lower corner of the camera
      Vec2<f32> right_bottom = camera.right_bottom();
      // get the tile at the left upper corner
      auto [x1,y1] = get_tile_at_point(transform, left_top);
      // get the tile at the right lower corner
      auto [x2,y2] = get_tile_at_point(transform, right_bottom);
      // return the range of tiles to render
      return {Range{x1,x2}, Range{y1,y2}};
   }

   Frame<f32> TileMap::get_tile(Frame<f32> const& transfom, uint x, uint y) const {
      // get the effective size of the tiles
      f32 sizeX = tileSetPtr->tileSize * transfom.size.x;
      f32 sizeY = tileSetPtr->tileSize * transfom.size.y;
      // calculate the position of the tile
      Vec2<f32> position = {
         .x = transfom.position.x + x * sizeX,
         .y = transfom.position.y + y * sizeY
      };
      // return the frame of the tile
      return {
         .position = position,
         .size = Vec2<f32>{sizeX, sizeY}
      };
   }

   Rigidbody::Rigidbody(Vec2<f32> velocity, Vec2<f32> acceleration, f32 mass) : velocity(velocity), acceleration(acceleration), mass(mass) {
      this->id = component_type_id<Rigidbody>();
   }

   RectangleCollider::RectangleCollider(Rectangle<f32> rect, bool isSolid) : rect(rect), isSolid(isSolid) {
      this->id = component_type_id<RectangleCollider>();
   }

   Rectangle<f32> RectangleCollider::get_rect(Frame<f32> const& transform) const {
      return {
         .position = transform.position + rect.position,
         .size = rect.size * transform.size
      };
   }
   
}
