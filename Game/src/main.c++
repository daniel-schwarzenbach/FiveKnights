#include "./loadingScene.h++"
#include "./mainMenu.h++"
#include <cstdlib>

int main (){
   std::system("ls -R");
   Senpai::App app;
   app.settings.fpsTarget = 30;
   app.sceneLoaders.push_back(set_up_loading_scene);
   app.sceneLoaders.push_back(set_up_menu_scene);
   app.run();

   cout << "App closed successfully" << endl;
   
   
   
   return 0;
};