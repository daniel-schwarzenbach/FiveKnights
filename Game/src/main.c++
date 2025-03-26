#include "./loadingScene.h++"
#include "./mainMenu.h++"
#include "./game.h++"
#include <cstdlib>

int main() {
   std::system("ls -R");

   Senpai::App app;
   app.settings.fpsTarget = 30;
   app.title = "Five Knights against King Fredric";
   app.width = 1920;
   app.height = 1080;
   app.sceneLoaders.push_back(set_up_loading_scene);
   app.sceneLoaders.push_back(set_up_menu_scene);
   app.sceneLoaders.push_back(load_game);
   app.run();

   cout << "App closed successfully" << endl;

   return 0;
};