#include "matrix.hpp"

int main ()
{
    mtrx::Matrix<int> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    matrix.dump();

    return 0;
}
