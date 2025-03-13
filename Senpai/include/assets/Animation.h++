#pragma once

#include "../core/pch.h"
#include "./Texture.h++"

namespace Senpai::Assets{
   struct Animation final : public Asset{
      public:
         Vector<Ptr<Texture>> frames;
         f32 speed = 30;

         ~Animation() = default;
   };
};