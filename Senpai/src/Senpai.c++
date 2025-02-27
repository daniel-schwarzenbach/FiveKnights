#include <Senpai.h++>
#include <SDL3/SDL.h>

namespace Senpai
{
   bool App::run(){
      if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO)){
         cout << "sdl initialized!" << endl;
      } else {
         return false;
      }

      for(auto i : Range{0,10}){
        cout << "PC: " << i << endl;
        sleep(0.5);
      }

      SDL_Quit();
      return true;
   }
   

} // namespace Senpai