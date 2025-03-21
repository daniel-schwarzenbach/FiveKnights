#pragma once

#include "../core/Core.h++"
#include "./Texture.h++"

namespace Senpai::Assets{
   struct Animation final : public Asset{
      public:
         Vector<Ptr<Texture>> frames;
         f32 speed = 30;

         ~Animation() = default;
   };
};