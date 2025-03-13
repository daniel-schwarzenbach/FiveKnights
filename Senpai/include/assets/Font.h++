#pragma once

#include "../core/pch.h"

namespace Senpai::Assets{
   struct Font final : public Asset{
      GenricPtr sdlFont = nullptr;
      String filename = "";
      int size = 0;
      // loads the font from the file
      Font(const String& filename, int size, String const& name);
      // destroys the font
      ~Font();
   };
};