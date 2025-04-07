#pragma once
#include <core/container/Geometry.h++>
#include <core/scene/Component.h++>
#include <core/scene/Script.h++>

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

   Vec2<f32>& translate();
   Vec2<f32>& scale();
   Vec2<f32>& rotAnchor();
   f32& rotation();
};
REGISTER_COMPONENT_TYPE(Transform);

/*
   ```cpp
   Info{String name = "Entity", String tag = "default"};
   ```
*/
struct Info final : public Component {
   String name = "Entity";
   String tag = "default";

   Info(String name = "Entity", String tag = "default");
};
REGISTER_COMPONENT_TYPE(Info);

struct ScriptsHolder final : public Component {
   Deque<UniquePtr<Script>> scripts = {};
   Map<u32, u32> scriptTypeMap = {};

   ScriptsHolder(ScriptsHolder const& other);
   ScriptsHolder& operator=(ScriptsHolder const& other);

   template <ScriptType ScriptT, typename... Args>
   ScriptT& add_script(Args&&... args) {
      UniquePtr<Script> script =
          make_unique<ScriptT>(std::forward<Args>(args)...);
      // register the script with the registry
      u32 key = script_type_id<ScriptT>();
      scriptTypeMap.try_emplace(key, scripts.size());
      script->prepare(nullptr, entityPtr);
      scripts.push_back(move(script));
      // set the script settings
      return *static_cast<Ptr<ScriptT>>(scripts.back().get());
   }

   template <ScriptType ScriptT>
   ScriptT& has_script() {
      return scriptTypeMap.find(script_type_id<ScriptT>()) !=
             scriptTypeMap.end();
   }

   template <ScriptType ScriptT>
   ScriptT& get_script() {
      u32 index = scriptTypeMap[script_type_id<ScriptT>()];
      if (index < scripts.size()) {
         return *static_cast<Ptr<ScriptT>>(scripts[index].get());
      }
      throw SenpaiException("Script not found");
   }

   ScriptsHolder();
};
REGISTER_COMPONENT_TYPE(ScriptsHolder);
}  // namespace Senpai::Components