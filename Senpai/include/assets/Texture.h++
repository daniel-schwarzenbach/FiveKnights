#pragma once
#include <core/pch.h>

namespace Senpai::Assets {
   struct Texture final : public Asset {
      public:
         GenricPtr sdlTexture;
         String filename;
         f32 height = 1;
         f32 width = 1;

         Texture(const String& filename, String const& name);
         ~Texture();
   };
   
}