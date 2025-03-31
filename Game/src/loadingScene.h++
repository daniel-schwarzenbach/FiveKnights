#pragma once
#include <Senpai>
using namespace Senpai;

struct loading_animator final : public Script {
   f32 timetochange = 0.1f;

   void on_update(f32 dt) override {
      if ((timetochange -= dt) > 0) return;
      timetochange = 0.3f;
      auto &b = entityPtr->get_component<Components::ButtonUI>();
      auto &tr = entityPtr->get_component<Components::Transform>();
      f32 shift = b.fontPtr->size / 10.0f;
      if (b.text == "Loading...") {
         b.text = "Loading";
         tr.frame.position.x = 0;
      } else if (b.text == "Loading") {
         b.text = "Loading.";
         tr.frame.position.x = +shift;
      } else if (b.text == "Loading.") {
         b.text = "Loading..";
         tr.frame.position.x = +2 * shift;
      } else if (b.text == "Loading..") {
         b.text = "Loading...";
         tr.frame.position.x = +3 * shift;
      }
   }
};

void set_up_loading_scene(Ptr<Scene> scenePtr) {
   // system that runs the scripts
   scenePtr->add_system<Systems::ScriptRunner>();
   // system that handles the UI elemnts
   scenePtr->add_system<Systems::UIRenderSystem>();
   // load the font asset
   Assets::Font &font = scenePtr->add_asset<Assets::Font>(
       std::move(Assets::Font{"./assets/fonts/The Centurion .ttf", 200,
                              "HeroFont"}));
   // add the text entity
   Entity &loadingText = scenePtr->add_entity();
   // add the transform component to the entity
   auto &tr = loadingText.add_component<Components::Transform>();
   // add a button ui component to the entity
   auto &button = loadingText.add_component<Components::ButtonUI>(
       Components::ButtonUI{&font,
                            "Loading",
                            {255, 255, 255, 255},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0}});
   // add the animation script to the entity
   loadingText.add_script<loading_animator>();
}