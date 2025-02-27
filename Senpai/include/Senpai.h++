#include <core/pch.h>

namespace Senpai
{
   struct App {
      u32 width = 0;
      u32 height = 0;
      String title;
      String version;
      
      bool run();
   };
} // namespace Senpai
