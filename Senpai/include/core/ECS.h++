#pragma once
#include "../core/pch.h"

namespace Senpai{
   // every Senpai::App is a collection of Scenes
   struct Scene;

   /*
   ```cpp
   {
      Ptr<Entity> entityPtr = nullptr;
      u32 id = 0;
      virtual ~Component() = default;
   };
   ```
   A reusable part of an Entity
   every Component-subtype must be registered, in order to be copyed or read, with the macro: `REGISTER_COMPONENT_TYPE(...)`
   example:
   ```cpp
   struct SubType final : public Component;
   REGISTER_COMPONENT_TYPE(SubType);
   ```
   */
   struct Component;
   // A collection of Components
   struct Entity;
   /*
   ```cpp
   {
      protected:
         Ptr<Scene> scenePtr = nullptr;
      public:
         bool canBePaused = false;
         inline virtual ~System() = default;
         // assings a system to the correct registries
         void prepare(Ptr<Scene> scenePtr);
         // get all entities with a certain component
         template<ComponentType ComponentT>
         Vector<Ptr<Entity>> view();
         // update the system, called every frame
         inline virtual void update(f32 δt){};
         // start the system, called once
         inline virtual void start(){};
   };
   ```
   Systems describe how the components interact with each other.
   every System-subtype must be registered, in order to be copyed or read, with the macro: `REGISTER_SYSTEM_TYPE(...)`
   example:
   ```cpp
   struct SubType final : public System;
   REGISTER_SYSTEM_TYPE(SubType);
   ```
   */
   struct System;
   // A collection of Entities
   struct ECRegistry;

   template<typename T>
   concept ComponentType = std::is_base_of_v<Component, T>;

   template<typename T>
   concept SystemType = std::is_base_of_v<System, T>;

   /*
   The ComponentTypeRegistry is a static class that keeps track of all registered components. this allows us to deduct the type of a component from its id and use the id to create a new component of the same type.
   */
   class ComponentTypeRegistry {
      private:
         static bool isInitialized;
         static void init();
      public:
         // A mapping from stable type name -> runtime id
         static Map<String, u32> name_to_id;
         // A mapping from runtime id -> constructor
         static Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>> id_to_constructor;
         // register a new component type
         template<ComponentType ComponentT>
         inline static u32 register_component_type(const String& typeName, u32 typeId) {
            // Check for initialization to avoid static initialization order fiasco
            if (!isInitialized) {
               init();
            }
            // Only register if not already registered.
            if (!id_to_constructor.contains(typeId)) {
               name_to_id.try_emplace(typeName, typeId);
               id_to_constructor.try_emplace(typeId, [](Ptr<Component> component) {
                  // check for nullptr
                  if(component)
                     return make_unique<ComponentT>(*static_cast<ComponentT*>(component));
                  else
                     return make_unique<ComponentT>();
               });
            }
            return typeId;
         }
   };

   // assings each component type an id, will always return the same id for the same type and a different id for different types
   template<ComponentType ComponentT>
   constexpr u32 component_type_id() {
      static const u32 id = get_subtype_id<Component>();
      return id;
   }

   // register a new component type, each component type must be registered with this macro!
   #define REGISTER_COMPONENT_TYPE(type) static u32 id##type = ComponentTypeRegistry::register_component_type<type>(String(#type),  component_type_id<type>())

   /*
   The SystemTypeRegistry is a static class that keeps track of all registered systems. this allows us to deduct the type of a system from its id and use the id to create a new system of the same type.
   */
   class SystemTypeRegistry {
      private:
         static bool isInitialized;
         static void init();
      public:
         // A mapping from stable type name -> runtime id
         static Map<String, u32> name_to_id;
         // A mapping from runtime id -> constructor
         static Map<u32, Function<UniquePtr<System>(Ptr<System>)>> id_to_constructor;
         // register a new system type
         template<SystemType SystemT>
         inline static u32 register_system_type(const String& typeName, u32 typeId) {
            // Check for initialization to avoid static initialization order fiasco
            if (!isInitialized) {
               init();
            }
            // Only register if not already registered.
            if (!id_to_constructor.contains(typeId)) {
               name_to_id.try_emplace(typeName, typeId);
               id_to_constructor.try_emplace(typeId, [](Ptr<System> system) {
                  // check for nullptr
                  if(system)
                     return make_unique<SystemT>(*static_cast<SystemT*>(system));
                  else
                     return make_unique<SystemT>();
               });
            }
            return typeId;
         }
   };
   
   // assings each system type an id, will always return the same id for the same type and a different id for different types
   template<SystemType SystemT>
   constexpr u32 system_type_id() {
      static const u32 id = get_subtype_id<System>();
      return id;
   }

   // register a new system type, each system type must be registered with this macro!
   #define REGISTER_SYSTEM_TYPE(type) static u32 id##type = SystemTypeRegistry::register_system_type<type>(String(#type),  system_type_id<type>())


   struct Component {
      Ptr<Entity> entityPtr = nullptr;
      u32 id = 0;
      virtual ~Component() = default;
   };

   

   // Handles all Entities and Components
   struct ECRegistry {
      private:
         // Keep track of all components and there entities
         Vector<Vector<Ptr<Entity>>> componentId_to_entityPtrs;
         // Contains all entities
         Vector<Entity> entities;
         // Contains all free ids in the entities vector
         Set<u32> freeIds;
      public:

      void clear();

      // add an new entity to the registry
      Ptr<Entity> add_entity(Entity&& entity, bool isAlive = true);
      // remove an entity from the registry
      void remove_entity(Ptr<Entity> entity);

      // enable an entity
      void revive(Ptr<Entity> entity);
      // disable an entity
      void destroy(Ptr<Entity> entity);

      template<ComponentType ComponentT>
      Vector<Ptr<Entity>>& view() {
         u32 componentId = component_type_id<ComponentT>();
         if(this->componentId_to_entityPtrs.size() <= componentId){
            componentId_to_entityPtrs.resize(componentId);
         }
         return componentId_to_entityPtrs[componentId];
      }

      void register_component(u32 componentId, Ptr<Entity> entityPtr);
      void unregister_component(u32 componentId, Ptr<Entity> entityPtr);
   };

   class Entity {
         //    Variables
      private:
         // the registry that the entity is registered with, null if dead
         Ptr<ECRegistry> registryPtr = nullptr;
      public:
         // the parentPtr of the entity, null if no parentPtr
         Ptr<Entity> parentPtr = nullptr;
         // components store
         Vector<UniquePtr<Component>> components;
         // component map
         Map<u32, u32> componentsMap;
         // define the constructors
         Entity();
         inline ~Entity() = default;
         Entity(Entity const& other);
         Entity& operator=(Entity const& other);

         //    Functions
         // get the total transform of the entity including the parents
         Frame<f32> get_transform();
         // register a copy of the componentn
         void revive(Ptr<ECRegistry> registryPtr);
         // add a component to the entity
         template<ComponentType ComponentT>
         inline void add_component(ComponentT&& component){
            // make a copy of the component
            UniquePtr<Component> componentPtr = make_unique<ComponentT>(move(component));
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
            if(registryPtr != nullptr)
                  registryPtr->register_component(key, this);
         }
         // add empty component
         template<ComponentType ComponentT>
         inline void add_component(){
            add_component(ComponentT());
         }

         template<ComponentType ComponentT>
         inline ComponentT& get_component(){
            u32 index = componentsMap[component_type_id<ComponentT>()];
            Ptr<Component> component = components[index].get();
            if (!component) {
               throw SenpaiException("no Such Component present!");
            }
            // cast the Component to ComponentT
            ComponentT* derivedComponent = static_cast<ComponentT*>(component);
            if (!derivedComponent) {
                throw std::bad_cast();
            }
            return *derivedComponent;
         }

         template<ComponentType ComponentT>
         inline void remove_component(){
            if(registryPtr != nullptr)
               registryPtr->unregister_component(component_type_id<ComponentT>(), this);
            u32 index = componentsMap[component_type_id<ComponentT>()];
            components.remove(index);
            // if the component is not the last component
            if (index < components.size()) {
               // update the component map
               componentsMap[components[index]->id] = index;
            }
            // remove the component from the component map
            componentsMap.erase(component_type_id<ComponentT>());
         }

         template<ComponentType ComponentT>
         inline bool has_component(){
            return componentsMap.find(component_type_id<ComponentT>()) != componentsMap.end();
         }
         // is this Componentn registered
         bool is_alive() const;
         // set the Entitiy incactive
         void destroy();
   };

   struct System {
      protected:
         Ptr<Scene> scenePtr = nullptr;
      // end protected
      public:
         bool canBePaused = false;

         inline virtual ~System() = default;
         // assings a system to the correct registries
         void prepare(Ptr<Scene> scenePtr);
         // get all entities with a certain component
         template<ComponentType ComponentT>
         Vector<Ptr<Entity>> view();
         // update the system, called every frame
         inline virtual void update(f32 δt){};
         // start the system, called once
         inline virtual void start(){};
      // end public
   };
} // namespace Senpai::ECS

namespace Senpai {

   struct Scene {
      bool isPaused = false;
      Vector<UniquePtr<System>> systems;
      // contains all Entities with their Component
      ECRegistry ecRegistry;
      // contains all Assets
      AssetRegistry assetRegistry;


      // copy an entity into the scene
      void add_entity(Entity&& entity, bool isAlive = true);

      // remove an entity from the scene
      void remove_entity(Ptr<Entity> toRemove);

      // update all systems
      void update(f32 δt);

      // start all systems
      void start();

      // ads a system to the scene
      template<SystemType SystemT>
      void register_system(SystemT&& system) {
         systems.push_back(make_unique<SystemT>(system));
         systems.back()->prepare(this);
      }

      // load all assets in the scene
      void load();

      // unload
      void unload();
   };
};


template<Senpai::ComponentType ComponentT>
Vector<Ptr<Senpai::Entity>> Senpai::System::view(){
   if(scenePtr == nullptr){
      return {};
   } else {
      return scenePtr->ecRegistry.view<ComponentT>();
   }
}