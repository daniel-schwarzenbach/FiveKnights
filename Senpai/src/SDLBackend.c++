#include <sdl/SDLBackend.h++>

// wrapper for SDL
namespace Senpai{
   // static variables for all to access
   static SDL_Window* window;
   static SDL_Renderer* renderer;


   namespace SDL {

      // needs to be called first
      // init whole sdl package
      bool init(String title, u32 width, u32 height){
         // initialize SDL
         if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO)){
            debug_log("sdl initialized!");
         } else {
            return false;
         }

         // create a window
         SDL_WindowFlags widowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
         window = SDL_CreateWindow(title.c_str(), width, height, widowFlags);
         if(window){
            debug_log("window created!");
         } else {
            return false;
         }

         // create a renderer
         renderer = SDL_CreateRenderer(window, NULL);
         if(renderer){
            debug_log("renderer created!");
         } else {
            return false;
         }
         return true;
      }

      // quits all sdl packages
      void quit(){
         SDL_DestroyRenderer(renderer);
         SDL_DestroyWindow(window);
         SDL_Quit();
      }
   }

   
   namespace Renderer {
      Ptr<SDL_Renderer> get(){
         return renderer;
      }
      // clear the renderer
      void clear(){
         SDL_RenderClear(renderer);
      }
      // present the renderer
      void present(){
         SDL_RenderPresent(renderer);
      }
   }

   namespace Window {
      // get the window
      Ptr<SDL_Window> get(){
         return window;
      }
      // get the size of the window
      Vec2<u32> get_size(){
         int width, height;
         SDL_GetWindowSize(window, &width, &height);
         return {u32(width), u32(height)};
      }
      // get the aspect ratio of the window
      f32 get_aspect_ratio(){
         auto [width, height] = get_size();
         return (f32)width / (f32)height;
   }
   
   }


   SDL_FRect to_sdl(const Senpai::Rectangle<f32>& rect) {
      auto pos = rect.left_top();
      return SDL_FRect{
         pos.x,
         -pos.y,
         rect.size.x,
         rect.size.y
      };
   }

   SDL_Rect to_sdl(const Senpai::Rectangle<int>& rect) {
      auto pos = rect.left_top();
      return SDL_Rect{
         pos.x,
         -pos.y,
         rect.size.x,
         rect.size.y
      };
   }

   SDL_Rect to_sdl_viewport(const Senpai::Rectangle<f32>& rect) {
      auto pos = rect.left_top();
      auto screen = Senpai::Window::get_size();
      return SDL_Rect{
         int(round(pos.x * screen.x)),
         int(round(screen.y - pos.y * screen.y)),
         int(round(rect.size.x * screen.x)),
         int(round(rect.size.y * screen.y))
      };
   }

   void SDL_RenderFrame(Senpai::Frame<f32> const& frame, SDL_Texture* texture , u8 flip, SDL_FRect*  src) {
      // define destination rect
      Senpai::Rectangle<f32> senpaiRect = {frame.position, frame.size};
      SDL_FRect dstRect = to_sdl(senpaiRect);
      // get the rotation point
      SDL_FPoint point = {
         frame.size.x/2 + frame.rotAnchor.x * frame.size.x, 
         frame.size.y/2 + frame.rotAnchor.y * frame.size.y
      };
      // render texture
      SDL_RenderTextureRotated(
         Senpai::Renderer::get(), // renderer
         texture, // texture
         src, // source rect
         &dstRect, // destination rect
         frame.rotation, // rotation
         &point, // rotation point
         (SDL_FlipMode)flip // flipflags
      );
   }

} // end Senpai