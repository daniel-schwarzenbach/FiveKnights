#pragma once
#include <core/ECS.h++>
#include <components/Components.h++>

namespace Senpai::Systems {
   struct CameraSystem : System {
      Entity* camera = nullptr;
      inline void update(f32 δt) {};
   };
   REGISTER_SYSTEM_TYPE(CameraSystem);
   struct SpriteRenderer : System {
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(SpriteRenderer);

   struct TextRenderer : System {
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(TextRenderer);

   struct SpriteAnimator : System {
      
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(SpriteAnimator);

   struct TileMapRenderer : System {
      void update(f32 δt);
   };
   REGISTER_SYSTEM_TYPE(TileMapRenderer);
}