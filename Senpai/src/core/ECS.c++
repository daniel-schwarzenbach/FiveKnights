#include <assets/Assets.h++>
#include <components/Components.h++>
#include <core/scene/ECS.h++>

// definitions of the ComponentTypeRegistry
namespace Senpai {
bool ComponentTypeRegistry::isInitialized = false;
Map<String, u32> ComponentTypeRegistry::name_to_id;
Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>
    ComponentTypeRegistry::id_to_constructor;
void ComponentTypeRegistry::init() {
   ComponentTypeRegistry::isInitialized = true;
   name_to_id = Map<String, u32>{};
   id_to_constructor =
       Map<u32, Function<UniquePtr<Component>(Ptr<Component>)>>{};
}

bool SystemTypeRegistry::isInitialized = false;
Map<String, u32> SystemTypeRegistry::name_to_id;
Map<u32, Function<UniquePtr<System>(Ptr<System>)>>
    SystemTypeRegistry::id_to_constructor;
void SystemTypeRegistry::init() {
   SystemTypeRegistry::isInitialized = true;
   name_to_id = Map<String, u32>{};
   id_to_constructor = Map<u32, Function<UniquePtr<System>(Ptr<System>)>>{};
}

bool RenderComponent::is_sub_type(Ptr<Component> componentPtr) {
   return dynamic_cast<RenderComponent *>(componentPtr) != nullptr;
}

void RenderComponent::flip_vertical() { this->flip ^= Flip::Vertical; }

void RenderComponent::flip_horizontal() { this->flip ^= Flip::Horizontal; }

bool UIComponent::is_sub_type(Ptr<Component> componentPtr) {
   return dynamic_cast<UIComponent *>(componentPtr) != nullptr;
}

void UIComponent::flip_vertical() { this->flip ^= Flip::Vertical; }

void UIComponent::flip_horizontal() { this->flip ^= Flip::Horizontal; }

void ECRegistry::clear() {
   this->componentId_to_entityPtrs.clear();
   this->entities.clear();
}

Entity& ECRegistry::add_entity(bool isAlive) {
   // make a new entity
   entities.push_back(Entity());
   Ptr<Entity> entityPtr = &entities.back();
   entityPtr->set_ecRegistry(this);
   if (isAlive) {
      entityPtr->enable();
   } else {
      entityPtr->disable();
   }
   return *entityPtr;
}

Entity& ECRegistry::add_entity_copy(Entity const& entity, bool isAlive) {
   entities.push_back(entity);
   Ptr<Entity> entityPtr = &entities.back();
   entityPtr->set_ecRegistry(this);
   if (isAlive) {
      entityPtr->enable();
   } else {
      entityPtr->disable();
   }
   return *entityPtr;
}

void ECRegistry::remove_entity(Ptr<Entity> entityPtr) {
   entityPtr->disable();
   // remove any entity at that pointer
   entities.remove_if([entityPtr](Entity &entity) {
      return &entity == entityPtr;
   });
}

void ECRegistry::register_component(Ptr<Component> componentPtr,
                                    Ptr<Entity> entityPtr) {
   if (componentPtr->id >= componentId_to_entityPtrs.size()) {
      componentId_to_entityPtrs.resize(componentPtr->id + 1);
   }
   componentId_to_entityPtrs[componentPtr->id].push_back(entityPtr);
   if (RenderComponent::is_sub_type(componentPtr)) {
      componentsToRender.insert_sorted(
          dynamic_cast<Ptr<RenderComponent>>(componentPtr));
   } else if (UIComponent::is_sub_type(componentPtr)) {
      uiComponents.insert_sorted(dynamic_cast<Ptr<UIComponent>>(componentPtr));
   }
}

void ECRegistry::unregister_component(Ptr<Component> componentPtr,
                                      Ptr<Entity> entityPtr) {
   if (componentPtr->id >= componentId_to_entityPtrs.size()) {
      return;
   }
   auto &entities = componentId_to_entityPtrs[componentPtr->id];
   entities.remove_element(entityPtr);
   if (RenderComponent::is_sub_type(componentPtr)) {
      componentsToRender.remove_element(
          dynamic_cast<Ptr<RenderComponent>>(componentPtr));
   } else if (UIComponent::is_sub_type(componentPtr)) {
      uiComponents.remove_element(dynamic_cast<Ptr<UIComponent>>(componentPtr));
   }
}

Entity::Entity()
    : components{}, componentsMap{}, parentPtr{nullptr},
      ecRegistryPtr{nullptr} {}

Entity::Entity(Entity const &other)
    : components{}, ecRegistryPtr{other.ecRegistryPtr},
      parentPtr{other.parentPtr}, componentsMap{} {
   // copy the components
   for (auto &componentPtr : other.components) {
      auto comp = ComponentTypeRegistry::id_to_constructor[componentPtr->id](
          componentPtr.get());
      u32 id = comp->id;
      components.push_back(move(comp));
      componentsMap.try_emplace(id, components.size() - 1);
   }
}

Entity &Entity::operator=(Entity const &other) {
   this->components.clear();
   this->componentsMap.clear();
   // copy the components
   for (auto &componentPtr : other.components) {
      auto comp = ComponentTypeRegistry::id_to_constructor[componentPtr->id](
          componentPtr.get());
      u32 id = comp->id;
      components.push_back(move(comp));
      componentsMap.try_emplace(id, components.size() - 1);
   }
   return *this;
}

void Entity::set_ecRegistry(Ptr<ECRegistry> ecRegistryPtr) {
   this->ecRegistryPtr = ecRegistryPtr;
   // set the entityPtr for all components
   for (auto &component : components) {
      component->entityPtr = this;
   }
}

Frame<f32> Entity::get_transform() {
   if (!(this->has_component<Components::Transform>())) {
      return Frame<f32>{};
   }
   const auto &transform = this->get_component<Components::Transform>();
   if (parentPtr != nullptr &&
       parentPtr->has_component<Components::Transform>()) {
      return transform.frame.apply_parrent_frame(parentPtr->get_transform());
   }
   return transform.frame;
}

void Entity::enable() {
   if (!this->ecRegistryPtr) {
      debug_log("Entity has no ecRegistry is not enabled");
      return;
   }
   if(enabled) return;
   for (auto &component : components) {
      ecRegistryPtr->register_component(component.get(), this);
   }
   enabled = true;
}

bool Entity::is_enabled() const { return enabled; }

void Entity::disable() {
   if (!enabled) {
      for (auto &component : components) {
         ecRegistryPtr->unregister_component(component.get(), this);
      }
   }
   enabled = false;
}

void System::prepare(Ptr<Scene> scenePtr) { this->scenePtr = scenePtr; }
} // namespace Senpai

namespace Senpai {
Entity& Scene::add_entity(bool isAlive) {
   return this->ecRegistry.add_entity(isAlive);
}

Entity& Scene::add_entity_copy(Entity const& entity, bool isAlive) {
   return this->ecRegistry.add_entity_copy(entity, isAlive);
}

void Scene::update(f32 δt) {
   if (this->isPaused) {
      for (auto &system : systems) {
         if (!system->canBePaused) {
            system->update(δt);
         }
      }
   } else {
      for (auto &system : systems) {
         system->update(δt);
      }
   }
}

void Scene::start() {
   for (auto &system : systems) {
      system->start();
   }
}

void Scene::on_pause() {
   for (auto& entity : ecRegistry.view<Components::ScriptsHolder>()) {
      auto& scriptHolder =  entity->get_component<Components::ScriptsHolder>();
      for (auto &script : scriptHolder.scripts) {
         script->on_resume();
      }
   }
}

void Scene::on_resume() {
   for (auto& entity : ecRegistry.view<Components::ScriptsHolder>()) {
      auto& scriptHolder =  entity->get_component<Components::ScriptsHolder>();
      for (auto &script : scriptHolder.scripts) {
         script->on_resume();
      }
   }
}

void Scene::clear() {
   this->ecRegistry.clear();
   this->assetRegistry.clear();
   this->systems.clear();
   isPaused = false;
   nextScene = -1;
}

} // namespace Senpai