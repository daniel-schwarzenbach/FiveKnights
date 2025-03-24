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

struct move_script final : public Script {
   Ptr<Assets::Audio> audio;
   void on_start() {
      audio->play(1.0, true);
   }
   void on_update(f32 Δt) override {
      auto &tr = entityPtr->get_component<Components::Transform>();
      bool show = false;
      bool toggleSpace = false;
      if (Inputs::get_key(Key::A)) {
         tr.frame.position.x -= 500 * Δt;
         show = true;
      }
      if (Inputs::get_key(Key::D)) {
         tr.frame.position.x += 500 * Δt;
         show = true;
      }
      if (Inputs::get_key(Key::W)) {
         tr.frame.position.y += 500 * Δt;
         show = true;
      }
      if (Inputs::get_key(Key::S)) {
         tr.frame.position.y -= 500 * Δt;
         show = true;
      }
      if (Inputs::get_key(Key::Q)) {
         tr.frame.size *= std::pow(2.0f, Δt);
         show = true;
      }
      if (Inputs::get_key(Key::E)) {
         tr.frame.size /= std::pow(2.0f, Δt);
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

struct start_game_script final : public Script {
   void on_button_click() override {
      // this will load the game scene
      scenePtr->nextScene = 2;
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
   Assets::Font &fontsmall = scenePtr->add_asset<Assets::Font>(String("./assets/fonts/The Centurion .ttf"), 50, String("HeroFontSmall"));
   auto& texture = scenePtr->add_asset<Assets::Texture>(String("./assets/pics/Test.png"), String("MenuBackground"));
   Assets::Audio &music = scenePtr->add_asset<Assets::Audio>(String("./assets/audio/Musique.mp3"), String("MenuMusic"));
   music.channel = 1;
   f32 highScore = read_high_score("./assets/data/highscore.dat");
   // camera entity
   Entity& camera = scenePtr->add_entity();
   camera.add_component<Components::Transform>();
   camera.add_component<Components::Camera>(Vec2<f32>{0,0},2.0);
   auto& ms = camera.add_script<move_script>();
   ms.audio = &music;
   // background entity
   Entity &bg = scenePtr->add_entity();
   auto &bg_tr = bg.add_component<Components::Transform>();
   auto &bg_sprite = bg.add_component<Components::Sprite>(&texture);
   bg_sprite.z = -5;
   // start game button entity
   Entity& game = scenePtr->add_entity();
   auto& game_tr = game.add_component<Components::Transform>(Vec2<f32>{0, 200});
   auto& game_button = game.add_component<Components::ButtonUI>(&font, String("Start Game"), Color{255, 255, 255, 255}, 1, Color{0, 0, 0, 0}, Color{255, 255, 255, 30}, Vec2<f32>{900, 250});
   game.add_script<start_game_script>();
   // settings button entity
   Entity& settings = scenePtr->add_entity();
   auto& settings_tr = settings.add_component<Components::Transform>(Vec2<f32>{0, -200});
   auto& settings_button = settings.add_component<Components::ButtonUI>(&font, String("Settings"), Color{255, 255, 255, 255}, 1, Color{0, 0, 0, 0}, Color{255, 255, 255, 30}, Vec2<f32>{900, 250});
   // settings button entity
   Entity& score = scenePtr->add_entity();
   auto& score_tr = score.add_component<Components::Transform>(Vec2<f32>{0, 0});
   auto& score_btn = score.add_component<Components::ButtonUI>(&fontsmall, String("Highscore = ") + str(highScore), Color{255, 255, 255, 255}, 1, Color{0,0,0,0}, Color{0,0,0,0}, Vec2<f32>{900, 250});
}