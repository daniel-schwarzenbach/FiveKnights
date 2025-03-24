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
      /**
       * 
       * @param  {int} channel : the chanal to change the pan of
       * @param  {float} pan   : ∈ [-1, 1]
       */
      static void set_pan(int channel, float pan);

      void play(float volume = 1.0f, bool looping = false);
   };
};