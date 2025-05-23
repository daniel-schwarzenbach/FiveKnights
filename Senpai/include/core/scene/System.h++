#pragma once
#include "./Entity.h++"

namespace Senpai {
struct System {
  protected:
   Ptr<Scene> scenePtr = nullptr;
   // end protected
  public:
   // execution hints

   // if the system should be updated on pause
   inline virtual  bool will_be_paused() { return false; }
   // if the system can be updated in parallel
   inline virtual  bool is_parralel() { return false; }

   inline virtual ~System() = default;
   // assings a system to the correct registries
   void prepare(Ptr<Scene> scenePtr);
   // get all entities with a certain component
   template <ComponentType ComponentT>
   Vector<Ptr<Entity>> view();
   // update the system, called every frame
   inline virtual void update(f32 dt) {};
   // start the system, called once
   inline virtual void start() {};
   // input
   inline virtual void in(IS &) {};
   // output
   inline virtual void out(OS &) {};
   // input operator
   inline friend IS &operator>>(IS &is, System &system) {
      system.in(is);
      return is;
   }
   // output operator
   inline friend OS &operator<<(OS &os, System &system) {
      system.out(os);
      return os;
   }
   // end public
};
}  // namespace Senpai