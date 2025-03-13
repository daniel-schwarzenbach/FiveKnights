#pragma once

#include "../core/pch.h"
#include "./Texture.h++"

namespace Senpai::Assets{
   struct TileSet final : public Asset{
      private:
         

      public:
         struct SlicedTexture {
            Ptr<Texture> texturePtr;
            uint cols = 1;
            uint rows = 1;
         };
         // max size of the tiles is 256! 8bit Tile engine
         Vector<Texture> tiles = {};
         uint tileSize = 16;
         ~TileSet();
   };
};