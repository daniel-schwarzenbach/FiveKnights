#pragma once

#include "../core/Core.h++"
#include "./Texture.h++"

namespace Senpai::Assets{
   struct TileSet final : public Asset{
      public:
         SharedPtr<void> sdlTexture;
         String filename;
         u8 cols = 1;
         u8 rows = 1;
         u16 sizeX = 32;
         u16 sizeY = 32;
         // get the src rectangle of the tile
         Rectangle<f32> operator[] (u16 index) const;

         TileSet(String const& filename, u8 cols, u8 rows, u16 sizeX, u16 sizeY);
   };
};