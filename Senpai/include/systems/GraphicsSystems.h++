#pragma once
#include <components/Components.h++>
#include <core/Core.h++>

namespace Senpai::Systems {

struct CameraRenderSystem final : public System {
   void update(f32 dt);
};
REGISTER_SYSTEM_TYPE(CameraRenderSystem);

struct UIRenderSystem final : public System {
   void update(f32 dt);
};
REGISTER_SYSTEM_TYPE(UIRenderSystem);

// checks if the button is hovered or clicked
struct UIButtonSystem final : System {
   void update(f32 dt);
};
REGISTER_SYSTEM_TYPE(UIButtonSystem);

struct SpriteAnimator final : public System {
   void update(f32 dt);
   void start();
};
REGISTER_SYSTEM_TYPE(SpriteAnimator);

struct LightingSystem final : public System {
   void update(f32 dt);
};
}  // namespace Senpai::Systems