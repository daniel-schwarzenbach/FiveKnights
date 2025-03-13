#pragma once
#include "./Base.h++"
#include "./Event.h++"
#include "./container/Vec2.h++"

namespace Senpai
{
   class Inputs{
      private:
         struct MouseState {
            // 7 buttons but we take 8 to have a round number
            Bitset<8> buttons;
            Vec2<f32> position, wheel;
         };
         // Inputs
         Ptr<const bool> keyboard;
         MouseState mouse;
         // Event Dispatcher
         EventDispatcher dispatcher;

      public:
         Inputs();
         //bool is_key(uint key);
         bool operator[](uint key);
         bool dispatch_events();
         EventDispatcher& get_dispatcher();
         // mousefunctions
         Vec2<f32> get_mouse_position();
         Vec2<f32> get_mouse_wheel();
         bool get_mouse_button(uint button);
   };
} // namespace Senpai
