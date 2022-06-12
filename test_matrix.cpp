#include <vector>
#include "gtest/gtest.h"
#include "infinite_matrix.hpp"

TEST(TEST_MATRIX, task_example)
{
    InfiniteMatrix<int, 2, -1> matrix;
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
    InfiniteMatrix<unsigned int, 2, 0> matrix;
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

    for(const auto& it:matrix)
    {
        const auto [x,y,v] = it;
        if(v != matrix.default_value())
        {
            ASSERT_TRUE(v == matrix[x][y]);
        }
    }

    ASSERT_TRUE(matrix.size() == 18);
}

TEST(TEST_MATRIX, multidimensional)
{
    InfiniteMatrix<int, 3, -1> matrix;
    ASSERT_TRUE(matrix.size() == 0); // все ячейки свободны

    auto a = matrix[0][0][0];

    ASSERT_TRUE(a == -1);
    ASSERT_TRUE(matrix.size() == 0);
    matrix[100][100][200] = 314;
    ASSERT_TRUE(matrix[100][100][200] == 314);
    ASSERT_TRUE(matrix.size() == 1);

    ASSERT_TRUE(matrix[100][100][200] == 314);

    ((matrix[100][100][200] = 314) = 0) = 217;
    ASSERT_TRUE(matrix[100][100][200] == 217);

    ASSERT_TRUE(matrix.size() == 1);

    matrix[100][100][200] = -1;
    ASSERT_TRUE(matrix.size() == 0);
}

TEST(TEST_MATRIX, output_multidimensional)
{
    InfiniteMatrix<unsigned int, 3, 0> matrix;
    for(std::size_t cntr = 0; cntr < 10; ++cntr)
        matrix[cntr][cntr][cntr] = cntr;

    ASSERT_TRUE(matrix.size() == 9);
    ASSERT_TRUE(matrix[0][0][0] == 0);
    ASSERT_TRUE(matrix[1][1][1] == 1);
    ASSERT_TRUE(matrix[2][2][2] == 2);
    ASSERT_TRUE(matrix[3][3][3] == 3);
    ASSERT_TRUE(matrix[4][4][4] == 4);
    ASSERT_TRUE(matrix[5][5][5] == 5);
    ASSERT_TRUE(matrix[6][6][6] == 6);
    ASSERT_TRUE(matrix[7][7][7] == 7);
    ASSERT_TRUE(matrix[8][8][8] == 8);
    ASSERT_TRUE(matrix[9][9][9] == 9);

    for(std::size_t row = 0, col = 9, zdim = 0; row < 10; ++row, --col, ++zdim)
        matrix[row][col][zdim] = col;

    ASSERT_TRUE(matrix.size() == 18);
    ASSERT_TRUE(matrix[0][9][0] == 9);
    ASSERT_TRUE(matrix[1][8][1] == 8);
    ASSERT_TRUE(matrix[2][7][2] == 7);
    ASSERT_TRUE(matrix[3][6][3] == 6);
    ASSERT_TRUE(matrix[4][5][4] == 5);
    ASSERT_TRUE(matrix[5][4][5] == 4);
    ASSERT_TRUE(matrix[6][3][6] == 3);
    ASSERT_TRUE(matrix[7][2][7] == 2);
    ASSERT_TRUE(matrix[8][1][8] == 1);
    ASSERT_TRUE(matrix[9][0][9] == 0);

    ASSERT_TRUE(matrix.size() == 18);
    for(const auto& it:matrix)
    {
        const auto [x,y,z,v] = it;
        if(v != matrix.default_value())
        {
            ASSERT_TRUE(v == matrix[x][y][z]);
        }
    }
    ASSERT_TRUE(matrix.size() == 18);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
