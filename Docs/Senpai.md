# Senpai Game Engine

Die von mir erstellte Spiele Engine Basiert auf diesem Buch: [CROSS-PLATFORM 2D GAME WITH SDL2](https://www.amazon.com/dp/B09SWTG4DC)

Der Quellcode des Buches ist hier verfügbar: https://github.com/Madsycode/book-fuse-engine

Senpai ist aber keine einfache Kopie!

Features wie Mehrere Scenen, Licht, Geometrie und Kameras sind nur einige der eigen erstellten Features und Optimierungen.

## How to use it

To run an app, one has to define Scenefunctions and push them into the `app.sceneLoaders`

```cpp
#include <cstdlib>

extern void load_loading_scene(Senpai::Scene* scenePtr);
extern void load_main_menu_scene(Senpai::Scene* scenePtr);
extern void load_game_scene(Senpai::Scene* scenePtr);

int main() {
   // create an app
   Senpai::App app;
   // set the app settings
   app.settings.fpsTarget = 30;
   // add the scenes to the app
   app.sceneLoaders.push_back(load_loading_scene);
   app.sceneLoaders.push_back(load_main_menu_scene);
   app.sceneLoaders.push_back(load_game_scene);
   // run the app
   app.run();
   // return 0 to the OS
   return 0;
}
```

## How it works

App loads scenes from the load functions

Every Scene consists of Asset, Entities and Systems. An Entity meanwhile consist of multiple Components that get updated by the Systems.

![Senpai Engine](./pics/ECSSystem.jpg)

## ECRegistry 

The Entity Component Registry contains all components and keeps track of which entity has which component.

## AssetsRegistry

The Assets Registry makes the Assets more accescable, by id and name.

## Input Class

static Class that provides easy access to events