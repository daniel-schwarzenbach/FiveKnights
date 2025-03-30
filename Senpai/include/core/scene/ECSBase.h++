#pragma once
#include "../Base.h++"
#include "../container/Vector.h++"

namespace Senpai {
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
every Component-subtype must be registered, in order to be copyed or read, with
the macro: `REGISTER_COMPONENT_TYPE(...)` example:
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
every System-subtype must be registered, in order to be copyed or read, with the
macro: `REGISTER_SYSTEM_TYPE(...)` example:
```cpp
struct SubType final : public System;
REGISTER_SYSTEM_TYPE(SubType);
```
*/
struct System;
// A collection of Entities
struct ECRegistry;

template <typename T>
concept ComponentType = std::is_base_of_v<Component, T>;

template <typename T>
concept SystemType = std::is_base_of_v<System, T>;

/*
The ComponentTypeRegistry is a static class that keeps track of all registered
components. this allows us to deduct the type of a component from its id and use
the id to create a new component of the same type.
*/
class ComponentTypeRegistry {
  private:
   // static bool isInitialized;
   // static void init();

  public:
   // A mapping from stable type name -> runtime id
   // static Map<String, u32> name_to_id;
   // A mapping from runtime id -> constructor
   // static Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>
   // id_to_constructor; only way to initialize the id_to_constructor map on
   // windows
   static Map<String, u32>& get_name_to_id() {
      static Map<String, u32> name_to_id = {};
      return name_to_id;
   }
   // only way to initialize the id_to_constructor map on windows
   static Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>&
   get_id_to_constructor() {
      static Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>
          id_to_constructor = {};
      return id_to_constructor;
   }
   // register a new component type
   template <ComponentType ComponentT>
   inline static u32 register_component_type(const String& typeName,
                                             u32 typeId) {
      // Check for initialization to avoid static initialization order fiasco
      // if (!isInitialized)
      //{
      //   init();
      //}
      // Only register if not already registered.
      if (!get_id_to_constructor().contains(typeId)) {
         get_name_to_id().try_emplace(typeName, typeId);
         get_id_to_constructor().try_emplace(
             typeId, [](Ptr<Component> component) {
                // check for nullptr
                if (component)
                   return make_unique<ComponentT>(
                       *dynamic_cast<ComponentT*>(component));
                else
                   return make_unique<ComponentT>();
             });
      }
      return typeId;
   }
};

// assings each component type an id, will always return the same id for the
// same type and a different id for different types
template <ComponentType ComponentT>
constexpr u32 component_type_id() {
   static const u32 id = get_subtype_id<Component>();
   return id;
}

// register a new component type, each component type must be registered with
// this macro!
#define REGISTER_COMPONENT_TYPE(type)                                          \
   static u32 id##type = ComponentTypeRegistry::register_component_type<type>( \
       String(#type), component_type_id<type>())

/*
The SystemTypeRegistry is a static class that keeps track of all registered
systems. this allows us to deduct the type of a system from its id and use the
id to create a new system of the same type.
*/
class SystemTypeRegistry {
   /* private:
      static bool isInitialized;
      static void init();

   public:
      // A mapping from stable type name -> runtime id
      static Map<String, u32> name_to_id;
      // A mapping from runtime id -> constructor
      static Map<u32, Function<UniquePtr<System>(Ptr<System>)>>
   id_to_constructor; */
   // only way to initialize the id_to_constructor map on windows
  public:
   static Map<String, u32>& get_name_to_id() {
      static Map<String, u32> name_to_id = {};
      return name_to_id;
   }
   // only way to initialize the id_to_constructor map on windows
   static Map<u32, Function<UniquePtr<System>(Ptr<System>)>>&
   get_id_to_constructor() {
      static Map<u32, Function<UniquePtr<System>(Ptr<System>)>>
          id_to_constructor = {};
      return id_to_constructor;
   }
   // register a new system type
   template <SystemType SystemT>
   inline static u32 register_system_type(const String& typeName, u32 typeId) {
      // Check for initialization to avoid static initialization order fiasco
      /* if (!isInitialized)
      {
         init();
      } */
      // Only register if not already registered.
      if (!get_id_to_constructor().contains(typeId)) {
         get_name_to_id().try_emplace(typeName, typeId);
         get_id_to_constructor().try_emplace(typeId, [](Ptr<System> system) {
            // check for nullptr
            if (system)
               return make_unique<SystemT>(*dynamic_cast<SystemT*>(system));
            else
               return make_unique<SystemT>();
         });
      }
      return typeId;
   }
};

// assings each system type an id, will always return the same id for the same
// type and a different id for different types
template <SystemType SystemT>
constexpr u32 system_type_id() {
   static const u32 id = get_subtype_id<System>();
   return id;
}

// register a new system type, each system type must be registered with this
// macro!
#define REGISTER_SYSTEM_TYPE(type)                                       \
   static u32 id##type = SystemTypeRegistry::register_system_type<type>( \
       String(#type), system_type_id<type>())

}  // namespace Senpai