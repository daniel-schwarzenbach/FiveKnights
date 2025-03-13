#include <assets/Assets.h++>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sdl/SDLBackend.h++>


namespace Senpai::Assets{

   TileSet::~TileSet(){};

   Font::Font(const String& filename, int size, String const& name){
      this->filename = filename;
      this->size = size;
      this->name = name;
      this->sdlFont = (GenricPtr)TTF_OpenFont(filename.c_str(), size);
      if (sdlFont == nullptr) {
         debug_log("Failed to load font: " + filename);
         return;
      }
   };
   Font::~Font(){
      if(sdlFont != nullptr){
         TTF_CloseFont((TTF_Font*)sdlFont);
      }
   };

   Texture::Texture(const String& filename, String const& name){
      this->filename = filename;
      this->name = name;
      this->sdlTexture = (GenricPtr)IMG_LoadTexture(
         Renderer::get(), filename.c_str());
      
      if (sdlTexture == nullptr) {
         debug_log("Failed to load texture: " + filename);
         return;
      }
      int access;
      SDL_GetTextureSize((SDL_Texture*)sdlTexture, &this->width, &this->height);
   };

   Texture::~Texture(){
      if(sdlTexture != nullptr){
         SDL_DestroyTexture((SDL_Texture*)sdlTexture);
      }
   };

   
};