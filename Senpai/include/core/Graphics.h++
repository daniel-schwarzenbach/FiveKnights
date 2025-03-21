#pragma once
#include "./Base.h++"
#include "./container/Vec2.h++"

namespace Senpai {

   enum BlendMode {
      Normal = 0x1,
      Add = 0x2,
      Multiply = 0x8,
      Modulate = 0x4
   };

   enum Flip {
      None = 0x0,
      Horizontal = 0x1,
      Vertical = 0x2
   };

   struct Color {
      // r ∈ [0,1] ⟷ {0, ..., 255}
      u8 r = 0;
      // g ∈ [0,1] ⟷ {0, ..., 255}
      u8 g = 0;
      // b ∈ [0,1] ⟷ {0, ..., 255}
      u8 b = 0;
      // a ∈ [0,1] ⟷ {0, ..., 255}
      u8 a = 255;

      // {0, ..., 255} → [0,1]
      static f32 to_f32(u8 color);
      // [0,1] → {0, ..., 255}
      static u8 from_f32(f32 color);

      // add up colors c by multipling the invers color values, c ∈ [0,1]
      Color operator+(const Color &other) const;
      // removes color by multipling the color values, c ∈ [0,1]
      Color operator-(const Color &other) const;
   };

   /*
      Senpai assumes that the Screen will be a 1080 x 1920 pixels
      to get the render scale, the effective Window size is divided by (1080,1920)
      with that Senpai is able to render the same on every screensize
   */
   Vec2<f32> get_render_scale();

} // namespace Senpai