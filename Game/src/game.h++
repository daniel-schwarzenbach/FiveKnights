#pragma once
#include <Senpai>
using namespace Senpai;

#include "./ChessComputer.h++"

// generate the positions of the knights at the boarder
Vector<V2> generate_knight_positions() {
   int n = rng::get_in_range(1, 4);
   V2 k0;
   switch(n) {
      case 1:
         k0 = {1,1};
         break;
      case 2:
         k0 = {1,boardsize.y};
         break;
      case 3:
         k0 = {boardsize.x,1};
         break;
      case 4:
         k0 = {boardsize.x, boardsize.y};
         break;
   }
   int x1 = rng::get_in_range(2, boardsize.x-1);
   int x2 = rng::get_in_range(2, boardsize.x-1);
   int y3 = rng::get_in_range(2, boardsize.y-1);
   int y4 = rng::get_in_range(2, boardsize.y-1);
   return {k0, V2{x1, 1}, V2{x2, boardsize.y}, V2{1, y3}, V2{boardsize.x, y4}};
}

Matrix<u16> generate_map(uint rows, uint cols) {
   Matrix<u16> map(rows, cols);
   bool startWhite = true;
   for (uint i = 0; i < rows; i++) {
      bool isWhite = startWhite;
      for (uint j = 0; j < cols; j++) {
         map[i, j] = rng::get_in_range(0, 7);
         if (isWhite) {
            map[i, j] += 8;
         }
         isWhite = !isWhite;
      }
      startWhite = !startWhite;
   }
   return map;
}

f32 timer = 0;
constexpr f32 yUnit = 32;
constexpr f32 xUnit = 48;
const Vec2<f32> playerStart = {4 * xUnit, 4 * yUnit};


struct Movement {
   Vec2<f32> start;
   Vec2<f32> target;
   f32 timeTotal = -1;
   f32 currentTime = 0;

   inline Vec2<f32> get_current_pos(f32 Δt) {
      currentTime += Δt;
      if (currentTime >= timeTotal) {
         return target;
      }
      f32 tRelative = currentTime / timeTotal;
      return lin_interpolate(start, target, tRelative);
   }

   inline bool is_moving() { return currentTime < timeTotal; }
};

struct GameManager final : public Script {
   Ptr<Entity> playerPtr;
   Assets::Audio *ambientPtr;

   void on_start() override {
      ambientPtr->play(1.0, true);
      timer = 0;
      auto &tr = entityPtr->get_component<Components::Transform>();
      auto &trくplayer = playerPtr->get_component<Components::Transform>();
      tr.frame.position = trくplayer.frame.position;
   }
   void on_update(f32 Δt) override {
      timer += Δt;
      auto &tr = entityPtr->get_component<Components::Transform>();
      auto &trくplayer = playerPtr->get_component<Components::Transform>();
      Vec2<f32> pos =
          lin_interpolate(tr.frame.position, trくplayer.frame.position, 0.1f);
      tr.frame.position = pos;
   }
};



struct KnightScript final : public Script {
   Movement movement;
   const f32 moveToggleTime = 2.0;
   f32 moveToggle = 1;
   public:
      // position on the chessboard
      Vec2<int> virtualPos;
      // knight id ∈ {1, ..., 5}
      int id;
      // constuctor
      inline KnightScript(Vec2<int> vPos, int id) : id{id}, virtualPos{vPos} {}
};

struct PlayerMovement final : public Script {
 private:
   bool flashlightToggle = false;
   Vec2<f32> lastMouse = {0, 0};

 public:
   Ptr<Assets::Audio> audioPtr;
   Ptr<Assets::Audio> flashAudioPtr;
   const f32 toggleTime = 2.0;
   f32 toggle = 1;
   // for the chess
   Vec2<int> virtualPos = {5,5};
   // movement
   Movement movement;
   // flashlight
   Entity *flashlightPtr = nullptr;

   void on_start() override {
      flashlightPtr->disable();
   }

   void on_update(f32 Δt) override {

      if (flashlightPtr->is_enabled()) {
         Vec2<f32> mouse = Inputs::get_mouse_position();
         if (!(lastMouse.similar_to(mouse))) {
            f32 factor = pow(0.95f, 1.0 / Δt);
            lastMouse = lin_interpolate(lastMouse, mouse, factor);
            f32 angle = get_angle(lastMouse) - 90.0f;
            auto &tr = flashlightPtr->get_component<Components::Transform>();
            tr.frame.rotation = angle;
         }
      } else {
         lastMouse = Inputs::get_mouse_position();
         f32 angle = get_angle(lastMouse) - 90.0f;
         auto &tr = flashlightPtr->get_component<Components::Transform>();
         tr.frame.rotation = angle;
      }

      // toglle flashlight
      if (Inputs::get_mouse_button(1) || Inputs::get_mouse_button(2)) {
         if (!flashlightToggle) {
            flashlightToggle = true;
            if (flashAudioPtr)
               flashAudioPtr->play();
            if (flashlightPtr->is_enabled()) {
               flashlightPtr->disable();
            } else {
               flashlightPtr->enable();
               
            }
         }
      } else {
         flashlightToggle = false;
      }
      if (movement.is_moving()) {
         auto &tr = entityPtr->get_component<Components::Transform>();
         tr.frame.position = movement.get_current_pos(Δt);
      }
      if (toggle < 0) {
         toggle += Δt;
         return;
      }
      Vec2<f32> direction = {0, 0};
      Vec2<int> virtualDir = {0, 0};
      if (Inputs::get_key(Key::W)) {
         direction += {0, yUnit};
         virtualDir += {0, 1};
      }
      if (Inputs::get_key(Key::S)) {
         direction += {0, -yUnit};
         virtualDir += {0, -1};
      }
      if (Inputs::get_key(Key::D)) {
         direction += {xUnit, 0};
         virtualDir += {1, 0};
      }
      if (Inputs::get_key(Key::A)) {
         direction += {-xUnit, 0};
         virtualDir += {-1, 0};
      }
      if (virtualDir != Vec2<int>{0, 0} && is_in_range(virtualDir + virtualPos)) {
         virtualPos += virtualDir;
         toggle = -toggleTime;
         auto &tr = entityPtr->get_component<Components::Transform>();
         auto &anim = entityPtr->get_component<Components::Animator>();
         movement = {tr.frame.position, tr.frame.position + direction, 0.5f,
                     0.0f};
         anim.switch_animation(1);
         if (audioPtr)
            audioPtr->play();
      }
   }
};

Entity& create_knight(Ptr<Scene> scenePtr, Vec2<int> virtualPos, Ptr<Assets::Texture> knightTexture) {
   static int id = 1;
   auto& knight = scenePtr->add_entity();
   Vec2<f32> offset = {0,+16};
   auto& tr = knight.add_component<Components::Transform>(Vec2<f32>{xUnit * (virtualPos.x-1), yUnit * (virtualPos.y-1)});
   auto& sp = knight.add_component<Components::Sprite>(knightTexture,offset);
   auto& script = knight.add_script<KnightScript>(virtualPos, id++);
   return knight;
}

void load_game(Ptr<Scene> scene) {
   ChessComputer::reset();
   Vector<V2> knightPositions = generate_knight_positions();
   ChessComputer::init(2, V2{5,5}, knightPositions);
   //       ADD SYSTEMS
   scene->add_system<Systems::ScriptRunner>();
   scene->add_system<Systems::SpriteAnimator>();
   scene->add_system<Systems::CameraRenderSystem>();
   scene->add_system<Systems::LightingSystem>();
   scene->add_system<Systems::UIRenderSystem>();

   //      ADD ASSETS
   auto &font = scene->add_asset<Assets::Font>(
       String("./assets/fonts/Griffiths.otf"), 200, String("Writtenfont"));
   auto &kingIdleTexture = scene->add_asset<Assets::Texture>(
       String("./assets/pics/KingIdle.png"), String("KingIdle"), true);
   auto &kingIdleAnim = scene->add_asset<Assets::Animation>(
       &kingIdleTexture, Vec2<f32>{48, 64}, 18);
   auto &kingRunTexture = scene->add_asset<Assets::Texture>(
       String("./assets/pics/KingRun.png"), String("KingRun"), true);
   auto &kingRunAnim = scene->add_asset<Assets::Animation>(
       &kingRunTexture, Vec2<f32>{48, 64}, 20);
   auto &kingAudio =
       scene->add_asset<Assets::Audio>("./assets/audio/King.mp3", "KingAudio");
   Assets::Audio &ambient = scene->add_asset<Assets::Audio>(
       String("./assets/audio/AmbientLoop.wav"), String("Ambient"));
   auto &tileSet = scene->add_asset<Assets::TileSet>(
       "./assets/pics/Tilemap.png", 4, 4, xUnit, yUnit);
   auto &laternLight = scene->add_asset<Assets::Texture>(
       "./assets/pics/LaternLight.png", "LaternLight", true);
   auto &flashLight = scene->add_asset<Assets::Texture>(
       "./assets/pics/Flashlight.png", "Flashlight", true);
   auto &knightTexture = scene->add_asset<Assets::Texture>("./assets/pics/Knight.png",
                                                    "Knight", true);
   auto &flashSound = scene->add_asset<Assets::Audio>(
       "./assets/audio/Flashlight.wav", "FlashlightSound");

   // GameManager Entity
   auto &manager = scene->add_entity();
   auto &trくmanager =
       manager.add_component<Components::Transform>(Vec2<f32>{0, 0});
   auto &camくmanager =
       manager.add_component<Components::Camera>(Vec2<f32>{0, 0}, 5.0);
   auto &gmくmanager = manager.add_script<GameManager>();
   gmくmanager.ambientPtr = &ambient;

   // Player Entity
   auto &player = scene->add_entity();
   auto &trくplayer =
       player.add_component<Components::Transform>(Vec2<f32>{0, 12} + playerStart);
   auto &spくplayer =
       player.add_component<Components::Sprite>(&kingIdleTexture);
   auto &animくplayer = player.add_component<Components::Animator>(
       30, Vector<Ptr<Assets::Animation>>{&kingIdleAnim, &kingRunAnim},
       Vector<u32>{0, 0});
   auto &pmくplayer = player.add_script<PlayerMovement>();
   pmくplayer.audioPtr = &kingAudio;
   auto &lightくplayer = player.add_component<Components::Light>(&laternLight);
   lightくplayer.offset = {0, -5};

   // Flashlight entity
   auto &flashlight = scene->add_entity();
   auto &trくflashlight = flashlight.add_component<Components::Transform>(
       Vec2<f32>{0, 100}, Vec2<f32>{1.0, 2.5}, 15, Vec2<f32>{0, -0.7});
   auto &lightくflashlight =
       flashlight.add_component<Components::Light>(&flashLight);
   flashlight.parentPtr = &player;
   pmくplayer.flashlightPtr = &flashlight;
   pmくplayer.flashAudioPtr = &flashSound;
   gmくmanager.playerPtr = &player;

   // Knight1
   auto& knight1 = create_knight(scene, , &knightTexture);

   // chessboard Entity
   auto &chess = scene->add_entity();
   auto &trくchess = chess.add_component<Components::Transform>();
   auto &tmくchess =
       chess.add_component<Components::TileMap>(generate_map(9, 9), &tileSet);
   tmくchess.z = 10;
}