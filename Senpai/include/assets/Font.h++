#pragma once

#include <core/Core.h++>

namespace Senpai::Assets {
/*
```cpp
   Font{String filename, int size, String name}
```
*/
struct Font final : public Asset {
   SharedPtr<void> sdlFont = nullptr;
   String filename = "";
   int size = 0;
   // loads the font from the file
   Font(const String& filename, int size, String const& name);
};
};  // namespace Senpai::Assets