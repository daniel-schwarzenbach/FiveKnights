#include <SDL3/SDL.h>
#include <sdl/SDLBackend.h++>
#include <thread>
#include <future>

namespace Senpai {

bool App::run() {
   // initialize SDL
   SDL::init(title, width, height);
   // init inputs
   Inputs::init();
   bool isRunning = true;
   Scene* scene = nullptr;
   // mouse position
   auto onMouseDown = [&](Events::MouseDown const &event) -> bool {
      cout << "Mouse Position: " << Inputs::get_mouse_position() << endl;
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
   Inputs::add_callback<Events::Quit>(onQuit);
   Inputs::add_callback<Events::KeyDown>(onPause);
   Inputs::add_callback<Events::MouseDown>(onMouseDown);
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
   // set up thread pointer for loading the scene
   UniquePtr<std::future<void>> loadPtr = nullptr;
   
   // main loop
   TimePoint next, last;
   f32 Δt = 0;
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
      scene->update(Δt);
      // show the new frame
      Renderer::present();
      // check if the next scene should be loaded
      if (scene->nextScene >= 0 && loadPtr == nullptr) {
         if (sceneLoaders.size() > scene->nextScene) {
            // load a new scene in a new thread
            currentScene.clear();
            loadPtr = make_unique<std::future<void>>(
               std::async(std::launch::async, 
                          sceneLoaders[scene->nextScene],
                          &currentScene));
            scene = &loadingScene;
            scene->nextScene = -1;
         } else {
            debug_log("Scene index out of bounds");
            scene->nextScene = -1;
         }
         // check if the scene is loaded
      } else {
         // if the scene is loaded
         if (loadPtr != nullptr && loadPtr->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            loadPtr->get();
            loadPtr = nullptr;
            scene = &currentScene;
            scene->inputPtr = &inputs;
            scene->start();
         }
      }
      // update Δt
      last = next;
      next = get_TimePoint();
      Δt = get_time_diff(last, next);
      // handle fps Target
      if (settings.fpsTarget > 0 && Δt < frameTime) {
         sleep(frameTime - Δt - ε);
         next = get_TimePoint();
         Δt = get_time_diff(last, next);
      }
   }

   loadingScene.clear();
   currentScene.clear();
   // quit SDL
   SDL::quit();
   Inputs::clear_all_callbacks();
   return true;
}
} // namespace Senpai