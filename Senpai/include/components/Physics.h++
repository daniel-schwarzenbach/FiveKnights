#pragma once
#include <core/ECS.h++>

namespace Senpai::Components {
   struct Rigidbody : Component {
      Vec2<f32> velocity = {0, 0};
      Vec2<f32> acceleration = {0, 0};
      f32 mass = 1;

      Rigidbody(Vec2<f32> velocity = {0, 0}, Vec2<f32> acceleration = {0, 0}, f32 mass = 1);
   }; 
   REGISTER_COMPONENT_TYPE(Rigidbody);

   struct RectangleCollider : Component {
      // flase if the collider is just a trigger and not a solid object
      bool isSolid = true;
      // bonciness of the object 1 = 100% reflection
      f32 bounciness = 1;
      // the rectangle of the collider
      Rectangle<f32> rect;
      // get effectif rectangle from the transform frame
      Rectangle<f32> get_rect(Frame<f32> const& transform) const;
      // the constructor
      RectangleCollider(Rectangle<f32> rect = {1, 1, 0, 0}, bool isSolid = true);
   };
   REGISTER_COMPONENT_TYPE(RectangleCollider);
}