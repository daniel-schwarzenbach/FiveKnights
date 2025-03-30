#pragma once
#include "../container/Geometry.h++"
#include "./ECSBase.h++"

namespace Senpai {
struct Component {
   Ptr<Entity> entityPtr = nullptr;
   u32 id = 0;
   virtual ~Component() = default;
   // input
   inline virtual void in(IS &) {};
   // output
   inline virtual void out(OS &) {};
   // input operator
   inline friend IS &operator>>(IS &is, Component &component) {
      component.in(is);
      return is;
   }
   // output operator
   inline friend OS &operator<<(OS &os, Component &component) {
      component.out(os);
      return os;
   }
};

// all the elements that get rendered by the camera
struct RenderComponent : public Component {
   // flip mode of the renderable
   u8 flip = 0;
   // virtual z axis that determines the render order
   f32 z = 0;
   void flip_vertical();
   void flip_horizontal();
   virtual void render(Rectangle<f32> const &camera,
                       Rectangle<f32> const &canvas) {};
   template <ComponentType ComponentT>
   static bool is_sub_type() {
      return std::is_base_of_v<RenderComponent, ComponentT>;
   }
   // find out if the component is a sub type of RenderComponent
   static bool is_sub_type(Ptr<Component> component);
   // compare the z values of two CameraRenderComponents
   bool operator<(const RenderComponent &other) const;
};

// all elements that are rendered after the  camera renderable components
struct UIComponent : public Component {
   // flip mode of the renderable
   u8 flip = 0;
   // virtual z axis that determines the render order
   f32 z = 0;
   void flip_vertical();
   void flip_horizontal();
   virtual void render() {};
   template <ComponentType ComponentT>
   static bool is_sub_type() {
      return std::is_base_of_v<UIComponent, ComponentT>;
   }
   // find out if the component is a public sub type of UIComponent
   static bool is_sub_type(Ptr<Component> component);
   // compare the z values of two UIComponents
   bool operator<(const UIComponent &other) const;
};
}  // namespace Senpai