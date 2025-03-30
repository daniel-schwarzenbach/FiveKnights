#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <sdl/SDLBackend.h++>

// wrapper for SDL
namespace Senpai {
// static variables for all to access
static SDL_Window* window;
static SDL_Renderer* renderer;
static Vec2<int> windowSize;
static SDL_AudioSpec audioSpec;

namespace SDL {

// needs to be called first
// init whole sdl package
bool init(String title, u32 width, u32 height) {
   // initialize SDL
   if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
      debug_log("sdl initialized!");
   } else {
      cerr << SDL_GetError() << endl;
      return false;
   }

   // initialize SDL_ttf
   if (TTF_Init()) {
      debug_log("sdl_ttf initialized!");
   } else {
      cerr << SDL_GetError() << endl;
      return false;
   }

   // initialize SDL_mixer
   if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK | MIX_INIT_MOD)) {
      debug_log("sdl_mixer initialized!");
   } else {
      cerr << SDL_GetError() << endl;
      return false;
   }
   /* Open Audio Device */
   audioSpec.freq = MIX_DEFAULT_FREQUENCY;
   audioSpec.format = MIX_DEFAULT_FORMAT;
   audioSpec.channels = MIX_DEFAULT_CHANNELS;
   if (Mix_OpenAudio(0, &audioSpec)) {
      debug_log("Audio Device Opened");
   } else {
      cerr << SDL_GetError() << endl;
   }
   // set volume for all chanals to max
   Mix_Volume(-1, MIX_MAX_VOLUME);

   // create a window
   SDL_WindowFlags widowFlags =
       SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
   window = SDL_CreateWindow(title.c_str(), width, height, widowFlags);
   if (window) {
      debug_log("window created!");
   } else {
      cerr << SDL_GetError() << endl;
      return false;
   }

   // create a renderer
   renderer = SDL_CreateRenderer(window, NULL);
   if (renderer) {
      debug_log("renderer created!");
   } else {
      cerr << SDL_GetError() << endl;
      return false;
   }
   return true;
}

// quits all sdl packages
void quit() {
   TTF_Quit();
   Mix_Quit();
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
}

SDL_FRect to_sdl_dst(const Senpai::Rectangle<f32>& rect) {
   auto pos = rect.left_top();
   return SDL_FRect{windowSize.x / 2.0f + pos.x, windowSize.y / 2.0f - pos.y,
                    rect.size.x, rect.size.y};
}

SDL_FRect to_sdl_src(const Senpai::PixelArea area) {
   return SDL_FRect{area.position.x, area.position.y, area.size.x, area.size.y};
}

SDL_Rect to_sdl(const Senpai::Rectangle<int>& rect) {
   auto pos = rect.left_top();
   return SDL_Rect{pos.x, -pos.y, rect.size.x, rect.size.y};
}

SDL_Rect to_sdl_viewport(const Senpai::Rectangle<f32>& rect) {
   auto pos = rect.left_top();
   return SDL_Rect{int(round(windowSize.x * (0.5f + pos.x))),
                   int(round(windowSize.y * (0.5f - pos.y))),
                   int(round(rect.size.x * windowSize.x)),
                   int(round(rect.size.y * windowSize.y))};
}

void RenderFrame(Senpai::Frame<f32> const& frame, SDL_Texture* texture, u8 flip,
                 SDL_BlendMode bm, PixelArea src) {
   SDL_FRect srcRect = to_sdl_src(src);
   // define destination rect
   Senpai::Rectangle<f32> senpaiRect = {frame.position, frame.size};
   SDL_FRect dstRect = to_sdl_dst(senpaiRect);
   // get the rotation point
   SDL_FPoint point = {dstRect.w * (0.5f + frame.rotAnchor.x),
                       dstRect.h * (0.5f - frame.rotAnchor.y)};
   SDL_SetTextureBlendMode(texture, bm);
   // render texture
   SDL_RenderTextureRotated(Senpai::Renderer::get(),  // renderer
                            texture,                  // texture
                            &srcRect,                 // source rect
                            &dstRect,                 // destination rect
                            frame.rotation,           // rotation
                            &point,                   // rotation point
                            (SDL_FlipMode)flip        // flipflags
   );
}
}  // namespace SDL

namespace Renderer {
Ptr<SDL_Renderer> get() { return renderer; }
// clear the renderer
void clear() {
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   SDL_RenderClear(renderer);
}
// present the renderer
void present() { SDL_RenderPresent(renderer); }
}  // namespace Renderer

namespace Window {
// get the window
Ptr<SDL_Window> get() { return window; }
// get the new sizes of the window
void resize() {
   SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

   int rw, rh;
   SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh);

   float widthScale = (float)rw / (float)windowSize.x;
   float heightScale = (float)rh / (float)windowSize.y;

   SDL_SetRenderScale(renderer, widthScale, heightScale);
}

Vec2<int> get_size() { return windowSize; }

// get the aaudioSpect ratio of the window
f32 get_aspect_ratio() {
   auto [width, height] = get_size();
   return (f32)width / (f32)height;
}

}  // namespace Window

}  // namespace Senpai