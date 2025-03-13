#pragma once

#include "../core/pch.h"

namespace Senpai::Assets{
   struct Audio final : public Asset{
      private:
         GenricPtr sdlAudio = nullptr;
         String filename = "";

      public:
         ~Audio();
   };
};