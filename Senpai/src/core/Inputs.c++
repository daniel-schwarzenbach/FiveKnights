#include <SDL3/SDL.h>
#include <core/Inputs.h++>
#include <events/Events.h++>

namespace Senpai
{

   Inputs::Inputs(){
      keyboard = SDL_GetKeyboardState(NULL);
   }

   bool Inputs::operator[](uint key){
      return keyboard[key];
   }

   bool Inputs::dispatch_events(){
      static SDL_Event event;
      bool dispatched = false;
      while (SDL_PollEvent(&event)) {
         dispatched = true;
         switch (event.type) {
            case SDL_EVENT_QUIT:
               dispatcher.post<Event::Quit>();
               break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
               dispatcher.post<Event::MouseDown>(event.button.button);
               mouse.buttons.set(event.button.button);
               break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
               dispatcher.post<Event::MouseUp>(event.button.button);
               mouse.buttons.reset(event.button.button);
               break;
            case SDL_EVENT_MOUSE_WHEEL:
               dispatcher.post<Event::MouseWheel>();
               mouse.wheel = {event.button.x, event.button.y};
               break;
            case SDL_EVENT_MOUSE_MOTION:
               dispatcher.post<Event::MouseMove>(event.motion.x, event.motion.y);
               mouse.position = {event.motion.x, event.motion.y};
               break;
            case SDL_EVENT_KEY_DOWN:
               dispatcher.post<Event::KeyDown>(event.key.key);
               keyboard = SDL_GetKeyboardState(NULL);
               break;
            case SDL_EVENT_KEY_UP:
               dispatcher.post<Event::KeyUp>(event.key.key);
               keyboard = SDL_GetKeyboardState(NULL);
               break;
         }
      }
      dispatcher.dispatch();
      return dispatched;
   }

   EventDispatcher& Inputs::get_dispatcher(){
      return dispatcher;
   }

   Vec2<f32> Inputs::get_mouse_position(){
      return mouse.position;
   }

   Vec2<f32> Inputs::get_mouse_wheel(){
      return mouse.wheel;
   }

   bool Inputs::get_mouse_button(uint button){
      return mouse.buttons[button];
   }

   

} // namespace Senpai
