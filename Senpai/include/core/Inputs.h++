#pragma once
#include "./Base.h++"
#include "./Event.h++"
#include "./Keys.h++"
#include "./container/Vec2.h++"

namespace Senpai {
// Inputs Class that allows easy Reading of the inputs
class Inputs {
 private:
   struct MouseState {
      // 7 buttons but we take 8 to have a round number
      Bitset<8> buttons;
      Vec2<f32> position, wheel;
   };
   // Keyboard State, will be reloadet by the dispatch_events function
   static Ptr<const bool> keyboard;
   // Mouse State, will be reloadet by the dispatch_events function
   static MouseState mouse;
   // Event Dispatcher that handles all the events
   static EventDispatcher dispatcher;

 public:
   // should be called by the App
   static void init();
   // returns true if the key is pressed
   static bool get_key(Key key);
   // dispatches all events and get the new state
   static bool dispatch_events();
   // post an event to the dispatcher
   template <EventType EventT, typename... Args>
   static inline void post(Args &&...args) {
      dispatcher.post<EventT>(forward<Args>(args)...);
   }
   // add a callback to the dispatcher
   template <EventType EventT>
   static inline void add_callback(Function<bool(EventT const &)> &&callback) {
      dispatcher.add_callback<EventT>(move(callback));
   }
   // mousefunctions
   static Vec2<f32> get_mouse_position();
   static Vec2<f32> get_mouse_wheel();
   static bool get_mouse_button(uint button);
   static void clear_all_callbacks();

};
} // namespace Senpai
