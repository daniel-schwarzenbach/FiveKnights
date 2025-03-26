#include <gtest/gtest.h>
#include <Senpai>

TEST(Containers, Vector){
   Vector<Int> vec1 = {1,2,3,4,5};
   std::stringstream ss;
   ss << vec1;
   Vector<Int> vec2;
   ss >> vec2;
   EXPECT_TRUE(vec1 == vec2);
   cout << vec1 << endl;
   cout << vec2 << endl;
}

TEST(Containers, Vec2){
   using Senpai::Vec2;
    Vec2<Int> vec1{1,2};
    Vec2<Int> vec2{3,4};
    Vec2<Int> vec3 = vec1 + vec2;
    EXPECT_EQ(vec3.x,4);
    EXPECT_EQ(vec3.y,6);
    vec3 = vec1 - vec2;
    EXPECT_EQ(vec3.x,-2);
    EXPECT_EQ(vec3.y,-2);
    vec3 = vec1 * vec2;
    EXPECT_EQ(vec3.x,3);
    EXPECT_EQ(vec3.y,8);
    vec3 = vec1 / vec2;
    EXPECT_EQ(vec3.x,0);
    EXPECT_EQ(vec3.y,0);
    vec3 = vec1 + 2;
    EXPECT_EQ(vec3.x,3);
    EXPECT_EQ(vec3.y,4);
    vec3 = vec1 - 2;
    EXPECT_EQ(vec3.x,-1);
    EXPECT_EQ(vec3.y,0);
    vec3 = vec1 * 2;
    EXPECT_EQ(vec3.x,2);
    EXPECT_EQ(vec3.y,4);
    vec3 = vec1 / 2;
    EXPECT_EQ(vec3.x,0);
    EXPECT_EQ(vec3.y,1);
    vec3 += vec1;
    EXPECT_EQ(vec3.x,1);
    EXPECT_EQ(vec3.y,3);
    vec3 -= vec1;
    EXPECT_EQ(vec3.x,0);
    EXPECT_EQ(vec3.y,1);
    vec3 *= vec1;
    EXPECT_EQ(vec3.x,0);
    EXPECT_EQ(vec3.y,2);
    vec3 /= vec1;
    EXPECT_EQ(vec3.x,0);
    EXPECT_EQ(vec3.y,1);
}


TEST(Containers, Range){
   for (Int i : Range{1,10}){
      cout << i << " ";
   }
   cout << endl;
   for (Int i : Range{5,1}){
      cout << i << " ";
   }
   cout << endl;
   Range range1{1,10};
   EXPECT_TRUE(range1.contains(2));
   EXPECT_FALSE(range1.contains(0));
   EXPECT_FALSE(range1.contains(11));
   EXPECT_EQ(range1.size(),10);

   Range range2{8,5};
   EXPECT_TRUE(range2.contains(5));
   EXPECT_FALSE(range2.contains(4));
   EXPECT_FALSE(range2.contains(9));
   EXPECT_EQ(range2.size(),4);

   cout << range1 << endl;
   cout << range2 << endl;
   EXPECT_EQ(range2[2],7);
   EXPECT_EQ(range2[1],8);
   EXPECT_EQ(range1[2],2);
}

TEST(Containers, Matrix) {
   Matrix<f32> A = {{1, 2, 3}, {7, 8, 9}};
   Matrix<f32> B;
   cout << A << endl;
   std::stringstream ss;
   ss << A;
   ss >> B;
   EXPECT_TRUE(A == B);
   Matrix<int> C(8,8);
   bool isWhite = true;
   for (auto& c : C) {
      c = Senpai::rng::get_in_range(0, 7);
      if(isWhite) {
         c += 8;
      }
      isWhite = !isWhite;
   }
   cout << C << endl;
}