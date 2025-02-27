#include <gtest/gtest.h>
#include <Senpai.h++>


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

// TEST(Containers, Field){
//     Int n = 8;
//     Field<int,4> field = {1,2UL,3L,7*n};
//     cout << field << endl;
//     EXPECT_EQ(field[1], 1);
//     EXPECT_EQ(field[4], 56);
//     EXPECT_EQ(field.size(), 4);
// }

// TEST(Containers, Array){
//     // A = Int^(3 × 3)
//     Array<Int,2> A(3,3);
//     A   << 11 << 12 << 13
//         << 21 << 22 << 23
//         << 31 << 32 << 33;
    
//     cout << A << endl;
//     EXPECT_EQ((A[3,3]), 33);
//     EXPECT_EQ((A[1,3]), 13);
//     EXPECT_EQ(A.periodic(0,0), 33);

//     Array<Int, 3> B(2,2,2);
//     B   << 111 << 112 << 121 << 122 
//         << 211 << 212 << 221 << 222 ;
//     cout << B << endl;
//     EXPECT_EQ((B[2,1,2]), 212);
//     EXPECT_EQ((B[1,1,2]), 112);
//     EXPECT_EQ(B.periodic(0,0,0), 222);
//     Array<Int, 1> vec(7);
//     fill(vec, 6);
//     vec << 3 << 4 << 5 << 1;
//     cout << vec << endl;
//     vec.push_back(1); vec.push_back(1); vec.push_back(1); 
//     vec.push_back(1); vec.push_back(1); vec.push_front(1);
//     cout << vec << endl;
//     vec[1] = 9; vec[vec.size()] = 5;
//     cout << vec << endl;
//     EXPECT_EQ(vec.periodic(-1), 1);
//     EXPECT_EQ(vec.periodic(0), 5);
//     EXPECT_EQ(vec.periodic(vec.size()+1), 9);

//     const Vector<int> v{0,-1L,1U,0};
//     const Matrix<Int> R({2,2}, v);
//     cout << R << endl;
//     EXPECT_EQ((R[2,2]),0);
// }

// #include "../library/Utility/Linalg.h++"

// TEST(Linalg, get_and_set){
//     Matrix<int> A({3,3},{11,12,13,21,22,23,31,32,33});
//     Vector<int> row1 = get_row(A,1);
//     EXPECT_EQ(row1, (Vector<int>{11,12,13}));
//     Vector<int> col2 = get_col(A,2);
//     EXPECT_EQ(col2, (Vector<int>{12,22,32}));
//     set_col(A,1,col2);
//     set_row(A,2,row1);
//     cout << A;
// }
