#pragma once
#include <core/scene/Component.h++>
#include <core/scene/Script.h++>
#include <core/container/Geometry.h++>

namespace Senpai::Components {

/*
   ```cpp
   { Vec2<f32> translate = {0.0f, 0.0f}, Vec2<f32> scale = {1.0f, 1.0f},
     f32 rotation = 0.0f, Vec2<f32> rotAnchor = {0.5f, 0.5f} }
   ```
*/
struct Transform final : public Component {
   Frame<f32> frame;

   Transform(Vec2<f32> translate = {0.0f, 0.0f}, Vec2<f32> scale = {1.0f, 1.0f},
             f32 rotation = 0.0f, Vec2<f32> rotAnchor = {0.5f, 0.5f});

   Vec2<f32> &translate();
   Vec2<f32> &scale();
   Vec2<f32> &rotAnchor();
   f32 &rotation();
};
REGISTER_COMPONENT_TYPE(Transform);

struct Info final : public Component {
   String name = "Entity";
   String tag = "default";

   Info(String name = "Entity", String tag = "default");
};
REGISTER_COMPONENT_TYPE(Info);

struct ScriptsHolder final : public Component {
   Vector<UniquePtr<Script>> scripts = {};
   
   ScriptsHolder(ScriptsHolder const& other);
   ScriptsHolder& operator=(ScriptsHolder const& other);

   template<ScriptType ScriptT, typename... Args>
   ScriptT& add_script(Args&&... args){
      UniquePtr<Script> script = make_unique<ScriptT>(std::forward<Args>(args)...);
      scripts.push_back(move(script));
      // set the script settings
      return *static_cast<Ptr<ScriptT>>(scripts.back().get());
   }

   ScriptsHolder();
};
REGISTER_COMPONENT_TYPE(ScriptsHolder);
} // namespace Senpai::Components