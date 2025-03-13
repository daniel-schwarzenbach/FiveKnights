#include <core/pch.h>
#include <events/Events.h++>
#include <systems/Systems.h++>
#include <components/Components.h++>

namespace Senpai
{
   struct App {
      struct Settings {
         u8 fpsTarget = 60;
         bool vsync = true;
         bool fullscreen = false;
      };
      
      u32 height = 720;
      u32 width = 1280;
      String title = "SenpaiGame";
      String version = "α";
      Settings settings = Settings{};
      bool run();
   };
} // namespace Senpai