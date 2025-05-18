#pragma once
#include <core/Base.h++>

namespace Senpai {

/*
```cpp
AppSettings{
   i8 fullscreen = -1;
   i16 fps = -1;
   int width = -1;
   int height = -1;
}
```
*/
struct AppSettings {
   // change fullscreen
   // if 0 = false, 1 = true, -1 no change,
   i8 fullscreen = -1;
   // 0 for unlimited
   i16 fps = -1;
   // the width of the window
   int width = -1;
   // the height of the window
   int height = -1;
};

}  // namespace Senpai

inline OS &operator<<(OS &os, Senpai::AppSettings const &s) {
   os << "AppSettings{" << s.fullscreen << ", " << s.fps << ", " << s.width
      << ", " << s.height << "}";
   return os;
}

inline IS &operator>>(IS &is, Senpai::AppSettings &s) {
   is >> "AppSettings{" >> s.fullscreen >> "," >> s.fps >> "," >> s.width >>
       "," >> s.height >> "}";
   return is;
}