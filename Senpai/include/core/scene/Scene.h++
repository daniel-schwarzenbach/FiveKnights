#pragma once
#include "./System.h++"

namespace Senpai {
   struct Scene {
      // the id of the next scene to load, -1 if no scene should be loaded
      int nextScene = -1;
      // value that determines if the scene is paused
      bool isPaused = false;
      // contains all Systems
      Vector<UniquePtr<System>> systems = {};
      // contains all Entities with their Component
      ECRegistry ecRegistry = {};
      // contains all Assets
      AssetRegistry assetRegistry = {};
      // ptr to the App.input
      Ptr<Inputs> inputPtr = nullptr;

      Entity& add_entity(bool isAlive = true);
      // move an entity into the scene
      Entity& add_entity_copy(Entity const& entity, bool isAlive = true);

      // move an asset into the scene
      template<AssetType AssetT, typename... Args>
      AssetT& add_asset(Args&&... args) {
         return assetRegistry.add_asset<AssetT>(std::forward<Args>(args)...);
      }

      // update all systems
      void update(f32 Δt);

      // start all systems
      void start();

      // clear the scene
      void clear();

      // invoke scripts on pause
      void on_pause();

      // invoke scripts on resume
      void on_resume();

      // ads a system to the scene
      template <SystemType SystemT, typename... Args>
      SystemT& add_system(Args&&... args) {
         systems.push_back(make_unique<SystemT>(std::forward<Args>(args)...));
         systems.back()->prepare(this);
         return *dynamic_cast<Ptr<SystemT>>(systems.back().get());
      }
   };
}