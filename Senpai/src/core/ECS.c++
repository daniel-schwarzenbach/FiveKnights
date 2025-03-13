#include <core/ECS.h++>
#include <assets/Assets.h++>
#include <components/Components.h++>

// definitions of the ComponentTypeRegistry
namespace Senpai {
   bool ComponentTypeRegistry::isInitialized = false;
   Map<String, u32> ComponentTypeRegistry::name_to_id;
   Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>> ComponentTypeRegistry::id_to_constructor;
   void ComponentTypeRegistry::init() {
      ComponentTypeRegistry::isInitialized = true;
      name_to_id = Map<String, u32>{};
      id_to_constructor = Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>{};
   }
   
   bool SystemTypeRegistry::isInitialized = false;
   Map<String, u32> SystemTypeRegistry::name_to_id;
   Map<u32, Function<UniquePtr<System>(Ptr<System>)>> SystemTypeRegistry::id_to_constructor;
   void SystemTypeRegistry::init() {
      SystemTypeRegistry::isInitialized = true;
      name_to_id = Map<String, u32>{};
      id_to_constructor = Map<u32, Function<UniquePtr<System>(Ptr<System>)>>{};
   }


} // namespace Senpai

namespace Senpai {

   void ECRegistry::clear() {
      this->componentId_to_entityPtrs.clear();
      this->entities.clear();
   }

   Ptr<Entity> ECRegistry::add_entity(Entity&& entity, bool isAlive) {
      u32 entityId;
      if(freeIds.empty()){
         entities.push_back(entity);
         entityId = entities.size() - 1;
      } else {
         entityId = *freeIds.begin();
         freeIds.erase(entityId);
         entities[entityId] = entity;
      }
      // if entity is alive add all components to the registry
      if(isAlive)
         revive(&entities[entityId]);
      return &entities[entityId];
   }

   void ECRegistry::remove_entity(Ptr<Entity> entityPtr) {
      destroy(entityPtr);
      entities.remove_ptr(entityPtr);
   }

   void ECRegistry::revive(Ptr<Entity> entityPtr) {
      entityPtr->revive(this);
   }

   void ECRegistry::destroy(Ptr<Entity> entityPtr) {
      entityPtr->destroy();
   }

   void ECRegistry::register_component(u32 componentId, Ptr<Entity> entityPtr) {
      if(componentId >= componentId_to_entityPtrs.size()){
         componentId_to_entityPtrs.resize(componentId + 1);
      }
      componentId_to_entityPtrs[componentId].push_back(entityPtr);
   }

   void ECRegistry::unregister_component(u32 componentId, Ptr<Entity> entityPtr) {
      if(componentId >= componentId_to_entityPtrs.size()){
         return;
      }
      componentId_to_entityPtrs[componentId].remove_element(entityPtr);
   }

   Entity::Entity() : components{}, componentsMap{}, parentPtr{nullptr}, registryPtr{nullptr} {}

   Entity::Entity(Entity const& other) : components{}, registryPtr{other.registryPtr}, parentPtr{other.parentPtr}, componentsMap{} {
      // copy the components
      for(auto& componentPtr : other.components) {
         auto comp = ComponentTypeRegistry::id_to_constructor[componentPtr->id](componentPtr.get());
         u32 id = comp->id;
         components.push_back(move(comp));
         componentsMap.try_emplace(id,components.size()-1);
      }
   }

   Entity& Entity::operator=(Entity const& other) {
      this->components.clear();
      this->componentsMap.clear();
      // copy the components
      for(auto& componentPtr : other.components) {
         auto comp = ComponentTypeRegistry::id_to_constructor[componentPtr->id](componentPtr.get());
         u32 id = comp->id;
         components.push_back(move(comp));
         componentsMap.try_emplace(id,components.size()-1);
      }
      return *this;
   }
   Frame<f32> Entity::get_transform() {
      if(!(this->has_component<Components::Transform>())){
         return Frame<f32>{};
      }
      const auto& transform = this->get_component<Components::Transform>();
      if(parentPtr != nullptr && parentPtr->has_component<Components::Transform>()) {
         return transform.frame.apply_parrent_frame(parentPtr->get_transform());
      }
      return transform.frame;
   }


   void Entity::revive(Ptr<ECRegistry> ecRegistryPtr) {
      this->registryPtr = registryPtr;
      for(auto& [cId, index] : componentsMap) {
         ecRegistryPtr->register_component(cId, this);
      }
   }



   bool Entity::is_alive() const{
      return registryPtr!=nullptr;
   }

   void Entity::destroy(){
      if(registryPtr != nullptr){
         // for(auto& [componentId,componentPtr] : components) {
         //    registryPtr->unregister_component(componentId, this);
         // }
      }
      registryPtr = nullptr;
   }
   

   void System::prepare(Ptr<Scene> scenePtr) {
      this->scenePtr = scenePtr;
   }
} // namespace Senpai::ECS

namespace Senpai {
   void Scene::add_entity(Entity&& entity, bool isAlive) {
      this->ecRegistry.add_entity(move(entity), isAlive);
   }

   void Scene::update(f32 δt) {
      if(this->isPaused) {
         for (auto& system : systems) {
            if(!system->canBePaused){
               system->update(δt);
            }
         }
      } else {
         for(auto& system : systems) {
            system->update(δt);
         }
      }
   }

   void Scene::start() {
      for(auto& system : systems) {
         system->start();
      }
   }

}  // namespace Senpai