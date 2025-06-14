#pragma once
#include <Senpai>
#include <fstream>
using namespace Senpai;

// global variables
namespace main_menu {
enum class State { MAIN, SETTINGS, HOW_TO_PLAY };
inline static State state = State::MAIN;
inline static App* gamePtr = nullptr;
}  // namespace main_menu

static Ptr<Assets::Audio> clickSound;

struct menuHandlerScript final : public Script {
   Ptr<Assets::Audio> ambient;
   Ptr<Assets::Audio> music;
   Vector<Entity*> mainMenuEntities = {};
   Vector<Entity*> settingsEntities = {};
   Vector<Entity*> howToPlayEntities = {};
   main_menu::State state = main_menu::State::MAIN;

   void on_start() {
      main_menu::state = state = main_menu::State::MAIN;
      ambient->play(0.4f, true);
      music->play(0.4f, false);
      for (auto entity : view<Components::Info>()) {
         if (entity->has_component<Components::Info>() == false) continue;
         auto& info = entity->get_component<Components::Info>();
         if (info.tag == "Main") {
            mainMenuEntities.push_back(entity);
         } else if (info.tag == "Settings") {
            settingsEntities.push_back(entity);
            entity->disable();
         } else if (info.tag == "HowToPlay") {
            howToPlayEntities.push_back(entity);
            entity->disable();
         }
      }
   }

   void on_update(f32 dt) override {
      if (Inputs::get_key(Key::DELETE)) {
         scenePtr->nextScene = 1;
         clickSound->play(1, false);
         sleep(0.3);
         return;
      }
      if (state != main_menu::State::MAIN) {
         if (Inputs::get_key(Key::ESCAPE)) {
            main_menu::state = main_menu::State::MAIN;
            clickSound->play(1, false);
         }
      }
      if (state != main_menu::state) {
         Vector<Entity*>* entities = nullptr;
         switch (state) {
            case main_menu::State::MAIN:
               entities = &mainMenuEntities;
               break;
            case main_menu::State::SETTINGS:
               entities = &settingsEntities;
               break;
            case main_menu::State::HOW_TO_PLAY:
               entities = &howToPlayEntities;
               break;
         }
         for (auto entity : *entities) {
            entity->disable();
         }
         state = main_menu::state;
         switch (state) {
            case main_menu::State::MAIN:
               entities = &mainMenuEntities;
               break;
            case main_menu::State::SETTINGS:
               entities = &settingsEntities;
               break;
            case main_menu::State::HOW_TO_PLAY:
               entities = &howToPlayEntities;
               break;
         }
         for (auto entity : *entities) {
            entity->enable();
         }
      }
   }
};

struct startGameScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         // this will load the game scene
         scenePtr->nextScene = 2;
         clickSound->play(1, false);
         sleep(0.3);
      }
   }
};

// goes to how to play state
struct HowToPlayScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         main_menu::state = main_menu::State::HOW_TO_PLAY;
         clickSound->play(1, false);
      }
   }
};

// goes to settings state
struct settingsScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         main_menu::state = main_menu::State::SETTINGS;
         clickSound->play(1, false);
      }
   }
};

// script for a settings apply button
struct ApplyScript final : public Script {
   AppSettings appSettings;
   void on_start() override {
      appSettings = read_from_file<AppSettings>("./assets/data/app_settings.dat");
   }
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         clickSound->play(1, false);
         toggle = -0.5;
         Inputs::post<Events::SettingsChanged>(appSettings);
         write_to_file<AppSettings>("./assets/data/app_settings.dat", appSettings);
      }
   }
};

// retun to main menu state
struct ReturnScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         main_menu::state = main_menu::State::MAIN;
         clickSound->play(1, false);
      }
   }
};

// setts the game to fullscreen
struct FullScreenScript final : public Script {
   Ptr<AppSettings> settingsPtr;
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
      if (settingsPtr->fullscreen == 1) {
         auto& button = entityPtr->get_component<Components::ButtonUI>();
         button.text = "Fullscreen: On";
      } else {
         auto& button = entityPtr->get_component<Components::ButtonUI>();
         button.text = "Fullscreen: Off";
      }
   }

   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         clickSound->play(1, false);
         if (settingsPtr->fullscreen == 1) {
            settingsPtr->fullscreen = 0;
         } else {
            settingsPtr->fullscreen = 1;
         }
      }
   }
};

// quits the game
struct exitScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if (toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         clickSound->play(1, false);
         sleep(0.3);
         Inputs::post<Events::Quit>();
      }
   }
};



constexpr f32 offset = 200;
Vec2<f32> buttonPos = {0, 100};
Vec2<f32> get_button_pos(int i = 0) {
   auto copy = buttonPos;
   copy.y -= i * offset;
   return copy;
}

// loading function for the menu scene
void set_up_menu_scene(Ptr<Scene> scenePtr) {
   // register all needet systems
   scenePtr->add_system<Systems::UIButtonSystem>();
   scenePtr->add_system<Systems::ScriptRunner>();
   scenePtr->add_system<Systems::CameraRenderSystem>();
   scenePtr->add_system<Systems::UIRenderSystem>();
   // load needet assets
   Assets::Font& font = scenePtr->add_asset<Assets::Font>(
       String("./assets/fonts/The Centurion .ttf"), 100, String("HeroFont"));
   Assets::Font& fontLarge = scenePtr->add_asset<Assets::Font>(
       String("./assets/fonts/The Centurion .ttf"), 130,
       String("HeroFontLarge"));
   Assets::Font& fontsmall = scenePtr->add_asset<Assets::Font>(
       String("./assets/fonts/Griffiths.otf"), 50, String("HeroFontSmall"));
   debug_log("Loading Texture1");
   auto& texture = scenePtr->add_asset<Assets::Texture>(
       "./assets/pics/BackGround.jpg", "MenuBackground");
   debug_log("Loading Texture2");
   auto& howToPlayTexture = scenePtr->add_asset<Assets::Texture>(
       String("./assets/pics/HowToPlay.png"), String("HowToPlayTexture"));
   Assets::Audio& ambient = scenePtr->add_asset<Assets::Audio>(
       String("./assets/audio/AmbientLoop.wav"), String("Ambient"));
   Assets::Audio& music = scenePtr->add_asset<Assets::Audio>(
       String("./assets/audio/Musique.wav"), String("MenuMusic"));
   clickSound = &scenePtr->add_asset<Assets::Audio>(
       String("./assets/audio/Click.mp3"), String("Click"));
   f32 highScore = read_from_file<f32>("./assets/data/highscore.dat");
   debug_log("Adding Entities");
   // camera entity
   Entity& camera = scenePtr->add_entity();
   camera.add_component<Components::Transform>(Vec2<f32>{0, 0},
                                               Vec2<f32>{1.0f, 1.0f});
   camera.add_component<Components::Camera>(Vec2<f32>{0, 0}, 1.0f);
   auto& ms = camera.add_script<menuHandlerScript>();
   ms.music = &music;
   ms.ambient = &ambient;
   // background entity
   Entity& bg = scenePtr->add_entity();
   auto& bg_tr = bg.add_component<Components::Transform>(Vec2<f32>{0, 0},
                                                         Vec2<f32>{0.6f, 0.6f});
   auto& bg_sprite = bg.add_component<Components::Sprite>(&texture);
   bg_sprite.z = -10;

   // create the menu entities
   buttonPos = {0, 150};

   // start game button entity
   Entity& game = scenePtr->add_entity();
   auto& game_tr = game.add_component<Components::Transform>(get_button_pos(0));
   auto& game_button = game.add_component<Components::ButtonUI>(
       &font, String("Start Game"), Color{255, 255, 255, 255},
       Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   game.add_script<startGameScript>();
   game.add_component<Components::Info>("StartGame", "Main");

   // How to play button entity
   Entity& how = scenePtr->add_entity();
   auto& how_tr = how.add_component<Components::Transform>(get_button_pos(1));
   auto& how_button = how.add_component<Components::ButtonUI>(
       &font, String("How to Play?"), Color{255, 255, 255, 255},
       Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   how.add_script<HowToPlayScript>();
   auto& how_info = how.add_component<Components::Info>("HowToPlay", "Main");

   // settings button entity
   Entity& settings = scenePtr->add_entity();
   auto& settings_tr =
       settings.add_component<Components::Transform>(get_button_pos(2));
   auto& settings_button = settings.add_component<Components::ButtonUI>(
       &font, String("Settings"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
       Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   settings.add_script<settingsScript>();
   settings.add_component<Components::Info>("Settings", "Main");

   // Exit button entity
   Entity& exit = scenePtr->add_entity();
   auto& exit_tr = exit.add_component<Components::Transform>(get_button_pos(3));
   auto& exit_button = exit.add_component<Components::ButtonUI>(
       &font, String("Exit"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
       Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   exit.add_script<exitScript>();
   exit.add_component<Components::Info>("Exit", "Main");

   // Highscore entity
   Entity& score = scenePtr->add_entity();
   auto& score_tr =
       score.add_component<Components::Transform>(Vec2<f32>{-650, -500});
   auto& score_btn = score.add_component<Components::ButtonUI>(
       &fontsmall,
       str("longest survival:  ") + str(highScore) + str(" seconds"),
       Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{0, 0, 0, 0},
       Vec2<f32>{0, 0});
   score.add_component<Components::Info>("Highscore", "Main");

   // Title entity
   Entity& title = scenePtr->add_entity();
   auto& title_tr =
       title.add_component<Components::Transform>(Vec2<f32>{0, 350});
   auto& title_btn = title.add_component<Components::ButtonUI>(
       &fontLarge, String("Five Knights against King Fredric"),
       Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{0, 0, 0, 0},
       Vec2<f32>{0, 0});
   title_btn.z = 4;
   title.add_component<Components::Info>("Title", "Main");
   // Title Shdow entity
   Entity& titlesh = scenePtr->add_entity();
   auto& titlesh_tr =
       titlesh.add_component<Components::Transform>(Vec2<f32>{5, 344});
   auto& titlesh_btn = titlesh.add_component<Components::ButtonUI>(
       &fontLarge, String("Five Knights against King Fredric"),
       Color{0, 0, 0, 200}, Color{0, 0, 0, 0}, Color{0, 0, 0, 0},
       Vec2<f32>{0, 0});
   titlesh_btn.z = 3;
   titlesh.add_component<Components::Info>("TitleShadow", "Main");

   //       - Settings -
   buttonPos = {0, 400};

   // return button
   Entity& returnSet = scenePtr->add_entity();
   auto& returnSet_tr =
       returnSet.add_component<Components::Transform>(get_button_pos(0));
   auto& returnSet_button = returnSet.add_component<Components::ButtonUI>(
       &font, String("Return"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
       Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   returnSet.add_script<ReturnScript>();
   returnSet.add_component<Components::Info>("Return", "Settings");

   // Apply button
   Entity& apply = scenePtr->add_entity();
   auto& apply_tr = apply.add_component<Components::Transform>(get_button_pos(1));
   auto& apply_button = apply.add_component<Components::ButtonUI>(
       &font, String("Apply"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
       Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   auto& apply_script = apply.add_script<ApplyScript>();
   apply.add_component<Components::Info>("Apply", "Settings");

   // Fullscreen button
   Entity& toggleFS = scenePtr->add_entity();
   auto& toggleFS_tr =
         toggleFS.add_component<Components::Transform>(get_button_pos(2));
   auto& toggleFS_button = toggleFS.add_component<Components::ButtonUI>(
         &font, String("Fullscreen"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
         Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   auto& toggleFS_script = toggleFS.add_script<FullScreenScript>();
   toggleFS_script.settingsPtr = &apply_script.appSettings;
   toggleFS.add_component<Components::Info>("Fullscreen", "Settings");

   //       - How to Play -
   buttonPos = {0, 400};

   // return button
   Entity& returnHow = scenePtr->add_entity();
   auto& returnHow_tr =
       returnHow.add_component<Components::Transform>(get_button_pos(0));
   auto& returnHow_button = returnHow.add_component<Components::ButtonUI>(
       &font, String("Return"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0},
       Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   returnHow.add_script<ReturnScript>();
   returnHow.add_component<Components::Info>("Return", "HowToPlay");

   // background entity
   Entity& bgHowTo = scenePtr->add_entity();
   auto& tr_bgHowTo = bgHowTo.add_component<Components::Transform>();
   auto& sp_bgHowTo =
       bgHowTo.add_component<Components::Sprite>(&howToPlayTexture);
   sp_bgHowTo.z = -1;
   bgHowTo.add_component<Components::Info>("BackGroundHowToPlay", "HowToPlay");
}