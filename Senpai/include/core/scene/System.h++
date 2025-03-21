#pragma once
#include "./Entity.h++"

namespace Senpai {
   struct System
   {
   protected:
      Ptr<Scene> scenePtr = nullptr;
      // end protected
   public:
      bool canBePaused = false;

      inline virtual ~System() = default;
      // assings a system to the correct registries
      void prepare(Ptr<Scene> scenePtr);
      // get all entities with a certain component
      template <ComponentType ComponentT>
      Vector<Ptr<Entity>> view();
      // update the system, called every frame
      inline virtual void update(f32 δt) {};
      // start the system, called once
      inline virtual void start() {};
      // input
      inline virtual void in(IS &){};
      // output
      inline virtual void out(OS &) {};
      // input operator
      inline friend IS &operator>>(IS &is, System &system)
      {
         system.in(is);
         return is;
      }
      // output operator
      inline friend OS &operator<<(OS &os, System &system)
      {
         system.out(os);
         return os;
      }
      // end public
   };
}