#pragma once

#include "matrix.hpp"
#include "matrix_chain.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace proc
{
    mtrx::Matrix<double> read_matrix (std::istream& input)
    {
        size_t k = 0;
        if (!(input >> k))
            throw std::runtime_error("Failed to read matrix size");

        mtrx::Matrix<double> matrix(k, k);

        for (size_t i = 0; i < k; i++)
            for (size_t j = 0; j < k; j++)
                if (!(input >> matrix[i][j]))
                    throw std::runtime_error("Failed to read matrix element");

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

    void process_chain (std::istream& input = std::cin,
                        std::ostream& output = std::cout)
    {
        size_t n = 0;
        if (!(input >> n) || n < 2)
            throw std::runtime_error ("Failed to read dimensions count");

        std::vector<size_t> dims (n);
        for (size_t i = 0; i < n; ++i)
            if (!(input >> dims[i]))
                throw std::runtime_error ("Failed to read dimension");

        mtrx::MatrixChain chain (dims);
        chain.solve();

        auto order = chain.optimal_order();
        for (size_t i = 0; i < order.size(); ++i)
        {
            if (i > 0)
                output << " ";
            output << order[i];
        }
        output << "\n";

        uint64_t naive = chain.naive_cost();
        uint64_t optimal = chain.solve();

        // single matrix: no operations, speedup is 1
        if (optimal == 0)
            output << 1 << "\n";
        else
            output << static_cast<double> (naive) / static_cast<double> (optimal) << "\n";
    }
}
