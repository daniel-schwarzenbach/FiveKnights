#pragma once
#include <core/ECS.h++>
#include <components/Components.h++>

namespace Senpai::Systems {
   struct Physics : System {
      f32 gravity = 9.81;
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(Physics);

   struct Collisions : System {
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(Collisions);

}