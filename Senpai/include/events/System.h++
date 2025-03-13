#pragma once

#include "../core/Event.h++"

namespace Senpai::Event {

   struct Quit final : EventInstance {
      inline Quit() = default;
   };

   struct KeyDown final : EventInstance {
      u32 key;
      inline KeyDown(u32 key) : key(key) {}
   };

   struct KeyUp final : EventInstance {
      u32 key;
      inline KeyUp(u32 key) : key(key) {}
   };

   struct MouseMove final : EventInstance {
      f32 x;
      f32 y;
      inline MouseMove(i32 x, i32 y) : x(x), y(y) {}
   };

   struct MouseDown final : EventInstance {
      u32 button;
      inline MouseDown(u32 button) : button(button) {}
   };

   struct MouseUp final : EventInstance {
      u32 button;
      inline MouseUp(u32 button) : button(button) {}
   };

   struct MouseWheel final : EventInstance {
      inline MouseWheel() = default;
   };

} // namespace Senpai::event
