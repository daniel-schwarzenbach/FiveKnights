/*
   This file should only be included when using the SDL backend directly.
*/
#pragma once
#include <Senpai.h++>
#include <SDL3/SDL.h>

namespace Senpai{

   namespace SDL {
      // needs to be called first, init whole sdl package
      bool init(String title, u32 width, u32 height);
      // quits all sdl packages
      void quit();
   }

   namespace Renderer {
      // get the renderer
      Ptr<SDL_Renderer> get();
      // clear the renderer
      void clear();
      // present the renderer
      void present();
   }

   namespace Window {
      // get the window
      Ptr<SDL_Window> get();
      // get the size of the window
      Senpai::Vec2<u32> get_size();
      // get the aspect ratio of the window
      f32 get_aspect_ratio();
   }

   SDL_FRect to_sdl(const Senpai::Rectangle<f32>& rect);
   SDL_Rect to_sdl(const Senpai::Rectangle<int>& rect);


   SDL_Rect to_sdl_viewport(const Senpai::Rectangle<f32>& rect);

   void SDL_RenderFrame(Senpai::Frame<f32> const& frame, SDL_Texture* texture , u8 flip, SDL_FRect*  src = nullptr);

}
