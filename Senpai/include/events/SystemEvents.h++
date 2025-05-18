#pragma once

#include "../core/Core.h++"

namespace Senpai::Events {

struct Quit final : public Event {
   inline Quit() = default;
};

struct KeyDown final : public Event {
   u32 key;
   inline KeyDown(u32 key) : key(key) {}
};

struct KeyUp final : public Event {
   u32 key;
   inline KeyUp(u32 key) : key(key) {}
};

struct MouseMove final : public Event {
   f32 x;
   f32 y;
   inline MouseMove(i32 x, i32 y) : x{f32(x)}, y{f32(y)} {}
};

struct MouseDown final : public Event {
   u32 button;
   inline MouseDown(u32 button) : button(button) {}
};

struct MouseUp final : public Event {
   u32 button;
   inline MouseUp(u32 button) : button(button) {}
};

struct MouseWheel final : public Event {
   inline MouseWheel() = default;
};

struct WindowResize final : public Event {
   inline WindowResize() = default;
};

struct SettingsChanged final : public Event {
   AppSettings settings;
   inline SettingsChanged(AppSettings newSettings) : settings(newSettings){};
};

}  // namespace Senpai::Events
