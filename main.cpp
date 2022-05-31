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

        // выведется одна строка
        // 100100314
        for(const auto& it:matrix)
        {
            int x,y,v;
            std::tie(x, y, v) = it;
//            const auto& [x,y,v] = it;
            std::cout << x << y << v << std::endl;
        }

        assert(matrix[100][100] == 314);
        assert(matrix.size() == 1);

        matrix[100][100] = -1;
        assert(matrix.size() == 0);
    }
    {
        InfiniteMatrix<unsigned int, 0> matrix;
        for(std::size_t cntr = 0; cntr < 10; ++cntr)
            matrix[cntr][cntr] = cntr;
        assert(matrix.size() == 9);
        for(std::size_t row = 0, col = 9; row < 10; ++row, --col)
            matrix[row][col] = col;

        assert(matrix.size() == 18);

        assert(matrix[0][0] == 0);
        assert(matrix[1][1] == 1);
        assert(matrix[2][2] == 2);
        assert(matrix[3][3] == 3);
        assert(matrix[4][4] == 4);
        assert(matrix[5][5] == 5);
        assert(matrix[6][6] == 6);
        assert(matrix[7][7] == 7);
        assert(matrix[8][8] == 8);
        assert(matrix[9][9] == 9);

        assert(matrix[0][9] == 9);
        assert(matrix[1][8] == 8);
        assert(matrix[2][7] == 7);
        assert(matrix[3][6] == 6);
        assert(matrix[4][5] == 5);
        assert(matrix[5][4] == 4);
        assert(matrix[6][3] == 3);
        assert(matrix[7][2] == 2);
        assert(matrix[8][1] == 1);
        assert(matrix[9][0] == 0);

        assert(matrix.size() == 18);

        std::cout << "\n[matrix[1][1] : matrix[8][8]]:\n";
        for(std::size_t row = 1; row < 9; ++row)
        {
            for(std::size_t col = 1; col < 9; ++col)
                std::cout << matrix[row][col] << ' ';
            std::cout << '\n';
        }

        std::cout << "\nNumber of occuppied cells is " << matrix.size() << '\n';

        std::cout << "\nOccuppied cells and it's indices:\n";
        for(const auto& it:matrix)
        {
            const auto [x,y,v] = it;
            //unsigned int x,y,v;
            //std::tie(x, y, v) = it;
            if(v != matrix.default_value())
                std::cout << "matrix[" << x << "][" << y << "] = " << v << std::endl;
        }
    }
    return 0;
}
