#include <SDL3/SDL.h>

#include <Senpai>
#include <sdl/SDLBackend.h++>

namespace Senpai {

bool App::run() {
   // initialize SDL
   SDL::init(title, width, height);
   // init inputs
   Inputs::init();
   Window::resize();
   bool isRunning = true;
   Scene *scene = nullptr;
   // mouse position
   auto onMouseDown = [&](Events::MouseDown const &event) -> bool {
      debug_log("Mouse Position: " << Inputs::get_mouse_position());
      return true;
   };
   // quit the game
   auto onQuit = [&](Events::Quit const &event) -> bool {
      debug_log("Quit Event Received!");
      isRunning = false;
      return true;
   };
   // pause the game
   auto onPause = [&](Events::KeyDown const &event) -> bool {
      if (event.key == Key::ESCAPE) {
         scene->isPaused = !scene->isPaused;
         if (scene->isPaused) {
            scene->on_pause();
         } else {
            scene->on_resume();
         }
      }
      return true;
   };
   // handle window resize
   auto onResize = [&](Events::WindowResize const &event) -> bool {
      Window::resize();
      return true;
   };
   // add the callbacks
   Inputs::add_callback<Events::Quit>(onQuit);
   Inputs::add_callback<Events::KeyDown>(onPause);
   Inputs::add_callback<Events::MouseDown>(onMouseDown);
   Inputs::add_callback<Events::WindowResize>(onResize);
   // create a scene
   Scene loadingScene = {};
   Scene currentScene = {};
   scene = &loadingScene;
   if (sceneLoaders.size() == 0) {
      throw SenpaiException("No scene loader set");
   }
   // load the loading scene
   sceneLoaders[0](scene);
   // set the next scene to load
   loadingScene.nextScene = 1;
   // init the loading scene
   loadingScene.start();
   // set up thread for loading the scene
   Thread loadThread;

   // main loop
   TimePoint next, last;
   f32 dt = 0;
   const f32 frameTime = 1.0f / this->settings.fpsTarget;
   last = next = get_TimePoint();
   //

   // als long as the game is running
   while (isRunning) {
      // get the timepoint
      last = get_TimePoint();
      // dispatch events
      inputs.dispatch_events();
      // clear the Renderer
      Renderer::clear();
      // update the scene
      scene->update(dt);
      // show the new frame
      Renderer::present();
      // check if the next scene should be loaded
      if (scene->nextScene >= 0 && !loadThread.is_running()) {
         if (sceneLoaders.size() > scene->nextScene) {
            // load a new scene in a new thread
            uint nextScene = scene->nextScene;
            currentScene.clear();
            scene = &loadingScene;
            scene->nextScene = -1;
            loadThread.execute(sceneLoaders[nextScene], &currentScene);
         } else {
            debug_log("Scene index out of bounds");
            scene->nextScene = -1;
         }
         // check if the scene is loaded
      } else {
         // if the scene is loaded
         if (loadThread.is_running() && loadThread.is_finished()) {
            debug_log("is joining thread") if (loadThread.join()) {
               debug_log("Scene loaded");
               scene = &currentScene;
               scene->start();
            }
            else {
               debug_warning("Scene failed to load");
               scene = &loadingScene;
               currentScene.clear();
               scene->nextScene = 1;
            }
         }
      }
      // update dt
      last = next;
      next = get_TimePoint();
      dt = get_time_diff(last, next);
      // handle fps Target
      if (settings.fpsTarget > 0 && dt < frameTime) {
         sleep(frameTime - dt - ε);
         next = get_TimePoint();
         dt = get_time_diff(last, next);
      }
   }  // end main loop

   loadingScene.clear();
   currentScene.clear();
   // quit SDL
   SDL::quit();
   Inputs::clear_all_callbacks();
   return true;
}
}  // namespace Senpai