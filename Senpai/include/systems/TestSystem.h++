#pragma once
#include "../components/Components.h++"

namespace Senpai::Systems {
   struct TestSystem final : public System {
      void update(f32 Δt) override;
   };
   REGISTER_SYSTEM_TYPE(TestSystem);
}