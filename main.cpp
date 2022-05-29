// homework #5: infinite matrix.

#include <iostream>
#include <cassert>
#include "infinite_matrix.hpp"

int main()
{
    InfiniteMatrix<int> m;
    auto a = m[0][0];
    ++a;
    m[0][0] = 123;
//    auto b = m[1];

    {
        InfiniteMatrix<int, -1> matrix;
        assert(matrix.size() == 0); // все ячейки свободны

        auto a = matrix[0][0];
        assert(a == -1);
        assert(matrix.size() == 0);
        matrix[100][100] = 314;
        assert(matrix[100][100] == 314);
        assert(matrix.size() == 1);
        matrix[100][100] = -1;
        assert(matrix.size() == 0);
    }
    return 0;
}
