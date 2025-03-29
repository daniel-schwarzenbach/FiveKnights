#pragma once
#include <Senpai>
using namespace Senpai;

#include "./ChessComputer.h++"

// generate the positions of the knights at the boarder
Vector<V2> generate_knight_positions() {
   int n = rng::get_in_range(1, 4);
   V2 k0;
   switch (n) {
   case 1:
      k0 = {1, 1};
      break;
   case 2:
      k0 = {1, boardsize.y};
      break;
   case 3:
      k0 = {boardsize.x, 1};
      break;
   case 4:
      k0 = {boardsize.x, boardsize.y};
      break;
   }
   int x1 = rng::get_in_range(2, boardsize.x - 1);
   int x2 = rng::get_in_range(2, boardsize.x - 1);
   int y3 = rng::get_in_range(2, boardsize.y - 1);
   int y4 = rng::get_in_range(2, boardsize.y - 1);
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

namespace game {
// heigth of a tile
constexpr f32 yUnit = 32;
// width of a tile
constexpr f32 xUnit = 48;
constexpr V2 kingStart = {5, 5};
constexpr Vec2<f32> playerStart = {4 * xUnit, 4 * yUnit};

// king can move all 4 sec
static constexpr f32 kingMoveTimer = 4;
// the time left until the king is allowed to move
f32 nextKingMove = -1;
f32 timer = 0;
f32 flashlightTime = 100;
bool gameOver = false;
V2 kingPosition = kingStart;
// battery of the flashlight
f32 batteryLeft = 120;
constexpr f32 batteryMax = 120;
// timer to increase the difficulty
f32 difficultyTimer = 20;
constexpr f32 difficultyTime = 20;



void init() {
   nextKingMove = -1;
   timer = 0;
   flashlightTime = 100;
   gameOver = false;
   kingPosition = kingStart;
   batteryLeft = batteryMax;
   difficultyTimer = difficultyTime;
}

Vec2<f32> virtual_to_actual(Vec2<int> vPos) {
   return Vec2<f32>{xUnit * (vPos.x - 1), yUnit * (vPos.y - 1)};
}
} // namespace game

struct Movement {
   Vec2<f32> start;
   Vec2<f32> target;
   f32 timeTotal = -1;
   f32 currentTime = 0;

   inline Vec2<f32> get_current_pos(f32 dt) {
      currentTime += dt;
      if (currentTime >= timeTotal) {
         return target;
      }
      f32 tRelative = currentTime / timeTotal;
      return lin_interpolate(start, target, tRelative);
   }

   inline bool is_moving() { return currentTime < timeTotal; }
};

struct KnightScript final : public Script {
   // if the knight has to flip after the move
   bool hasToAdjust = true;
   Movement movement;
   Ptr<Assets::Audio> audioPtr;

 public:
   // position on the chessboard
   Vec2<int> virtualPos;
   // constuctor
   inline KnightScript(Vec2<int> vPos, Assets::Audio *audioPtr)
       : virtualPos{vPos}, audioPtr{audioPtr} {}

   inline void do_move(Vec2<int> newPos, f32 dstTime, Vec2<int> kingPos) {
      if (newPos == virtualPos)
         return;
      debug_log("Knight moves from " << virtualPos << " to " << newPos);
      auto &tr = entityPtr->get_component<Components::Transform>();
      movement = {tr.frame.position, game::virtual_to_actual(newPos), dstTime,
                  0.0f};
      audioPtr->play_direction(convert<f32,int>(newPos - kingPos), 255.0f/10.0f);
      auto& sp = entityPtr->get_component<Components::Sprite>();
      hasToAdjust = true;
      virtualPos = newPos;

   }
   inline void flip_sprite() {
      auto &sp = entityPtr->get_component<Components::Sprite>();
      if(virtualPos.x > game::kingPosition.x) sp.flip = Flip::Horizontal;
      if(virtualPos.x < game::kingPosition.x) sp.flip = Flip::None;
   }

   inline void adjust_sprite() {
      auto &sp = entityPtr->get_component<Components::Sprite>();
      flip_sprite();
      sp.z = -virtualPos.y;
      hasToAdjust = false;
   }

   

   inline void on_update(f32 dt) override {
      if (movement.is_moving()) {
         auto &tr = entityPtr->get_component<Components::Transform>();
         tr.frame.position = movement.get_current_pos(dt);
         if (movement.is_moving())
            return;
         if (hasToAdjust) {
            adjust_sprite();
         }
         if (game::nextKingMove == inf) {
            game::gameOver = true;
         }
      }
   }
};

// Layer between the game and the ChessComputer
struct AIManager {
 private:
   // the average time the knight needs to move if the king is not moving
   static constexpr f32 knightRestTimer = 10;
   // the maxtime the knight needs to react to the kings move
   static constexpr f32 knightReactTimer = game::kingMoveTimer - 0.5;

 public:
   inline static bool isGameOver = false;
   // how fast travels the king from one position to another
   static constexpr f32 kingMoveTime = 0.5;
   // how fast travels the knight from one position to another
   static constexpr f32 knightMoveTime = 0.5;
   // all the positions of the knights
   inline static Vector<V2> knightPositions;
   // all knight scripts
   inline static Vector<KnightScript *> knightScriptPtrs;
   // the time left until the next knight moves
   inline static f32 nextKnightMove = 20;
   // the next move by the AI
   inline static chess_computer::ChessMove nextMove{-1, V2{0,0}};
   // if the king is in check
   inline static bool isCheck = false;

   // returns true if the player is allowed to move
   inline static bool player_move(V2 newPos) {
      if (game::nextKingMove > 0) {
         return false;
      } else {
         game::nextKingMove = game::kingMoveTimer;
         game::kingPosition = newPos;
         // if the player moves into check he dies
         if (chess_computer::is_check(newPos, knightPositions)) {
            uint id =
                chess_computer::which_knight_checks(newPos, knightPositions);
            knightScriptPtrs[id]->do_move(newPos, kingMoveTime * 1.2, newPos);
            knightPositions[id] = newPos;
            game::nextKingMove = inf;
            nextKnightMove = inf;
            return true;
         }
         if (!chess_computer::has_finished()) {
            debug_log("AI is still computing");
            return false;
         }
         // knight should soon counter move
         nextKnightMove = knightReactTimer * rng::get();
         auto copy = knightPositions;
         nextMove = chess_computer::get_result(newPos);
         game::kingPosition = newPos;
         for(auto knight : knightScriptPtrs) {
            knight->flip_sprite();
         }
         nextMove.apply(copy);
         isCheck = false;
         debug_log("Cumpute player_move()");
         if (copy != chess_computer::knightsPositions) {
            debug_warning(endl << "Desync on player_move()");
            debug_warning("KnightInput: " << copy);
            debug_warning("KnightGot: " << chess_computer::knightsPositions);
            debug_warning("NextMove: " << nextMove.knightId << ", " << nextMove.target);
         }
         return true;
         
      }
      return false;
   }

   inline static void init(V2 kingPos, Vector<KnightScript *> knights) {
      isGameOver = false;
      game::kingPosition = kingPos;
      knightScriptPtrs = knights;
      knightPositions.resize(knights.size());
      for (uint i = 0; i < knights.size(); i++) {
         knightPositions[i] = knights[i]->virtualPos;
      }
      chess_computer::reset();
      chess_computer::knightsPositions = knightPositions;
      chess_computer::init(2, V2{5, 5}, knightPositions);
   }

   inline static void sanaty_check(){
      Vector<V2> knights = {};
      for (auto knight : knightScriptPtrs) {
         knights.push_back(knight->virtualPos);
      }
      if(knights != knightPositions) {
         throw std::logic_error("Knight positions desynced");
      }
   }

   // to be called by game manager at the start of the game
   inline static void start() {
      nextMove = {-1, V2{0,0}};
      nextKnightMove =
          knightRestTimer * abs(rng::get_gaussian()) + game::kingMoveTimer;
      chess_computer::knightsPositions = knightPositions;
      // precompute all possible moves
      chess_computer::compute();
      for (auto knight : knightScriptPtrs) {
         knight->adjust_sprite();
      }
   }

   // to be called by the game manager at update
   inline static void update(f32 dt) {
      game::nextKingMove -= dt;
      nextKnightMove -= dt;
      game::difficultyTimer -= dt;
      if (game::difficultyTimer < 0) {
         game::difficultyTimer = game::difficultyTime;
         chess_computer::increase_difficulty();
      }
      #ifndef NDEBUG
      sanaty_check();
      #endif
      if (nextKnightMove < 0) {
         // the king was in check yet did not move
         if (isCheck) {
            debug_log("Checkmate");
            uint id =
                  chess_computer::which_knight_checks(game::kingPosition, knightPositions);
            knightScriptPtrs[id]->do_move(game::kingPosition, kingMoveTime * 1.2, game::kingPosition);
            game::nextKingMove = inf;
            nextKnightMove = inf;
            return;
         }
         // set the timer for the next king move to at least the time the knight needs to move
         game::nextKingMove = max(game::nextKingMove, knightMoveTime);
         // if did not got a next move from the AI
         if (nextMove.knightId == -1) {
            // check if a next move is ready
            if (!chess_computer::has_finished()) {
               return;
            }
            // get the next move if next move is not set yet
            nextMove = chess_computer::get_result(game::kingPosition);
         }
         bool flip = false;
         if (nextMove.target.x > game::kingPosition.x) {
            flip = true;
         }
         knightScriptPtrs[nextMove.knightId]->do_move(nextMove.target,
                                                     knightMoveTime, game::kingPosition);
         nextMove.apply(knightPositions);
         nextKnightMove =
             knightRestTimer * abs(rng::get_gaussian()) + game::kingMoveTimer;
         debug_log("nextKnightMove: " << nextKnightMove);
         nextMove = {-1, V2{0, 0}};
         if(chess_computer::is_check(game::kingPosition, knightPositions)) {
            cout << "Check" << endl;
            isCheck = true;
            return;
         }
         isCheck = false;
         debug_log("Cumpute update()");
         if(chess_computer::knightsPositions != knightPositions) {
            debug_warning(endl << "Desync on update()");
            debug_warning("KnightInput: " << knightPositions);
            debug_warning("KnightGot: " << chess_computer::knightsPositions);
            debug_warning("nextMove: " << nextMove.knightId << ", " << nextMove.target);
         }
      }
   }
};

struct GameManager final : public Script {
   bool once = false;
   Ptr<Entity> playerPtr;
   Ptr<Entity> flashLightUIPtr;
   Assets::Audio *ambientPtr;

   void on_game_over() {
      cout << "Game Over" << endl;
      chess_computer::reset();
   }

   void on_start() override {
      ambientPtr->play(1.0, true);
      game::timer = 0;
      auto &tr = entityPtr->get_component<Components::Transform>();
      auto &tr_player = playerPtr->get_component<Components::Transform>();
      tr.frame.position = tr_player.frame.position;
      AIManager::start();
   }
   void on_update(f32 dt) override {
      // handle game over case
      if (game::gameOver) {
         if (!once) {
            once = true;
            on_game_over();
         }
         if(Inputs::get_key(Key::ESCAPE)) {
            scenePtr->nextScene = 1;
         }
         return;
      }
      // increase the time
      game::timer += dt;
      // make the camera follow the player
      auto &tr = entityPtr->get_component<Components::Transform>();
      auto &tr_player = playerPtr->get_component<Components::Transform>();
      Vec2<f32> pos =
          lin_interpolate(tr.frame.position, tr_player.frame.position, 0.1f);
      tr.frame.position = pos;
      // update flashlightUI
      if (game::batteryLeft < 0) {
         game::batteryLeft = 0;
      }
      auto &textUI = flashLightUIPtr->get_component<Components::ButtonUI>();
      textUI.text = to_string(int((game::batteryLeft/game::batteryMax) * 100)) + " %";
      // update the AI
      AIManager::update(dt);
   }


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
   Vec2<int> virtualPos = {5, 5};
   // movement
   Movement movement;
   // flashlight
   Entity *flashlightPtr = nullptr;

   void on_start() override { flashlightPtr->disable(); }

   void on_update(f32 dt) override {

      if (flashlightPtr->is_enabled()) {
         Vec2<f32> mouse = Inputs::get_mouse_position();
         if (!(lastMouse.similar_to(mouse))) {
            f32 factor = pow(0.95f, 1.0 / dt);
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
      if (flashlightPtr->is_enabled()) {
         game::batteryLeft -= dt;
      }
      if (game::batteryLeft <= 0) {
         if (flashlightPtr->is_enabled()) {
            flashlightPtr->disable();
         }
      } else {
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
      }
      if (movement.is_moving()) {
         auto &tr = entityPtr->get_component<Components::Transform>();
         tr.frame.position = movement.get_current_pos(dt);
      }
      if (toggle < 0) {
         toggle += dt;
         return;
      }
      Vec2<f32> direction = {0, 0};
      Vec2<int> virtualDir = {0, 0};
      if (Inputs::get_key(Key::W)) {
         direction += {0, game::yUnit};
         virtualDir += {0, 1};
      }
      if (Inputs::get_key(Key::S)) {
         direction += {0, -game::yUnit};
         virtualDir += {0, -1};
      }
      if (Inputs::get_key(Key::D)) {
         direction += {game::xUnit, 0};
         virtualDir += {1, 0};
      }
      if (Inputs::get_key(Key::A)) {
         direction += {-game::xUnit, 0};
         virtualDir += {-1, 0};
      }
      if (Inputs::get_key(Key::Q)) {
         direction += {-game::xUnit, +game::yUnit};
         virtualDir += {-1, 1};
      }
      if (Inputs::get_key(Key::E)) {
         direction += {+game::xUnit, +game::yUnit};
         virtualDir += {1, 1};
      }
      if (Inputs::get_key(Key::Y) || Inputs::get_key(Key::Z)) {
         direction += {-game::xUnit, -game::yUnit};
         virtualDir += {-1, -1};
      }
      if (Inputs::get_key(Key::C)) {
         direction += {+game::xUnit, -game::yUnit};
         virtualDir += {1, -1};
      }
      if (virtualDir != Vec2<int>{0, 0} && in_range(virtualDir + virtualPos) &&
          AIManager::player_move(virtualDir + virtualPos)) {
         virtualPos += virtualDir;
         debug_assert(virtualPos == game::kingPosition, "King desync");
         toggle = -game::kingMoveTimer;
         auto &tr = entityPtr->get_component<Components::Transform>();
         auto &sp = entityPtr->get_component<Components::Sprite>();
         auto &anim = entityPtr->get_component<Components::Animator>();
         movement = {tr.frame.position, tr.frame.position + direction, AIManager::kingMoveTime,
                     0.0f};
         anim.switch_animation(1);
         sp.z = -virtualPos.y - 0.5f;
         if (audioPtr)
            audioPtr->play();
      }
   }
};


Tuple<Entity*, KnightScript*>
create_knight(Ptr<Scene> scenePtr, Vec2<int> virtualPos,
              Ptr<Assets::Texture> knightTexture, Ptr<Assets::Audio> audioPtr) {
   auto &knight = scenePtr->add_entity();
   Vec2<f32> offset = {0, +16};
   auto &tr = knight.add_component<Components::Transform>(
       game::virtual_to_actual(virtualPos));
   auto &sp = knight.add_component<Components::Sprite>(knightTexture, offset);
   auto &script = knight.add_script<KnightScript>(virtualPos, audioPtr);
   return {&knight, &script};
}

void load_game(Ptr<Scene> scene) {
   game::init();
   chess_computer::reset();
   Vector<V2> knightPositions = generate_knight_positions();
   //       ADD SYSTEMS
   scene->add_system<Systems::ScriptRunner>();
   scene->add_system<Systems::SpriteAnimator>();
   scene->add_system<Systems::CameraRenderSystem>();
   scene->add_system<Systems::LightingSystem>();
   scene->add_system<Systems::UIRenderSystem>();

   //      ADD ASSETS
   auto &font = scene->add_asset<Assets::Font>(
       String("./assets/fonts/Griffiths.otf"), 40, String("Writtenfont"));
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
       "./assets/pics/Tilemap.png", 4, 4, game::xUnit, game::yUnit);
   auto &laternLight = scene->add_asset<Assets::Texture>(
       "./assets/pics/LaternLight.png", "LaternLight", true);
   auto &flashLight = scene->add_asset<Assets::Texture>(
       "./assets/pics/Flashlight.png", "Flashlight", true);
   auto &knightTexture = scene->add_asset<Assets::Texture>(
       "./assets/pics/Knight.png", "Knight", true);
   auto &flashSound = scene->add_asset<Assets::Audio>(
       "./assets/audio/Flashlight.wav", "FlashlightSound");
   auto &knightAudio = scene->add_asset<Assets::Audio>(
       "./assets/audio/Horse.wav", "FlashlightSound");
   
   // FlashlightUI
   auto &flashlightUI = scene->add_entity();
   auto &tr_flashlightUI = flashlightUI.add_component<Components::Transform>(
       Vec2<f32>{0, 400}, Vec2<f32>{1.0, 1.0}, 100);
   auto &bt_flashlightUI = flashlightUI.add_component<Components::ButtonUI>(
      &font, "100 %", Color{255, 255, 255, 255}, Color{0, 0, 0, 0}, Color{0, 0, 0, 0}, Vec2<f32>{0, 0});

   // GameManager Entity
   auto &manager = scene->add_entity();
   auto &tr_manager =
       manager.add_component<Components::Transform>(Vec2<f32>{0, 0});
   auto &cam_manager =
       manager.add_component<Components::Camera>(Vec2<f32>{0, 0}, 5.0); // 5.0
   auto &gm_manager = manager.add_script<GameManager>();
   gm_manager.ambientPtr = &ambient;
   gm_manager.flashLightUIPtr = &flashlightUI;

   // Player Entity
   auto &player = scene->add_entity();
   auto &tr_player = player.add_component<Components::Transform>(
       Vec2<f32>{0, 12} + game::playerStart);
   auto &sp_player =
       player.add_component<Components::Sprite>(&kingIdleTexture);
   auto &anim_player = player.add_component<Components::Animator>(
       30, Vector<Ptr<Assets::Animation>>{&kingIdleAnim, &kingRunAnim},
       Vector<u32>{0, 0});
   auto &pm_player = player.add_script<PlayerMovement>();
   pm_player.audioPtr = &kingAudio;
   auto &light_player = player.add_component<Components::Light>(&laternLight);
   light_player.offset = {0, -5};

   // Flashlight entity
   auto &flashlight = scene->add_entity();
   auto &tr_flashlight = flashlight.add_component<Components::Transform>(
       Vec2<f32>{0.0f, 100.0f}, Vec2<f32>{1.0f, 2.5f}, 15.0f, Vec2<f32>{0.0f, -0.7f});
   auto &light_flashlight =
       flashlight.add_component<Components::Light>(&flashLight);
   flashlight.parentPtr = &player;
   pm_player.flashlightPtr = &flashlight;
   pm_player.flashAudioPtr = &flashSound;
   gm_manager.playerPtr = &player;

   // Knight0
   auto [knight0, kScript0] =
       create_knight(scene, knightPositions[0], &knightTexture, &knightAudio);

   auto [knight1, kScript1] =
       create_knight(scene, knightPositions[1], &knightTexture, &knightAudio);

   auto [knight2, kScript2] =
       create_knight(scene, knightPositions[2], &knightTexture, &knightAudio);

   auto [knight3, kScript3] =
       create_knight(scene, knightPositions[3], &knightTexture, &knightAudio);

   auto [knight4, kScript4] =
       create_knight(scene, knightPositions[4], &knightTexture, &knightAudio);

   AIManager::init(V2{5, 5},
                   {kScript0, kScript1, kScript2, kScript3, kScript4});

   // chessboard Entity
   auto &chess = scene->add_entity();
   auto &tr_chess = chess.add_component<Components::Transform>();
   auto &tm_chess =
       chess.add_component<Components::TileMap>(generate_map(9, 9), &tileSet);
   tm_chess.z = -20;
}