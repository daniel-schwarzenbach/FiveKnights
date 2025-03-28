#include "../../Game/src/ChessComputer.h++"
#include "gtest/gtest.h"

TEST(ChessComputer, get_all_posssible_king_moves) {
   auto possible = chess_computer::get_all_possible_king_moves(
       {5, 5}, Vector<V2>{V2{1, 2}, V2{2, 1}, V2{2, 2}});
   EXPECT_EQ(possible.size(), 9);

   possible = chess_computer::get_all_possible_king_moves(
       {5, 5}, Vector<V2>{V2{1, 2}, V2{6, 7}, V2{2, 2}});
   EXPECT_EQ(possible.size(), 7);

   possible = chess_computer::get_all_possible_king_moves(
       {5, 5}, Vector<V2>{V2{1, 2}, V2{6, 7}, V2{2, 2}, V2{4, 4}});
   EXPECT_EQ(possible.size(), 4);

   possible = chess_computer::get_all_possible_king_moves(
       {5, 7}, Vector<V2>{V2{3, 6}, V2{7, 1}, V2{2, 9}, V2{3, 3}, V2{9, 8}});
   EXPECT_EQ(possible.size(), 7);

   possible = chess_computer::get_all_possible_king_moves(
       {4, 6}, Vector<V2>{V2{1, 9}, V2{5, 4}, V2{4, 9}, V2{1, 8}, V2{9, 3}});
   EXPECT_EQ(possible.size(), 5);
}