#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <assets/Assets.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai::Assets {

Font::Font(const String &filename, int size, String const &name) {
   this->filename = filename;
   this->size = size;
   this->name = name;
   GenericPtr genericPtr = (GenericPtr)TTF_OpenFont(filename.c_str(), size);
   if (genericPtr == nullptr) {
      debug_log("Failed to load font: " + filename);
      cout << SDL_GetError() << endl;
      return;
   }
   sdlFont = SharedPtr<void>(genericPtr, [](void *font) {
      debug_log("Font destroyed");
      TTF_Font *test = (TTF_Font *)font;
      try {
         TTF_CloseFont((TTF_Font *)font);
      } catch (const std::exception &e) {
         cout << e.what() << endl;
      }
   });
};

Texture::Texture(const String &filename, String const &name) {
   this->filename = filename;
   this->name = name;
   // theadsaf
   GenericPtr surfacePtr = IMG_Load(filename.c_str());
   if (surfacePtr == nullptr) {
      debug_log("Failed to load texture-surface: " + filename);
      cout << SDL_GetError() << endl;
      return;
   }
   sdlSurface = SharedPtr<void>(surfacePtr, [](void *texture) {
      debug_log("Surface destroyed");
      SDL_DestroySurface((SDL_Surface *)texture);
   });
   // quickload the texture to get the size, this could be done probebly better
   SDL_Texture* texturePtr = SDL_CreateTextureFromSurface(Renderer::get(),
                                                        (SDL_Surface *)sdlSurface.get());
   
   SDL_GetTextureSize(texturePtr, &this->width, &this->height);
   SDL_DestroyTexture(texturePtr);
};

GenericPtr Texture::get_sdl_texture() {
   if(sdlTexture) {
      return sdlTexture.get();
   } else if (sdlSurface) {
      GenericPtr genericPtr = SDL_CreateTextureFromSurface(Renderer::get(),(SDL_Surface*)sdlSurface.get());
      if (genericPtr == nullptr) {
         debug_log("Failed to load texture: " + filename);
         cout << SDL_GetError() << endl;
         return nullptr;
      }
      sdlTexture = SharedPtr<void>(genericPtr, [](void *texture) {
         debug_log("Surface destroyed");
         SDL_DestroyTexture((SDL_Texture*)texture);
      });
      return sdlTexture.get();
   } else {
      debug_log("No Texture Present!")
      return nullptr;
   }
}

    TileSet::TileSet(String const &filename, u8 cols, u8 rows, u16 sizeX,
                     u16 sizeY)
    : filename(filename), cols(cols), rows(rows), sizeX(sizeX), sizeY(sizeY) {
   GenericPtr genericPtr =
       (GenericPtr)IMG_LoadTexture(Renderer::get(), filename.c_str());
   if (genericPtr == nullptr) {
      debug_log("Failed to load texture: " + filename);
      cout << SDL_GetError() << endl;
      return;
   }
   sdlTexture = SharedPtr<void>(genericPtr, [](void *texture) {
      SDL_DestroyTexture((SDL_Texture *)texture);
   });
};

Rectangle<f32> TileSet::operator[](u16 index) const {
   f32 x = index % cols;
   f32 y = index / cols;
   if (y >= rows) {
      debug_log("TileSet index out of bounds");
      y = 0.0f;
   }
   return {.position = {sizeX / 2.0f + x * sizeX, sizeY / 2.0f + y * sizeY},
           .size = {f32(sizeX), f32(sizeY)}};
};

}; // namespace Senpai::Assets