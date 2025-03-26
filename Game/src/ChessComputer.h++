#pragma once
#include <Senpai>

const Senpai::Vec2<int> boardsize = {9,9};

bool in_range(Senpai::Vec2<int> virtualPos) {
   return virtualPos.x > 0 && virtualPos.x <= boardsize.x && virtualPos.y <= boardsize.y && virtualPos.y > 0;
}
using V2 = Senpai::Vec2<int>;

// bruteforce chess computer
class ChessComputer {
   private:
      

      struct ChessMove {
         int knightId;
         V2 target;
      };

      inline const static Vector<V2> kingMoves = Vector<V2>{V2{1,0}, V2{0,1}, V2{-1,0}, V2{0,-1}, V2{0,0}};
      inline const static Vector<V2> knightMoves = Vector<V2>{V2{1,2}, V2{2,1}, V2{2,-1}, V2{1,-2}, V2{-1,-2}, V2{-2,-1}, V2{-2,1}, V2{-1,2}};

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
         // all possible moves of the knights, for each king move, up to 5 × 5 × 8 = 200 possible moves
         Vector<Vector<UniquePtr<MoveNode>>> subNodes;

         bool has_sublayer() {
            return !subNodes.empty();
         }
      };

      static UniquePtr<MoveNode> moveTree;
      static int calculationDepth;

   public:
      // returns if the king sits in check
      static inline bool is_check(V2 king, Vector<V2> knights) {
         for(auto knight : knights) {
            if(king == knight) return true;
            V2 relative = knight - king ;
            // check if the knight is in a position to attack the king
            if((abs(relative.x) == 2 && abs(relative.y) == 1)
               || (abs(relative.x) == 1 && abs(relative.y) == 2)) {
               return true;
            }
         }
         return false;
      }

   private:
      /*
         get all possible moves of the king that do not result in a check. the fewer the moves the higher the score
      */
      static inline Vector<V2> get_all_possible_king_moves(V2 king, Vector<V2> knights) {
         // start with an empty vector
         Vector<V2> moves{};
         for(auto move : kingMoves) {
            V2 newKing = king + move;
            // if the king is in range and not in check, add the move to the vector
            if(in_range(newKing) && !is_check(newKing, knights)) {
               moves.push_back(newKing);
            }
         }
         return moves;
      }

      // scorefunction = xˣ
      inline static f32 score(uint possibleMoves) {
         if(possibleMoves >= 5) return 0;
         f32 x = 5.0 - possibleMoves;
         return pow(x,x);
      }

      static inline Vector<ChessMove> get_all_possible_knight_moves(Vector<V2> knights, V2 king) {
         // start with an empty vector
         Vector<ChessMove> moves{};
         for(int i = 0; i < static_cast<int>(knights.size()); i++) {
            V2 knight = knights[i];
            for(auto move : knightMoves) {
               V2 newKnight = knight + move;
               bool check = true;
               if(!in_range(newKnight) || newKnight == king) {
                  continue;
               } 
               for(auto knight : knights) {
                  if(newKnight == knight) {
                     check = false;
                     break;
                  }
               }
               if(check ) {
                  moves.push_back(ChessMove{i, newKnight});
               }
            }
         }
         return moves;
      }

      // create a new layer
      static inline MoveNode get_new_layer(ChessMove chessMove, Vector<V2> knights, V2 king) {
         MoveNode node;
         if(chessMove.knightId >= 0) knights[chessMove.knightId] = chessMove.target;
         node.chessMove = chessMove;
         node.king = king;
         // get all possible moves of the king
         node.kingMoves = get_all_possible_king_moves(king, knights);
         // get the score of the king moves
         node.score = score(node.kingMoves.size());
         // make the subNodes empty
         node.subNodes = {};
      }

      // 
      static inline void calculate_move_tree(MoveNode* Tree, int depth, Vector<V2> knights) {
         Tree->score = score(Tree->kingMoves.size());
         if(Tree->has_sublayer()) {
            for(auto& sublayer : Tree->subNodes) {
               for(auto& node : sublayer) {
                  calculate_move_tree(node.get(), depth-1, knights);
                  Tree->score += node->score;
               }
            } 
         } else {
            // reseve the memory for the knightMoves
            Tree->subNodes.reserve(Tree->kingMoves.size());
            for (auto kingMove : Tree->kingMoves) {
               Tree->subNodes.push_back({});
               auto& subsubNodes = Tree->subNodes.back();
               // reserve the memory for the knightMoves for each kingMove
               subsubNodes.reserve(40);
               for(auto knightMove : get_all_possible_knight_moves(knights, kingMove)) {
                  // get the next layer of the move
                  subsubNodes.push_back(make_unique<MoveNode>(get_new_layer(knightMove, knights, kingMove)));
                  Tree->score += subsubNodes.back()->score;

               }
            }
         }
      }

   public:
      // this has to be called to initialize the chess computer
      static void init(uint depth, V2 king, Vector<V2> knightspos) {
         knightsPositions = knightspos;
         calculationDepth = depth;
         moveTree = make_unique<MoveNode>(get_new_layer(ChessMove{-1,V2{0,0}}, knightspos, king));
         calculate_move_tree(moveTree.get(), depth, knightsPositions);
      }

   private:
      static void get_next_move(V2 kingPosition, Vector<V2> knightsPositions, ChessMove& result) {
         if(ChessComputer::knightsPositions != knightsPositions) {
            throw std::logic_error("The knights desynchronized");
         }

         for(int i = 0; i < moveTree->kingMoves.size(); i++) {
            ChessMove result;
            if(moveTree->kingMoves[i] == kingPosition) {
               // sort all options by score and return the best one
               moveTree->subNodes[i].sort([](UniquePtr<MoveNode> const &a, UniquePtr<MoveNode> const &b) {
                  return a->score > b->score;
               });
               // get a random move from gaussian distribution
               int index = floor(min(abs(Senpai::rng::get_gaussian()*4), static_cast<f32>(moveTree->subNodes[i].size()-1)));
               result = moveTree->subNodes[i][index]->chessMove;
               // do the move
               moveTree = move(moveTree->subNodes[i][index]);
               knightsPositions[result.knightId] = result.target;
               calculate_move_tree(moveTree.get(), calculationDepth, knightsPositions);
            }
         }
         throw std::logic_error("KingPosition not found in moveTree, Desync!");
      }

      static Senpai::Thread thread;
      static ChessMove nextMove;
   
   public:
      inline static bool compute(V2 kingPosition, Vector<V2> knightsPositions) {
         if(thread.is_running()) {
            throw std::logic_error("Thread is already running");
         }
         thread.execute([kingPosition, knightsPositions](){
            get_next_move(kingPosition, knightsPositions, nextMove);
         });
      }

      inline static bool has_finished() {
         return thread.is_finished();
      }

      inline static ChessMove get_result() {
         thread.join();
         return nextMove;
      }

      inline static void reset() {
         thread.join();
         moveTree = nullptr;
      }
};
