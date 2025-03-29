#pragma once
#include <Senpai>
using namespace Senpai;

struct loading_animator final : public Script {
   f32 timetochange = 0.1f;

   void on_update(f32 dt) override {

      if ((timetochange -= dt) > 0)
         return;
      timetochange = 0.3f;
      auto &b = entityPtr->get_component<Components::ButtonUI>();
      auto& tr = entityPtr->get_component<Components::Transform>();
      f32 shift = b.fontPtr->size / 10.0f;
      if (b.text == "Loading...") {
         b.text = "Loading";
         tr.frame.position.x = 0;
      } else if (b.text == "Loading") {
         b.text = "Loading.";
         tr.frame.position.x = +shift;
      } else if (b.text == "Loading.") {
         b.text = "Loading..";
         tr.frame.position.x = +2*shift;
      } else if (b.text == "Loading..") {
         b.text = "Loading...";
         tr.frame.position.x = +3*shift;
      }
   }
};

void set_up_loading_scene(Ptr<Scene> scenePtr) {
   scenePtr->add_system<Systems::ScriptRunner>();
   // scenePtr->add_system<Systems::CameraRenderSystem>();
   scenePtr->add_system<Systems::UIRenderSystem>();

   Assets::Font &font = scenePtr->add_asset<Assets::Font>(std::move(Assets::Font{
       String("./assets/fonts/The Centurion .ttf"), 200, String("HeroFont")}));
   /* auto& texture = scenePtr->add_asset<Assets::Texture>(String("./assets/pics/Test.png"), String("MenuBackground")); */
   

   Entity &loadingText = scenePtr->add_entity();
   auto &tr = loadingText.add_component<Components::Transform>();
   auto &button = loadingText.add_component<Components::ButtonUI>(
       Components::ButtonUI{&font,
                            "Loading",
                            {255, 255, 255, 255},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0}});
   loadingText.add_script<loading_animator>();

   // camera entity
   Entity& camera = scenePtr->add_entity();
   camera.add_component<Components::Transform>();
   camera.add_component<Components::Camera>(Vec2<f32>{0,0},1.6);
   // background entity
   /* Entity &bg = scenePtr->add_entity();
   auto &bg_tr = bg.add_component<Components::Transform>();
   auto &bg_sprite = bg.add_component<Components::Sprite>(&texture);
   bg_sprite.z = -5; */
}