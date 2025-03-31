#include <cstdlib>

#include "./game.h++"
#include "./loadingScene.h++"
#include "./mainMenu.h++"

int main() {
   // create an app
   Senpai::App app;
   // set the app settings
   app.settings.fpsTarget = 30;
   app.title = "Five Knights against King Fredric";
   app.settings.width = 1920;
   app.settings.height = 1080;
   // add the scenes to the app
   app.sceneLoaders.push_back(set_up_loading_scene);
   app.sceneLoaders.push_back(set_up_menu_scene);
   app.sceneLoaders.push_back(load_game);
   // run the app
   app.run();
   // make sure the chess computer is reset
   chess_computer::reset();
   cout << "App closed successfully" << endl;
   // return 0 to the OS
   return 0;
}