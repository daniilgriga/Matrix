#pragma once

#include <iostream>
#include <cassert>

namespace mtrx
{
    template <typename T>
    class Matrix
    {
    private:
        T* data;
        int num_cols;
        int num_rows;

        void copy_matrix (const Matrix& other)
        {
            num_cols = other.num_cols;
            num_rows = other.num_rows;

            data = new T[num_cols * num_rows];
            std::copy (other.data, other.data + (num_cols * num_rows), data);
        }

    public:
        Matrix(int cols, int rows, const T& val = T{})
            : data(nullptr),
              num_cols(cols),
              num_rows(rows)
        {
            if (cols <= 0 || rows <= 0)
                assert (0 && "Matrix dimensions must be positive");

            data = new T [num_cols * num_rows];

            std::fill (data, data + (num_cols * num_rows), val);
        }

        Matrix(const Matrix& other) : data(nullptr)
        {
            copy_matrix (other);
        }

        Matrix(Matrix&& other) noexcept
              : data(other.data),
                num_cols(other.num_cols),
                num_rows(other.num_rows) { other.data = nullptr; }

        Matrix& operator= (const Matrix& other)
        {
            if (this != &other)
            {
                delete[] data;
                copy_matrix (other);
            }

            return *this;
        }

        Matrix& operator= (Matrix&& other) noexcept
        {
            if (this != &other)
            {
                delete[] data;
                data = other.data;
                num_cols = other.num_cols;
                num_rows = other.num_rows;

                other.data = nullptr;
            }
            return *this;
        }

        ~Matrix()
        {
            delete[] data;
        }

        int ncols() const { return num_cols; }
        int nrows() const { return num_rows; }

        bool is_valid() const { return data != nullptr &&
                                       num_cols > 0    &&
                                       num_rows > 0; }

        void dump() const
        {
            for (int i = 0; i < num_rows; ++i)
            {
                for (int j = 0; j < num_cols; ++j)
                    std::cout << data[i * num_cols + j] << " ";

                std::cout << std::endl;
            }
        }
    };
}
