#include <core/Inputs.h++>
#include <events/Events.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai {

Ptr<const bool> Inputs::keyboard = nullptr;
Inputs::MouseState Inputs::mouse = {};
EventDispatcher Inputs::dispatcher = {};

void Inputs::init() { Inputs::keyboard = SDL_GetKeyboardState(NULL); }

bool Inputs::get_key(Key key) { return keyboard[key]; }

bool Inputs::dispatch_events() {
   Vec2<int> wSize = Window::get_size();
   static SDL_Event event;
   bool dispatched = false;
   while (SDL_PollEvent(&event)) {
      dispatched = true;
      switch (event.type) {
         case SDL_EVENT_QUIT:
            dispatcher.post<Events::Quit>();
            break;
         case SDL_EVENT_MOUSE_BUTTON_DOWN:
            dispatcher.post<Events::MouseDown>(event.button.button);
            mouse.buttons.set(event.button.button);
            break;
         case SDL_EVENT_MOUSE_BUTTON_UP:
            dispatcher.post<Events::MouseUp>(event.button.button);
            mouse.buttons.reset(event.button.button);
            break;
         case SDL_EVENT_MOUSE_WHEEL:
            dispatcher.post<Events::MouseWheel>();
            mouse.wheel = {event.button.x, event.button.y};
            break;
         case SDL_EVENT_MOUSE_MOTION:
            dispatcher.post<Events::MouseMove>(event.motion.x, event.motion.y);
            mouse.position = {-wSize.x / 2.0f + event.motion.x,
                              wSize.y / 2.0f - event.motion.y};
            break;
         case SDL_EVENT_KEY_DOWN:
            dispatcher.post<Events::KeyDown>(event.key.key);
            keyboard = SDL_GetKeyboardState(NULL);
            break;
         case SDL_EVENT_KEY_UP:
            dispatcher.post<Events::KeyUp>(event.key.key);
            keyboard = SDL_GetKeyboardState(NULL);
            break;
         case SDL_EVENT_WINDOW_RESIZED:
            dispatcher.post<Events::WindowResize>();
            break;
      }
   }
   dispatcher.dispatch();
   return dispatched;
}

// EventDispatcher& Inputs::get_dispatcher(){
//    return dispatcher;
// }

Vec2<f32> Inputs::get_mouse_position() { return mouse.position; }

Vec2<f32> Inputs::get_mouse_wheel() { return mouse.wheel; }

bool Inputs::get_mouse_button(uint button) { return mouse.buttons[button]; }

void Inputs::clear_all_callbacks() { dispatcher.clear(); }

}  // namespace Senpai
