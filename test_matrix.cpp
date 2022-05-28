#include <vector>
#include "gtest/gtest.h"
#include "infinite_matrix.hpp"

TEST(TEST_MATRIX, process_)
{
    InfiniteMatrix<int, -1> matrix;
    assert(matrix.size() == 0); // все ячейки свободны

    auto a = matrix[0][0];
    ASSERT_TRUE(a == -1);
    ASSERT_TRUE(matrix.size() == 0);
    matrix[100][100] = 314;
    ASSERT_TRUE(matrix[100][100] == 314);
    ASSERT_TRUE(matrix.size() == 1);

    // выведется одна строка
    // 100100314
    //for(auto c: matrix)
    //{
    //    int x,y,v;
    //    std::tie(x, y, v) = c;
    //    std::cout << x << y << v << std::endl;
    //}
    ASSERT_TRUE(false);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
