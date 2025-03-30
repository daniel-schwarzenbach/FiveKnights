#pragma once
#include <components/Components.h++>
#include <core/Core.h++>

namespace Senpai::Systems {
struct Physics : System {
   f32 gravity = 9.81f;
   void update(f32 δt);
};
REGISTER_SYSTEM_TYPE(Physics);

struct Collisions : System {
   void update(f32 δt);
};
REGISTER_SYSTEM_TYPE(Collisions);

}  // namespace Senpai::Systems