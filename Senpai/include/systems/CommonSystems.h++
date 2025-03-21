#include <core/Core.h++>

namespace Senpai::Systems {
   struct ScriptRunner final : public System {
      void update(f32 δt);
      void start();
   };
   REGISTER_SYSTEM_TYPE(ScriptRunner);
}