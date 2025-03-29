#pragma once
#include <Senpai>
#include <fstream>
using namespace Senpai;

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
   void on_start() {
      ambient->play(0.4f, true);
      music->play(0.4f, false);
   }
   void on_update(f32 dt) override {
      auto &tr = entityPtr->get_component<Components::Transform>();
      bool show = false;
      bool toggleSpace = false;
      if (Inputs::get_key(Key::A)) {
         tr.frame.position.x -= 500 * dt;
         show = true;
      }
      if (Inputs::get_key(Key::D)) {
         tr.frame.position.x += 500 * dt;
         show = true;
      }
      if (Inputs::get_key(Key::W)) {
         tr.frame.position.y += 500 * dt;
         show = true;
      }
      if (Inputs::get_key(Key::S)) {
         tr.frame.position.y -= 500 * dt;
         show = true;
      }
      if (Inputs::get_key(Key::Q)) {
         tr.frame.size *= std::pow(2.0f, dt);
         show = true;
      }
      if (Inputs::get_key(Key::E)) {
         tr.frame.size /= std::pow(2.0f, dt);
         show = true;
      }

      bool space = Inputs::get_key(Key::SPACE);
      if (space && !toggleSpace) {
         toggleSpace = true;
         cout << tr.frame.position.x << ", " << tr.frame.position.y << endl;
      } else if (!space) {
         toggleSpace = false;
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
   auto& texture = scenePtr->add_asset<Assets::Texture>(String("./assets/pics/Midjourney.png"), String("MenuBackground"));
   Assets::Audio &ambient = scenePtr->add_asset<Assets::Audio>(String("./assets/audio/AmbientLoop.wav"), String("Ambient"));
   Assets::Audio &music = scenePtr->add_asset<Assets::Audio>(String("./assets/audio/Musique.wav"), String("MenuMusic"));
   clickSound = &scenePtr->add_asset<Assets::Audio>(String("./assets/audio/Click.mp3"), String("Click"));
   f32 highScore = read_high_score("./assets/data/highscore.dat");
   // camera entity
   Entity& camera = scenePtr->add_entity();
   camera.add_component<Components::Transform>();
   camera.add_component<Components::Camera>(Vec2<f32>{0,0},0.7);
   auto& ms = camera.add_script<menuHandlerScript>();
   ms.music = &music;
   ms.ambient = &ambient;
   // background entity
   Entity &bg = scenePtr->add_entity();
   auto &bg_tr = bg.add_component<Components::Transform>();
   auto &bg_sprite = bg.add_component<Components::Sprite>(&texture);
   bg_sprite.z = -5;
   // start game button entity
   Entity& game = scenePtr->add_entity();
   auto& game_tr = game.add_component<Components::Transform>(Vec2<f32>{0, 0});
   auto& game_button = game.add_component<Components::ButtonUI>(&font, String("Start Game"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{900, 150});
   game.add_script<startGameScript>();
   // settings button entity
   Entity& settings = scenePtr->add_entity();
   auto& settings_tr = settings.add_component<Components::Transform>(Vec2<f32>{0, -200});
   auto& settings_button = settings.add_component<Components::ButtonUI>(&font, String("Settings"), Color{255, 255, 255, 255},Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{900, 150});
   settings.add_script<settingsScript>();
   // Exit button entity
   Entity& exit = scenePtr->add_entity();
   auto& exit_tr = exit.add_component<Components::Transform>(Vec2<f32>{0, -400});
   auto& exit_button = exit.add_component<Components::ButtonUI>(&font, String("Exit"), Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{30, 30, 0, 30}, Vec2<f32>{900, 150});
   exit.add_script<exitScript>();
   // Highscore entity
   Entity& score = scenePtr->add_entity();
   auto& score_tr = score.add_component<Components::Transform>(Vec2<f32>{-700, -500});
   auto& score_btn = score.add_component<Components::ButtonUI>(&fontsmall, str("Longest Survival:  ") + str(highScore) + str("s"), Color{255, 255, 255, 255}, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{0, 0});
   // Title entity
   Entity& title = scenePtr->add_entity();
   auto& title_tr = title.add_component<Components::Transform>(Vec2<f32>{0, 350});
   auto& title_btn = title.add_component<Components::ButtonUI>(&fontLarge, String("Five Knights against King Fredric"), Color{255, 255, 255, 255}, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{0, 0});
}