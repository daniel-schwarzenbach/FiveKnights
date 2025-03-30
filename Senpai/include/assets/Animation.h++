#pragma once

#include "../core/Core.h++"
#include "./Texture.h++"

namespace Senpai::Assets {

/*
```cpp
{Ptr<Texture> texture = nullptr, Vec2<f32> size = {64, 64}, uint frames = 1, 
 String const& name = ""}
```
*/
struct Animation final : public Asset {
  public:
   Ptr<Texture> texturePtr;
   Vector<PixelArea> frameAreas;
   Animation(Ptr<Texture> texture = nullptr, Vec2<f32> size = {64, 64},
             uint frames = 1, String const& name = "");
   ~Animation() = default;
};
};  // namespace Senpai::Assets