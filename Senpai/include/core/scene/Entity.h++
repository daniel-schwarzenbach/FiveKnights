#pragma once
#include "./ECRegistry.h++"
#include <components/CommonComponents.h++>

namespace Senpai {

class Entity {
   //    Variables
 private:
   // the registry that the entity is registered with, null if dead
   Ptr<ECRegistry> ecRegistryPtr = nullptr;
   // is enabled
   bool enabled = false;

 public:
   // the parentPtr of the entity, null if no parentPtr
   Ptr<Entity> parentPtr = nullptr;
   // components store, refrence safe
   Deque<UniquePtr<Component>> components;
   // component map
   Map<u32, u32> componentsMap;
   // define the constructors
   Entity();
   inline ~Entity() = default;
   Entity(Entity const &other);
   Entity &operator=(Entity const &other);

   //    Functions
   // set the registry of the entity
   void set_ecRegistry(Ptr<ECRegistry> ecRegistryPtr, bool isAlive);
   // get the total transform of the entity including the parents
   Frame<f32> get_transform();
   // register a copy of the componentn
   void enable();
   // add a component to the entity
   template <ComponentType ComponentT, typename... Args>
   inline ComponentT& add_component(Args&&... args) {
      // make the component
      UniquePtr<Component> componentPtr =
          make_unique<ComponentT>(std::forward<Args>(args)...);
      // set the component settings
      componentPtr->entityPtr = this;
      // make sure the component has an the correct type id
      const u32 key = component_type_id<ComponentT>();
      componentPtr->id = key;
      // add the component to the entity
      components.push_back(move(componentPtr));
      // add the component to the component map
      componentsMap.try_emplace(key, components.size() - 1);
      // register the compoenent with the registry
      if (ecRegistryPtr != nullptr && is_enabled()) {
         ecRegistryPtr->register_component(components.back().get(), this);
      }
      return *dynamic_cast<Ptr<ComponentT>>(components.back().get());
   }
   // add empty component
   // template <ComponentType ComponentT> 
   // inline ComponentT& add_component() {
   //    return add_component<ComponentT>(ComponentT());
   // }

   template <ComponentType ComponentT> inline ComponentT &get_component() {
      u32 index = componentsMap[component_type_id<ComponentT>()];
      Ptr<Component> component = components[index].get();
      if (!component) {
         throw SenpaiException("no Such Component present!");
      }
      // cast the Component to ComponentT
      Ptr<ComponentT> derivedComponent = dynamic_cast<ComponentT *>(component);
      if (!derivedComponent) {
         throw SenpaiException("Component is not of the correct type!");
      }
      return *derivedComponent;
   }

   template <ComponentType ComponentT> inline void remove_component() {
      if (ecRegistryPtr != nullptr)
         ecRegistryPtr->unregister_component(component_type_id<ComponentT>(),
                                             this);
      u32 index = componentsMap[component_type_id<ComponentT>()];
      // we let the component inside the deque for refrence safety reasons
      // components.remove(index);
      // if the component is not the last component
      if (index < components.size()) {
         // update the component map
         componentsMap[components[index]->id] = index;
      }
      // remove the component from the component map
      componentsMap.erase(component_type_id<ComponentT>());
   }

   template <ComponentType ComponentT> inline bool has_component() {
      return componentsMap.find(component_type_id<ComponentT>()) !=
             componentsMap.end();
   }
   // is this Componentn registered
   bool is_enabled() const;
   // set the Entitiy incactive
   void disable();
   // add a script to the entity
   template <ScriptType ScriptT, typename... Args>
   inline ScriptT& add_script(Args&&... args) {
      if (!this->has_component<Components::ScriptsHolder>()) {
         this->add_component<Components::ScriptsHolder>();
      }
      Components::ScriptsHolder &sh =
          get_component<Components::ScriptsHolder>();
      return sh.add_script<ScriptT>(std::forward<Args>(args)...);
   }
};
} // namespace Senpai