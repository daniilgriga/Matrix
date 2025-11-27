#pragma once

#include "matrix.hpp"

#include <iostream>

namespace proc
{
    mtrx::Matrix<double> read_matrix (std::istream& input)
    {
        size_t k = 0;
        input >> k;

        mtrx::Matrix<double> matrix(k, k);

        for (size_t i = 0; i < k; i++)
            for (size_t j = 0; j < k; j++)
                input >> matrix[i][j];

        return matrix;
    }

    double calc_det (std::istream& input)
    {
        auto matrix = read_matrix (input);

        return matrix.det();
    }

    void process_det (std::istream& input = std::cin,
                      std::ostream& output = std::cout)
    {
        double det = calc_det (input);
        output << det << std::endl;
    }
}
