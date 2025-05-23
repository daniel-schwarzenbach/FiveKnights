#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <assets/Assets.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai::Assets {

Audio::Audio(const String &filename, String const &name) {
   this->filename = filename;
   this->name = name;
   GenericPtr genericPtr = Mix_LoadWAV(filename.c_str());
   if (genericPtr == nullptr) {
      debug_log("Failed to load audio: " + filename);
      cout << SDL_GetError() << endl;
      return;
   }
   sdlAudio = SharedPtr<void>(genericPtr, [](void *audio) {
      debug_log("Audio destroyed");
      Mix_FreeChunk((Mix_Chunk *)audio);
   });
};

void Audio::play(float volume, bool looping) {
   if (sdlAudio) {
      Mix_Chunk *audio = (Mix_Chunk *)sdlAudio.get();

      Mix_VolumeChunk(audio, volume * MIX_MAX_VOLUME);
      Mix_PlayChannel(channel, audio, looping ? -1 : 0);
   }
};

void Audio::play_direction(Vec2<f32> dir, f32 scale) {
   if (!sdlAudio) return;
   // get the distance:
   f32 rawDistance = std::sqrt(dir | dir);
   int distance = int(min(max(rawDistance * scale, 1), 255));
   // get angle
   f32 angle = get_angle(dir) - 90.0f;
   if (angle < 0) angle += 360.0f;

   Mix_Chunk *audio = (Mix_Chunk *)sdlAudio.get();
   Mix_VolumeChunk(audio, MIX_MAX_VOLUME);
   channel = Mix_PlayChannel(-1, audio, 0);
   Mix_SetPosition(channel, i16(angle), u8(distance));
}

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

Texture::Texture(const String &filename, String const &name, bool pp)
    : isPixelPerfect{pp} {
   this->filename = filename;
   this->name = name;
   // theadsaf
   debug_log("Loading Surface: " << filename);
   GenericPtr surfacePtr = IMG_Load(filename.c_str());
   if (surfacePtr == nullptr) {
      debug_error("Failed to load texture-surface from " << filename);
      cout << SDL_GetError() << endl;
      return;
   }
   width = ((SDL_Surface *)surfacePtr)->w;
   height = ((SDL_Surface *)surfacePtr)->h;
   sdlSurface = SharedPtr<void>(surfacePtr, [](void *texture) {
      debug_log("Surface destroyed");
      SDL_DestroySurface((SDL_Surface *)texture);
   });
};

GenericPtr Texture::get_sdl_texture() {
   if (sdlTexture) {
      return sdlTexture.get();
   } else if (sdlSurface) {
      GenericPtr genericPtr = SDL_CreateTextureFromSurface(
          Renderer::get(), (SDL_Surface *)sdlSurface.get());
      if (isPixelPerfect)
         SDL_SetTextureScaleMode((SDL_Texture *)genericPtr,
                                 SDL_SCALEMODE_NEAREST);
      if (genericPtr == nullptr) {
         debug_log("Failed to load texture: " + filename);
         cout << SDL_GetError() << endl;
         return nullptr;
      }
      sdlTexture = SharedPtr<void>(genericPtr, [](void *texture) {
         debug_log("Texture destroyed");
         SDL_DestroyTexture((SDL_Texture *)texture);
      });
      SDL_GetTextureSize((SDL_Texture *)sdlTexture.get(), &this->width,
                         &this->height);
      return sdlTexture.get();
   } else {
      debug_log("No Texture Present!") return nullptr;
   }
}

TileSet::TileSet(String const &filename, u16 cols, u16 rows, f32 sizeX,
                 f32 sizeY, String const &name)
    : filename(filename), cols(cols), rows(rows), sizeX(sizeX), sizeY(sizeY) {
   if (name == "") {
      this->name = "TileSet";
   } else {
      this->name = name;
   }
   GenericPtr genericPtr = (GenericPtr)IMG_Load(filename.c_str());
   if (genericPtr == nullptr) {
      debug_log("Failed to load texture: " + filename);
      cout << SDL_GetError() << endl;
      return;
   }
   sdlSurface = SharedPtr<void>(genericPtr, [](void *surface) {
      SDL_DestroySurface((SDL_Surface *)surface);
   });
};

GenericPtr TileSet::get_sdl_texture() {
   if (sdlTexture) {
      return sdlTexture.get();
   } else if (sdlSurface) {
      GenericPtr genericPtr = SDL_CreateTextureFromSurface(
          Renderer::get(), (SDL_Surface *)sdlSurface.get());
      // tileset should always be pixel perfect
      SDL_SetTextureScaleMode((SDL_Texture *)genericPtr, SDL_SCALEMODE_NEAREST);
      if (genericPtr == nullptr) {
         debug_log("Failed to load texture: " + filename);
         cout << SDL_GetError() << endl;
         return nullptr;
      }
      sdlTexture = SharedPtr<void>(genericPtr, [](void *texture) {
         debug_log("Surface destroyed");
         SDL_DestroyTexture((SDL_Texture *)texture);
      });
      return sdlTexture.get();
   } else {
      debug_log("No Texture Present!") return nullptr;
   }
}

PixelArea TileSet::operator[](u16 index) const {
   f32 x = index % cols;
   f32 y = index / cols;
   if (y >= rows) {
      debug_log("TileSet index out of bounds");
      y = 0.0f;
   }
   return {.position = {x * sizeX, y * sizeY},
           .size = {f32(sizeX), f32(sizeY)}};
};

Animation::Animation(Ptr<Texture> texture, Vec2<f32> size, uint frames,
                     String const &name)
    : texturePtr(texture) {
   if (name == "") {
      this->name = "Animation";
   } else {
      this->name = name;
   }
   for (uint i = 0; i < frames; i++) {
      frameAreas.push_back({.position = {i * size.x, 0}, .size = size});
   }
}

};  // namespace Senpai::Assets