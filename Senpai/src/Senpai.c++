#include <Senpai>
#include <sdl/SDLBackend.h++>
#include <SDL3_image/SDL_image.h>

namespace Senpai {

bool App::apply_settings() {
   frameTime = 1.0f / settings.fps;
   if (iconPtr == nullptr && icon != "") {
      GenericPtr iconRaw = IMG_Load(icon.c_str());
      iconPtr = SharedPtr<void>(iconRaw, [](void *iconPtr) {
         debug_log("Icon destroyed");
         SDL_DestroySurface((SDL_Surface *)iconPtr);
         iconPtr = nullptr;
      });
      if (iconPtr == nullptr) {
         debug_warning("Failed to load icon: " << icon);
         icon = "";
      }
   }
   SDL_SetWindowSize((SDL_Window *)Window::get(), settings.width, settings.height);
   if (iconPtr != nullptr) {
      SDL_SetWindowIcon((SDL_Window*)Window::get(), (SDL_Surface*)iconPtr.get());
   }
   SDL_SetWindowFullscreen((SDL_Window*)Window::get(), settings.fullscreen == 1);
   return true;
}

bool App::run() {
   // initialize SDL
   SDL::init(title, settings.width, settings.height);
   apply_settings();
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
   // change the settings
   auto onSettingsChanged = [&](Events::SettingsChanged const &event) -> bool {
      debug_log("Settings Changed");
      auto wSize = Window::get_size();
      this->settings.width = wSize.x;
      this->settings.height = wSize.y;
      if(event.settings.fullscreen > -1) {
         settings.fullscreen = event.settings.fullscreen;
      }
      if(event.settings.fps > -1) {
         settings.fps = event.settings.fps;
      }
      if(event.settings.width > -1) {
         settings.width = event.settings.width;
      }
      if(event.settings.height > -1) {
         settings.height = event.settings.height;
      }
      apply_settings();
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
   Inputs::add_callback<Events::SettingsChanged>(onSettingsChanged);
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
   frameTime = 1.0f / this->settings.fps;
   last = next = get_TimePoint();
   //

   // als long as the game is running
   while (isRunning) {
      // dispatch events
      Inputs::dispatch_events();
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
      } else if (loadThread.is_running() && loadThread.is_finished()) {
         // if the scene is loaded
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
      // update dt
      last = next;
      next = get_TimePoint();
      dt = get_time_diff(last, next);
      // handle fps Target
      if (settings.fps > 0 && dt < frameTime) {
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