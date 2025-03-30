#pragma once

#include <core/Inputs.h++>

#include "../Event.h++"
#include "./Asset.h++"
#include "./ECSBase.h++"

namespace Senpai {

// Script can be attached to an entity via the Components::ScriptAttacher
struct Script {
  protected:
   Ptr<Scene> scenePtr = nullptr;
   Ptr<Entity> entityPtr = nullptr;

  public:
   // prepare the script, called by the ScriptRunner on start()
   inline void prepare(Ptr<Scene> scenePtr, Ptr<Entity> entityPtr) {
      this->scenePtr = scenePtr;
      this->entityPtr = entityPtr;
   }

   template <ComponentType ComponentT>
   Vector<Ptr<Entity>> view();

   inline virtual ~Script() = default;
   // called when an entity holding this script is copied
   inline virtual UniquePtr<Script> clone() {
      throw SenpaiException(
          "Script::clone() must be implemented for each "
          "Script subtype for copyable Entities!");
      return nullptr;
   };
   // called when a collider component collides with another collider component
   inline virtual void on_collision(Ptr<Entity> other) {};
   // called when the entity is destroyed
   inline virtual void on_destroy() {};
   // called when the entity is enabled
   inline virtual void on_enable() {};
   // called every frame
   inline virtual void on_update(f32 dt) {};
   // called on the start of the scene
   inline virtual void on_start() {};
   // called when the mouse is clicked on the entity
   inline virtual void on_button_click() {};
   // called when pause specific event is posted
   inline virtual void on_pause() {};
   // called when resume event is posted
   inline virtual void on_resume() {};
};

template <typename T>
concept ScriptType = std::is_base_of_v<Script, T>;

}  // namespace Senpai