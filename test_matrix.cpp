#include <vector>
#include "gtest/gtest.h"
#include "infinite_matrix.hpp"

TEST(TEST_MATRIX, task_example)
{
    InfiniteMatrix<int, -1> matrix;
    ASSERT_TRUE(matrix.size() == 0); // все ячейки свободны

    auto a = matrix[0][0];
    ASSERT_TRUE(a == -1);
    ASSERT_TRUE(matrix.size() == 0);
    matrix[100][100] = 314;
    ASSERT_TRUE(matrix[100][100] == 314);
    ASSERT_TRUE(matrix.size() == 1);

    ASSERT_TRUE(matrix[100][100] == 314);

    ((matrix[100][100] = 314) = 0) = 217;
    ASSERT_TRUE(matrix[100][100] == 217);

    ASSERT_TRUE(matrix.size() == 1);

    matrix[100][100] = -1;
    ASSERT_TRUE(matrix.size() == 0);
}

TEST(TEST_MATRIX, task_output)
{
    InfiniteMatrix<unsigned int, 0> matrix;
    for(std::size_t cntr = 0; cntr < 10; ++cntr)
        matrix[cntr][cntr] = cntr;
    ASSERT_TRUE(matrix.size() == 9);
    for(std::size_t row = 0, col = 9; row < 10; ++row, --col)
        matrix[row][col] = col;

    ASSERT_TRUE(matrix.size() == 18);

    ASSERT_TRUE(matrix[0][0] == 0);
    ASSERT_TRUE(matrix[1][1] == 1);
    ASSERT_TRUE(matrix[2][2] == 2);
    ASSERT_TRUE(matrix[3][3] == 3);
    ASSERT_TRUE(matrix[4][4] == 4);
    ASSERT_TRUE(matrix[5][5] == 5);
    ASSERT_TRUE(matrix[6][6] == 6);
    ASSERT_TRUE(matrix[7][7] == 7);
    ASSERT_TRUE(matrix[8][8] == 8);
    ASSERT_TRUE(matrix[9][9] == 9);

    ASSERT_TRUE(matrix[0][9] == 9);
    ASSERT_TRUE(matrix[1][8] == 8);
    ASSERT_TRUE(matrix[2][7] == 7);
    ASSERT_TRUE(matrix[3][6] == 6);
    ASSERT_TRUE(matrix[4][5] == 5);
    ASSERT_TRUE(matrix[5][4] == 4);
    ASSERT_TRUE(matrix[6][3] == 3);
    ASSERT_TRUE(matrix[7][2] == 2);
    ASSERT_TRUE(matrix[8][1] == 1);
    ASSERT_TRUE(matrix[9][0] == 0);

    ASSERT_TRUE(matrix.size() == 18);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
