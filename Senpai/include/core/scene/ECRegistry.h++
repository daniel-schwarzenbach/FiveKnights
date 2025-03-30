#pragma once
#include "./Component.h++"
#include "./ECSBase.h++"

namespace Senpai {
// Handles all Entities and Components
struct ECRegistry {
  private:
   // Keep track of all components and there enabled entities
   Vector<Vector<Ptr<Entity>>> componentId_to_entityPtrs;
   // Contains all entities, dead or alive, List is refrence safe!!!
   List<Entity> entities;

  public:
   // a sorted vector of all components that need to be rendered by UI
   Vector<Ptr<UIComponent>> uiComponents;
   // a sorted vector of all components that need to be rendered by a camera
   Vector<Ptr<RenderComponent>> componentsToRender;
   // clear the entire registry
   void clear();
   // add an new entity to the registry
   Entity& add_entity(bool isAlive = true);
   // add an preexisting entity to the registry
   Entity& add_entity_copy(Entity const& entity, bool isAlive = true);
   // remove an entity from the registry
   void remove_entity(Ptr<Entity> entity);

   template <ComponentType ComponentT>
   Vector<Ptr<Entity>>& view() {
      u32 componentId = component_type_id<ComponentT>();
      if (this->componentId_to_entityPtrs.size() <= componentId) {
         componentId_to_entityPtrs.resize(componentId + 1);
      }
      return componentId_to_entityPtrs[componentId];
   }

   void register_component(Ptr<Component> componentPtr, Ptr<Entity> entityPtr);

   void unregister_component(Ptr<Component> componentPtr,
                             Ptr<Entity> entityPtr);
};
}  // namespace Senpai