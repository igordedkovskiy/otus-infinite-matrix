// homework #5: infinite matrix.

#include <iostream>
#include "infinite_matrix.hpp"

int main()
{
    InfiniteMatrix<unsigned int, 0> matrix;
    for(std::size_t cntr = 0; cntr < 10; ++cntr)
        matrix[cntr][cntr] = cntr;
    for(std::size_t row = 0, col = 9; row < 10; ++row, --col)
        matrix[row][col] = col;

    std::cout << "\n[matrix[1][1] matrix[8][8]]:\n";
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
    return 0;
}
