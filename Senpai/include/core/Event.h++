#pragma once
#include "./Base.h++"

namespace Senpai {
   // Event Base Class
   struct EventInstance {
      inline virtual ~EventInstance() = default;
   };

   template <typename T>
   concept EventType = std::is_base_of_v<EventInstance, T>;
   
   struct Callback {
      inline virtual ~Callback() = default;
      virtual bool run(Ptr<EventInstance>) = 0;
   };

   template <EventType EventT>
   struct EventCallback : Callback {
      Function<bool(EventT const&)> callbackFn;
      inline EventCallback(Function<bool(EventT const&)>& clbkFn) : callbackFn(clbkFn) {}
      inline bool run(Ptr<EventInstance> eventPtr) {
         EventT event = static_cast<EventT const&>(*eventPtr);
         return callbackFn(event);
      }
   };

   using CallbackList = Array<SharedPtr<Callback>>;

   template<EventType EventT>
   constexpr u32 event_id() {
      static const u32 id = get_subtype_id<EventInstance>();
      return id;
   }

   struct EventDispatcher {
      Array<UniquePtr<EventInstance>> events;
      Array<u32> dispatchList;
      Array<CallbackList> callbacks;
      

      template <EventType EventT, typename... Args>
      inline void post(Args&&... args){
         if(events.size() <= event_id<EventT>()){
            events.resize(event_id<EventT>() + 1);
            callbacks.resize(event_id<EventT>() + 1);
         }
         events[event_id<EventT>()] = make_unique<EventT>(std::forward<Args>(args)...);
         dispatchList.push_back(event_id<EventT>());
      }

      template <EventType EventT>
      inline void add_callback(Function<bool(EventT const&)>&& callbackFn){
         if(callbacks.size() <= event_id<EventT>()){
            events.resize(event_id<EventT>() + 1);
            callbacks.resize(event_id<EventT>() + 1);
         }
         auto callback = make_shared<EventCallback<EventT>>(callbackFn);
         callbacks[event_id<EventT>()].push_back(callback);
      }

      inline void dispatch(){
         for(u32& eventId : dispatchList){
            if(events[eventId] != nullptr){
               for(SharedPtr<Callback>& callback : callbacks[eventId]){
                  if(callback && callback->run(events[eventId].get())){
                     break;
                  }
               }
            }
            events[eventId].reset();
         }
         dispatchList.clear();
      }
   };
};