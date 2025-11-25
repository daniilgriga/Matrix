#include "matrix.hpp"

int main ()
{
    mtrx::Matrix<int> matrix (2, 2, 5);

    matrix.dump();

    return 0;
}
