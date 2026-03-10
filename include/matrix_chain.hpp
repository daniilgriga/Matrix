#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "matrix.hpp"

namespace mtrx
{

    class MatrixChain
    {
    private:
        std::vector<size_t> dims_;
        std::vector<std::vector<uint64_t>> cost_;
        std::vector<std::vector<size_t>> split_;
        std::vector<std::vector<std::vector<size_t>>> order_;
        bool solved_;

        std::string brackets_helper (size_t i, size_t j) const
        {
            if (i == j)
                return "A" + std::to_string (i);

            size_t k = split_[i][j];
            return "(" + brackets_helper (i, k) + " " + brackets_helper (k + 1, j) + ")";
        }

        template<typename T>
        Matrix<T> multiply_helper (const std::vector<Matrix<T>>& matrices, size_t i, size_t j) const
        {
            if (i == j)
                return matrices[i];

            size_t k = split_[i][j];
            return multiply_helper (matrices, i, k) * multiply_helper (matrices, k + 1, j);
        }

    public:
        MatrixChain() : solved_(false) {}

        // ctor from dims array [p0, p1, ..., pn]
        explicit MatrixChain (const std::vector<size_t>& dims)
            : dims_(dims), solved_(false)
        {
            if (dims_.size() < 2)
                throw std::invalid_argument ("Need at least 2 dimensions for 1 matrix");

            for (size_t d : dims_)
                if (d == 0)
                    throw std::invalid_argument ("Dimensions must be positive");
        }

        void add (size_t rows, size_t cols)
        {
            if (rows == 0 || cols == 0)
                throw std::invalid_argument ("Matrix dimensions must be positive");

            if (!dims_.empty() && dims_.back() != rows)
                throw std::invalid_argument ("Incompatible matrix dimensions in chain");

            if (dims_.empty())
                dims_.push_back (rows);

            dims_.push_back (cols);
            solved_ = false;
        }

        size_t size() const
        {
            return dims_.size() > 0 ? dims_.size() - 1 : 0;
        }

        uint64_t solve()
        {
            size_t n = size();
            if (n == 0)
                throw std::logic_error ("Cannot solve empty matrix chain");

            if (solved_)
                return cost_[0][size() - 1];

            cost_.assign  (n, std::vector<uint64_t> (n, 0));
            split_.assign (n, std::vector<size_t>   (n, 0));
            order_.assign (n, std::vector<std::vector<size_t>> (n));

            for (size_t len = 2; len <= n; ++len)
            {
                for (size_t i = 0; i <= n - len; ++i)
                {
                    size_t j = i + len - 1;
                    cost_[i][j] = UINT64_MAX;

                    for (size_t k = i; k < j; ++k)
                    {
                        uint64_t c = cost_[i][k] + cost_[k + 1][j]
                                   + dims_[i] * dims_[k + 1] * dims_[j + 1];

                        // build candidate order: left + right + k
                        std::vector<size_t> candidate;
                        candidate.reserve (order_[i][k].size() + order_[k + 1][j].size() + 1);
                        candidate.insert (candidate.end(), order_[i][k].begin(), order_[i][k].end());
                        candidate.insert (candidate.end(), order_[k + 1][j].begin(), order_[k + 1][j].end());
                        candidate.push_back (k);

                        if (c < cost_[i][j] || (c == cost_[i][j] && candidate < order_[i][j]))
                        {
                            cost_[i][j] = c;
                            split_[i][j] = k;
                            order_[i][j] = std::move (candidate);
                        }
                    }
                }
            }

            solved_ = true;
            return cost_[0][n - 1];
        }

        uint64_t naive_cost() const
        {
            size_t n = size();
            if (n < 2)
                return 0;

            uint64_t cost = 0;
            size_t rows = dims_[0];
            for (size_t i = 1; i < n; ++i)
            {
                cost += rows * dims_[i] * dims_[i + 1];
                rows = dims_[0];
            }

            return cost;
        }

        std::string optimal_brackets() const
        {
            if (!solved_)
                throw std::logic_error ("Call solve() before optimal_brackets()");

            return brackets_helper (0, size() - 1);
        }

        std::vector<size_t> optimal_order() const
        {
            if (!solved_)
                throw std::logic_error ("Call solve() before optimal_order()");

            return order_[0][size() - 1];
        }

        template<typename T>
        Matrix<T> multiply (const std::vector<Matrix<T>>& matrices)
        {
            if (matrices.size() != size())
                throw std::invalid_argument ("Wrong number of matrices");
            if (!solved_)
                throw std::logic_error ("Call solve() before multiply()");

            return multiply_helper (matrices, 0, size() - 1);
        }
    };

}
