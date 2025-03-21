#pragma once

#include <core/Core.h++>

namespace Senpai::Assets{
   struct Audio final : public Asset{
      private:
         SharedPtr<void> sdlAudio = nullptr;
         String filename = "";
   };
};