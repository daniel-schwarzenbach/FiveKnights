#pragma once
#include <Senpai>
#include <fstream>
using namespace Senpai;

enum class MenuState {
   MAIN,
   SETTINGS,
   HOW_TO_PLAY
};

inline static MenuState menuState = MenuState::MAIN;

f32 read_high_score(const std::string& filePath) {
   std::ifstream file(filePath);
   if (!file) {
      std::cerr << "Failed to open file: " << filePath << std::endl;
      return 0.0f;
   }
   f32 score;
   file >> score;
   return score;
}

static Ptr<Assets::Audio> clickSound;

struct menuHandlerScript final : public Script {
   Ptr<Assets::Audio> ambient;
   Ptr<Assets::Audio> music;
   Vector<Entity *> mainMenuEntities = {};
   Vector<Entity *> settingsEntities  = {};
   Vector<Entity *> howToPlayEntities  = {};
   MenuState state = MenuState::MAIN;
   void on_start() {
      menuState = state = MenuState::MAIN;
      ambient->play(0.4f, true);
      music->play(0.4f, false);
      for (auto entity : view<Components::Info>()) {
         if (entity->has_component<Components::Info>() == false) continue;
         auto &info = entity->get_component<Components::Info>();
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
      if (state != MenuState::MAIN) {
         if (Inputs::get_key(Key::ESCAPE)) {
            menuState = MenuState::MAIN;
            clickSound->play(1, false);
         }
      }
      if (state != menuState) {
         Vector<Entity *>* entities = nullptr;
         switch (state) {
            case MenuState::MAIN:
               entities = &mainMenuEntities;
               break;
            case MenuState::SETTINGS:
               entities = &settingsEntities;
               break;
            case MenuState::HOW_TO_PLAY:
               entities = &howToPlayEntities;
               break;
         }
         for (auto entity : *entities) {
            entity->disable();
         }
         state = menuState;
         switch (state) {
            case MenuState::MAIN:
               entities = &mainMenuEntities;
               break;
            case MenuState::SETTINGS:
               entities = &settingsEntities;
               break;
            case MenuState::HOW_TO_PLAY:
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
      if(toggle < 0) {
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

struct HowToPlayScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if(toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         menuState = MenuState::HOW_TO_PLAY;
         clickSound->play(1, false);
      }
   }
};


struct settingsScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if(toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         menuState = MenuState::SETTINGS;
         clickSound->play(1, false);
      }
   }
};

struct ReturnScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if(toggle < 0) {
         toggle += dt;
      }
   }
   void on_button_click() override {
      if (toggle >= 0) {
         toggle = -0.5;
         menuState = MenuState::MAIN;
         clickSound->play(1, false);
      }
   }
};

struct exitScript final : public Script {
   f32 toggle = 0;
   void on_update(f32 dt) override {
      if(toggle < 0) {
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
Vec2<f32> get_button_pos() {
   auto copy = buttonPos;
   buttonPos.y -= offset;
   return  copy;
}

void set_up_menu_scene(Ptr<Scene> scenePtr) {
   // register all needet systems
   scenePtr->add_system<Systems::UIButtonSystem>();
   scenePtr->add_system<Systems::ScriptRunner>();
   scenePtr->add_system<Systems::CameraRenderSystem>();
   scenePtr->add_system<Systems::UIRenderSystem>();
   // load needet assets
   Assets::Font &font = scenePtr->add_asset<Assets::Font>(String("./assets/fonts/The Centurion .ttf"), 100, String("HeroFont"));
   Assets::Font &fontLarge = scenePtr->add_asset<Assets::Font>(String("./assets/fonts/The Centurion .ttf"), 130, String("HeroFont"));
   Assets::Font &fontsmall = scenePtr->add_asset<Assets::Font>(String("./assets/fonts/Griffiths.otf"), 50, String("HeroFontSmall"));
   auto& texture = scenePtr->add_asset<Assets::Texture>(String("./assets/pics/BackGround.png"), String("MenuBackground"));
   Assets::Audio &ambient = scenePtr->add_asset<Assets::Audio>(String("./assets/audio/AmbientLoop.wav"), String("Ambient"));
   Assets::Audio &music = scenePtr->add_asset<Assets::Audio>(String("./assets/audio/Musique.wav"), String("MenuMusic"));
   clickSound = &scenePtr->add_asset<Assets::Audio>(String("./assets/audio/Click.mp3"), String("Click"));
   f32 highScore = read_high_score("./assets/data/highscore.dat");
   // camera entity
   Entity& camera = scenePtr->add_entity();
   camera.add_component<Components::Transform>();
   camera.add_component<Components::Camera>(Vec2<f32>{0,0},0.45);
   auto& ms = camera.add_script<menuHandlerScript>();
   ms.music = &music;
   ms.ambient = &ambient;
   // background entity
   Entity &bg = scenePtr->add_entity();
   auto &bg_tr = bg.add_component<Components::Transform>();
   auto &bg_sprite = bg.add_component<Components::Sprite>(&texture);
   bg_sprite.z = -5;

   // create the menu entities
   buttonPos = {0, 150};

   // start game button entity
   Entity& game = scenePtr->add_entity();
   auto& game_tr = game.add_component<Components::Transform>(get_button_pos());
   auto& game_button = game.add_component<Components::ButtonUI>(&font, String("Start Game"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   game.add_script<startGameScript>();
   game.add_component<Components::Info>("StartGame", "Main");

   // How to play button entity
   Entity& how = scenePtr->add_entity();
   auto& how_tr = how.add_component<Components::Transform>(get_button_pos());
   auto& how_button = how.add_component<Components::ButtonUI>(&font, String("How to Play?"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   how.add_script<HowToPlayScript>();
   auto& how_info = how.add_component<Components::Info>("HowToPlay", "Main");

   // settings button entity
   Entity& settings = scenePtr->add_entity();
   auto& settings_tr = settings.add_component<Components::Transform>(get_button_pos());
   auto& settings_button = settings.add_component<Components::ButtonUI>(&font, String("Settings"), Color{255, 255, 255, 255},Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   settings.add_script<settingsScript>();
   settings.add_component<Components::Info>("Settings", "Main");

   // Exit button entity
   Entity& exit = scenePtr->add_entity();
   auto& exit_tr = exit.add_component<Components::Transform>(get_button_pos());
   auto& exit_button = exit.add_component<Components::ButtonUI>(&font, String("Exit"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   exit.add_script<exitScript>();
   exit.add_component<Components::Info>("Exit", "Main");



   // Highscore entity
   Entity& score = scenePtr->add_entity();
   auto& score_tr = score.add_component<Components::Transform>(Vec2<f32>{-700, -500});
   auto& score_btn = score.add_component<Components::ButtonUI>(&fontsmall, str("Longest Survival:  ") + str(highScore) + str("s"), Color{255, 255, 255, 255}, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{0, 0});
   score.add_component<Components::Info>("Highscore", "Main");

   // Title entity
   Entity& title = scenePtr->add_entity();
   auto& title_tr = title.add_component<Components::Transform>(Vec2<f32>{0, 350});
   auto& title_btn = title.add_component<Components::ButtonUI>(&fontLarge, String("Five Knights against King Fredric"), Color{255, 255, 255, 255}, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{0, 0});
   title_btn.z = 4;
   title.add_component<Components::Info>("Title", "Main");
   // Title Shdow entity
   Entity& titlesh = scenePtr->add_entity();
   auto& titlesh_tr = titlesh.add_component<Components::Transform>(Vec2<f32>{5, 344});
   auto& titlesh_btn = titlesh.add_component<Components::ButtonUI>(&fontLarge, String("Five Knights against King Fredric"), Color{0, 0, 0, 200}, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{0, 0});
   titlesh_btn.z = 3;
   titlesh.add_component<Components::Info>("TitleShadow", "Main");

   //       - Settings -
   buttonPos = {0, 400};

   // return button
   Entity& returnSet = scenePtr->add_entity();
   auto& returnSet_tr = returnSet.add_component<Components::Transform>(get_button_pos());
   auto& returnSet_button = returnSet.add_component<Components::ButtonUI>(&font, String("Return"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   returnSet.add_script<ReturnScript>();
   returnSet.add_component<Components::Info>("Return", "Settings");


   //       - How to Play -
   buttonPos = {0, 400};

   // return button
   Entity& returnHow = scenePtr->add_entity();
   auto& returnHow_tr = returnHow.add_component<Components::Transform>(get_button_pos());
   auto& returnHow_button = returnHow.add_component<Components::ButtonUI>(&font, String("Return"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{1920, 150});
   returnHow.add_script<ReturnScript>();
   returnHow.add_component<Components::Info>("Return", "HowToPlay");


}