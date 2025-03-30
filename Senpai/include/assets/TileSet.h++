#pragma once

#include "../core/Core.h++"
#include "./Texture.h++"

namespace Senpai::Assets {
struct TileSet final : public Asset {
  private:
   // can only be loaded by the main-thread
   SharedPtr<void> sdlTexture = nullptr;
   // will be loaded by a seperate thread
   SharedPtr<void> sdlSurface = nullptr;

  public:
   String filename;
   u16 cols = 1;
   u16 rows = 1;
   f32 sizeX = 32;
   f32 sizeY = 32;
   // get the src rectangle of the tile
   PixelArea operator[](u16 index) const;

   TileSet(String const& filename, u16 cols, u16 rows, f32 sizeX, f32 sizeY);

   GenericPtr get_sdl_texture();
};
};  // namespace Senpai::Assets