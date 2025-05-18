#include <cstdlib>

#include "./game.h++"
#include "./loadingScene.h++"
#include "./mainMenu.h++"

int main() {
   // create an app
   Senpai::App app;
   // set the app settings
   app.settings.fps = 30;
   app.settings = read_from_file<Senpai::AppSettings>("./assets/data/app_settings.dat");
   app.icon = "assets/pics/Icon.png";
   app.title = "Five Knights against King Fredric";
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