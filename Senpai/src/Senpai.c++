#include <sdl/SDLBackend.h++>
#include <SDL3/SDL.h>

namespace Senpai
{

   bool App::run(){
      // initialize SDL
      SDL::init(title, width, height);

      // create a scene
      Scene scene;
      Systems::TestSystem testSystem;
      Entity testEntity;
      testEntity.add_component(Components::Transform{});
      scene.register_system(move(testSystem));
      scene.add_entity(move(testEntity));

      // init inputs
      Inputs inputs;
      bool isRunning = true;
      auto onQuit = [&](Event::Quit const& event){
         cout << "Quit Event Received!" << endl;
         isRunning = false;
         return true;
      };
      auto onMouseDown = [&](Event::MouseDown const& event){
         cout << "Mouse Down Event Received!" << endl;
         cout << "Mouse Position: " << inputs.get_mouse_position() << endl;
         return true;
      };
      inputs.get_dispatcher().add_callback<Event::Quit>(onQuit);
      inputs.get_dispatcher().add_callback<Event::MouseDown>(onMouseDown);
      bool is_a_pressed = false;
      // main loop
      TimePoint next, last;
      f32 δt = 0;
      const f32 frameTime = 1.0f / this->settings.fpsTarget;
      last = next = get_TimePoint();
      while (isRunning) {
         last = get_TimePoint();
         inputs.dispatch_events();
         if(inputs[SDL_SCANCODE_A] != is_a_pressed){
            is_a_pressed = !is_a_pressed;
            debug_log("key 'A' was pressed!");
         }
         inputs.get_dispatcher().dispatch();
         Renderer::clear();
         scene.update(δt);
         Renderer::present();
         last = next;
         next = get_TimePoint();
         δt = get_time_diff(last,next);
         // handle fps Target
         if(settings.fpsTarget > 0 && δt < frameTime){
            sleep(frameTime - δt);
            next = get_TimePoint();
            δt = get_time_diff(last,next);
         }
      }

      SDL::quit();
      return true;
   }
} // namespace Senpai