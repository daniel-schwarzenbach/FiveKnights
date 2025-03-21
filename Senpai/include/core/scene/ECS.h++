/*
   This file contains late implementation of the ECS system.
*/
#pragma once
#include "./Scene.h++"

template <Senpai::ComponentType ComponentT>
Vector<Ptr<Senpai::Entity>> Senpai::System::view()
{
   if (scenePtr == nullptr)
   {
      return {};
   }
   else
   {
      return scenePtr->ecRegistry.view<ComponentT>();
   }
}