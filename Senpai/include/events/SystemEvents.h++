#pragma once

#include "../core/Event.h++"

namespace Senpai::Events {

   struct Quit final : Event {
      inline Quit() = default;
   };

   struct KeyDown final : Event {
      u32 key;
      inline KeyDown(u32 key) : key(key) {}
   };

   struct KeyUp final : Event {
      u32 key;
      inline KeyUp(u32 key) : key(key) {}
   };

   struct MouseMove final : Event {
      f32 x;
      f32 y;
      inline MouseMove(i32 x, i32 y) : x(x), y(y) {}
   };

   struct MouseDown final : Event {
      u32 button;
      inline MouseDown(u32 button) : button(button) {}
   };

   struct MouseUp final : Event {
      u32 button;
      inline MouseUp(u32 button) : button(button) {}
   };

   struct MouseWheel final : Event {
      inline MouseWheel() = default;
   };

   struct WindowResize final : Event {
      inline WindowResize() = default;
   };

} // namespace Senpai::event
