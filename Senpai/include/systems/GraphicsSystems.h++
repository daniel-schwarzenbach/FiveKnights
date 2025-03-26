#pragma once
#include <core/Core.h++>
#include <components/Components.h++>

namespace Senpai::Systems {

   struct CameraRenderSystem final : public System {
      void update(f32 Δt);
   };
   REGISTER_SYSTEM_TYPE(CameraRenderSystem);

   struct UIRenderSystem final : public System {
      void update(f32 Δt);
   };
   REGISTER_SYSTEM_TYPE(UIRenderSystem);

   // checks if the button is hovered or clicked
   struct UIButtonSystem final : System {
      void update(f32 Δt);
   };
   REGISTER_SYSTEM_TYPE(UIButtonSystem);

   struct SpriteAnimator final : public System {
      void update(f32 Δt);
      void start();
   };
   REGISTER_SYSTEM_TYPE(SpriteAnimator);

   struct LightingSystem final : public System {
      void update(f32 Δt);
   };
}