#pragma once
#include "../core/ECS.h++"
#include "../core/pch.h"

namespace Senpai::Components {
   
   struct Transform : Component {
      Frame<f32> frame;
      
      Transform(
         Vec2<f32> translate = {0.0f, 0.0f}, 
         Vec2<f32> scale = {1.0f, 1.0f}, 
         f32 rotation = 0.0f,
         Vec2<f32> rotAnchor = {0.5f, 0.5f}
      );
      
      Vec2<f32>& translate();
      Vec2<f32>& scale();
      Vec2<f32>& rotAnchor();
      f32& rotation();
   };
   REGISTER_COMPONENT_TYPE(Transform);

   struct Info : Component {
      String name = "Entity";
      String tag = "default";

      Info(String name = "Entity", String tag = "default");
   }; 
   REGISTER_COMPONENT_TYPE(Info);
}