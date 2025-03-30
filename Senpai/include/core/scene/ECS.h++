/*
   This file contains late implementation of the ECS system.
*/
#pragma once
#include "./Scene.h++"

namespace Senpai {

template <Senpai::ComponentType ComponentT>
Vector<Ptr<Entity>> System::view() {
   if (scenePtr == nullptr) {
      debug_warning("ScenePtr is null, view() will return empty vector");
      return {};
   } else {
      return scenePtr->ecRegistry.view<ComponentT>();
   }
}

template <ComponentType ComponentT>
Vector<Ptr<Entity>> Script::view() {
   if (scenePtr == nullptr) {
      debug_warning("ScenePtr is null, view() will return empty vector");
      return {};
   } else {
      return scenePtr->ecRegistry.view<ComponentT>();
   }
}

}  // namespace Senpai