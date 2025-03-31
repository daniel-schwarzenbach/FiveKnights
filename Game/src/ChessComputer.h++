#pragma once
#include <Senpai>

const Vec2<int> boardsize = {9, 9};

bool in_range(Vec2<int> virtualPos) {
   return virtualPos.x > 0 && virtualPos.x <= boardsize.x &&
          virtualPos.y <= boardsize.y && virtualPos.y > 0;
}
using V2 = Vec2<int>;

// bruteforce chess computer
namespace chess_computer {

struct ChessMove {
   int knightId;
   V2 target;

   void apply(Vector<V2> &knights) {
      if (knightId >= 0) knights[knightId] = target;
   }
};

inline const static Vector<V2> possibleKingMoves =
    Vector<V2>{V2{1, 0}, V2{0, 1},  V2{-1, 0}, V2{0, -1}, V2{0, 0},
               V2{1, 1}, V2{1, -1}, V2{-1, 1}, V2{-1, -1}};
inline const static Vector<V2> possibleKnightMoves =
    Vector<V2>{V2{1, 2},   V2{2, 1},   V2{2, -1}, V2{1, -2},
               V2{-1, -2}, V2{-2, -1}, V2{-2, 1}, V2{-1, 2}};

// current position of all the knigts
static Vector<V2> knightsPositions;

struct MoveNode {
   // the higher the score the better this move is
   f32 score;
   // the move the knight makes
   ChessMove chessMove;
   // position of the king after this moment
   V2 king;
   // all possible moves of the king
   Vector<V2> kingMoves;
   // all possible moves of the knights, for each king move, up to 5 × 5 × 8 =
   // 200 possible moves
   Deque<Deque<UniquePtr<MoveNode>>> subNodes;

#ifndef NDEBUG
   // debug the knights positions
   Vector<V2> knightsDebug;
   // debug the king position
   V2 kingDebug;
#endif

   bool has_sublayer() { return !subNodes.empty(); }
};

static UniquePtr<MoveNode> moveTree;
static int calculationDepth;

// returns if the king sits in check
static inline bool is_check(V2 king, Vector<V2> knights) {
   for (auto knight : knights) {
      if (king == knight) return true;
      V2 relative = knight - king;
      // check if the knight is in a position to attack the king
      if ((abs(relative.x) == 2 && abs(relative.y) == 1) ||
          (abs(relative.x) == 1 && abs(relative.y) == 2)) {
         return true;
      }
   }
   return false;
}

// returns the id of the knight that checks the king
static inline uint which_knight_checks(V2 king, Vector<V2> knights) {
   for (uint i = 0; i < knights.size(); i++) {
      if (king == knights[i]) return i;
      V2 relative = knights[i] - king;
      // check if the knight is in a position to attack the king
      if ((abs(relative.x) == 2 && abs(relative.y) == 1) ||
          (abs(relative.x) == 1 && abs(relative.y) == 2)) {
         return i;
      }
   }
   throw std::logic_error("No knight checks the king");
}

/*
   get all possible moves of the king that do not result in a check. the fewer
   the moves the higher the score
*/
static inline Vector<V2> get_all_possible_king_moves(V2 king,
                                                     Vector<V2> knights) {
   // start with an empty vector
   Vector<V2> moves{};
   for (auto move : possibleKingMoves) {
      V2 newKing = king + move;
      // if the king is in range and not in check, add the move to the vector
      if (in_range(newKing) && !is_check(newKing, knights)) {
         moves.push_back(newKing);
      }
   }
   return moves;
}

// scorefunction = xˣ
inline static f32 score(UInt possibleMoves) {
   if (possibleMoves >= possibleKingMoves.size()) return 0;
   f32 x = f32(possibleKingMoves.size()) - f32(possibleMoves);
   return std::pow(x, x);
}

static inline Vector<ChessMove> get_all_possible_knight_moves(
    Vector<V2> knights, V2 king) {
   // start with an empty vector
   Vector<ChessMove> moves{};
   // iterate over all knights by intex
   for (int i = 0; i < static_cast<int>(knights.size()); i++) {
      // get the knight
      V2 knight = knights[i];
      // iterate over all possible moves of the knight
      for (auto relMove : possibleKnightMoves) {
         // get the new position of the knight
         V2 newKnight = knight + relMove;
         bool check = true;
         // check if the knight is in range and not on the king
         if (!in_range(newKnight) || newKnight == king) {
            check = false;
            continue;
         }
         // check if the knightpostiong is on ontop of another knight
         for (auto knight : knights) {
            if (newKnight == knight) {
               check = false;
               break;
            }
         }
         if (check) {
            moves.push_back(ChessMove{i, newKnight});
         }
      }
   }
   return moves;
}

// create a new layer
static inline MoveNode get_new_layer(ChessMove chessMove, Vector<V2> knights,
                                     V2 king) {
   MoveNode node;
   node.chessMove = chessMove;
   chessMove.apply(knights);
   node.king = king;
   // get all possible moves of the king
   node.kingMoves.clear();
   node.kingMoves = get_all_possible_king_moves(king, knights);
   // get the score of the king moves
   node.score = score(node.kingMoves.size());
   // ensure the subNodes are empty
   node.subNodes.clear();
// debug the knights and king
#ifndef NDEBUG
   node.knightsDebug = knights;
   node.kingDebug = king;
#endif
   return node;
}

//
static inline void calculate_move_tree(MoveNode *Tree, int depth,
                                       Vector<V2> knights) {
   Tree->chessMove.apply(knights);
   debug_assert(Tree->knightsDebug == knights,
                "Knights desync on calculate_move_tree(...)-begin()");
   Tree->score = score(Tree->kingMoves.size());
   if (Tree->has_sublayer()) {
      for (auto &sublayer : Tree->subNodes) {
         for (auto &node : sublayer) {
            calculate_move_tree(node.get(), depth - 1, knights);
            Tree->score += node->score;
#ifndef NDEBUG
            auto copy = knights;
            node->chessMove.apply(copy);
            debug_assert(node->knightsDebug == copy,
                         "Knights desync on calculate_move_tree(...)-sublayer");
#endif
         }
      }
   } else if (depth > 0) {
      for (int i = 0; i < Tree->kingMoves.size(); i++) {
         auto kingMove = Tree->kingMoves[i];
         // init with an empty deque
         Tree->subNodes.push_back(Deque<UniquePtr<MoveNode>>());
         auto &subsubNodes = Tree->subNodes[i];
         // get all possible knight moves
         for (auto knightMove :
              get_all_possible_knight_moves(knights, kingMove)) {
            // get the next layer of the move
            subsubNodes.push_back(make_unique<MoveNode>(
                get_new_layer(knightMove, knights, kingMove)));
            // calculate the next layer
            calculate_move_tree(subsubNodes.back().get(), depth - 1, knights);
            Tree->score += subsubNodes.back()->score;
// debug the desync
#ifndef NDEBUG
            auto copy = knights;
            knightMove.apply(copy);
            debug_assert(subsubNodes.back()->knightsDebug == copy,
                         "Knights desync on calculate_move_tree(...)-newLayer");
#endif
         }
      }
   }
}

// sort the moves by score at the moveTree
static inline void sort_moves() {
   for (auto &nodes : moveTree->subNodes) {
      std::sort(nodes.begin(), nodes.end(),
                ([](UniquePtr<MoveNode> const &a, UniquePtr<MoveNode> const &b)
                     -> bool { return a->score > b->score; }));
   }
}

// the lower the difficulty the less random the moves
static inline f32 difficulty = 20.0f;
void increase_difficulty() { difficulty *= 0.9f; }

// this has to be called to initialize the chess computer
static void init(uint depth, V2 king, Vector<V2> knightspos) {
   if (knightsPositions != knightspos) {
      debug_log("KnightInput: " << knightspos);
      debug_log("KnightGot: " << knightsPositions);
   }
   knightsPositions = knightspos;
   calculationDepth = depth;
   f32 difficulty = 20.0f;
   moveTree = make_unique<MoveNode>(
       get_new_layer(ChessMove{-1, V2{0, 0}}, knightspos, king));
   debug_assert(moveTree->knightsDebug == knightsPositions,
                "Knights desync on init(...)");
}

static inline ChessMove nextMove;

static ChessMove get_next_move(V2 kingPosition, ChessMove &result) {
   debug_assert(moveTree->knightsDebug == knightsPositions,
                "KnightsDebug desync on get_next_move()");
   if (!moveTree->has_sublayer()) {
      calculate_move_tree(moveTree.get(), calculationDepth, knightsPositions);
   }

   for (int i = 0; i < moveTree->kingMoves.size(); i++) {
      if (moveTree->kingMoves[i] == kingPosition) {
         // sort all options by score and return the best one
         auto &mNodes = moveTree->subNodes[i];
         // get a random move from gaussian distribution
         int index = (int)floor(
             min(abs(Senpai::rng::get_gaussian() * difficulty),
                 static_cast<f32>(moveTree->subNodes[i].size() - 1)));
         result = moveTree->subNodes[i][index]->chessMove;
         // do the move
         moveTree = move(moveTree->subNodes[i][index]);
         // update the knight positions
         debug_log("KnightBefore: " << knightsPositions);
         debug_log("KnightMove: " << result.knightId << ", " << result.target);
         result.apply(knightsPositions);
         debug_log("KnightAfter: " << knightsPositions);
         nextMove = result;
         // unset the tree move
         moveTree->chessMove = {-1, V2{0, 0}};
         if (moveTree->kingDebug != kingPosition)
            debug_error("KingDebug desync on get_next_move()");
         return result;
      }
   }

   debug_error("KingInput: " << kingPosition);
   debug_error("KingMoves: " << moveTree->kingMoves);
   debug_error("King: " << moveTree->kingDebug);
   debug_error("Knights Positions: " << moveTree->knightsDebug);
   throw std::logic_error("KingPosition not found in moveTree, Desync!");
}

static inline Senpai::Thread thread;

inline static bool compute() {
   if (thread.is_running()) {
      throw std::logic_error("Thread is already running");
   }
   thread.execute([=]() {
      calculate_move_tree(moveTree.get(), calculationDepth, knightsPositions);
      sort_moves();
   });
   return true;
}

inline static bool has_finished() { return thread.is_finished(); }

inline static ChessMove get_result(V2 kingPos) {
   if (!thread.is_finished()) {
      debug_warning("AI is still computing!");
   }
   thread.join();
   get_next_move(kingPos, nextMove);
   compute();
   return nextMove;
}

inline static void reset() {
   thread.join();
   moveTree = nullptr;
}

};  // namespace chess_computer
