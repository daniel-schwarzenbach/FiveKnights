#pragma once
#include "../components/Components.h++"

namespace Senpai::Systems {
   struct TestSystem final : public System {
      void update(f32 δt)  {
         for (auto& entity : view<Components::Transform>()){
            auto& transform = entity->get_component<Components::Transform>();
            transform.frame.position.x += 1.0f;
            debug_log("Transform: " << transform.frame.position);
            debug_log("delta t = " << δt)
         }
      }
   };
   REGISTER_SYSTEM_TYPE(TestSystem);
}