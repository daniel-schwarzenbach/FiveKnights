#pragma once

#include <core/Core.h++>

namespace Senpai::Assets{
   /*
   ```cpp
      Audio{const String& filename, String const& name}
   ```
   */
   struct Audio final : public Asset{
      int channel = -1;
      SharedPtr<void> sdlAudio = nullptr;
      String filename = "";
      String name = "";
      // loads the audio from the file
      Audio(const String& filename, String const& name);
      

      void play(float volume = 1.0f, bool looping = false);
      // plays an audio from a specific direction
      void play_direction(Vec2<f32> direction, f32 scale = 1.0f);
   };
}