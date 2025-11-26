#pragma once

#include <iostream>
#include <cassert>
#include <iomanip>
#include <limits>

namespace mtrx
{
    template <typename T>
    class Matrix
    {
    private:
        T* data_;
        size_t num_cols_;
        size_t num_rows_;

        void copy_matrix (const Matrix& other)
        {
            num_cols_ = other.num_cols_;
            num_rows_ = other.num_rows_;

            data_ = new T[num_cols_ * num_rows_];
            std::copy (other.data_, other.data_ + (num_cols_ * num_rows_), data_);
        }

        static void check_column_bounds (size_t col, size_t row_size)
        {
            if (col >= row_size)
                assert (0 && "Column index out of range");
        }

        struct ProxyRow
        {
            T* row_;
            size_t row_size_;

            ProxyRow(T* r_ptr, size_t r_size) : row_(r_ptr), row_size_(r_size) {}

            const T& operator[](size_t c) const
            {
                check_column_bounds (c, row_size_);
                return row_[c];
            }

            T& operator[](size_t c)
            {
                check_column_bounds (c, row_size_);
                return row_[c];
            }
        };

        struct ConstProxyRow
        {
            const T* row_;
            size_t row_size_;

            ConstProxyRow(const T* r_ptr, size_t r_size) : row_(r_ptr), row_size_(r_size) {}

            const T& operator[](size_t c) const
            {
                check_column_bounds (c, row_size_);
                return row_[c];
            }
        };

    public:

        ProxyRow operator[](size_t r)
        {
            if (r >= num_rows_)
                assert (0 && "Row index out of range");

            return ProxyRow (data_ + r * num_cols_, num_cols_);
        }

        ConstProxyRow operator[](size_t r) const
        {
            if (r >= num_rows_)
                assert (0 && "Row index out of range");

            return ConstProxyRow (data_ + r * num_cols_, num_cols_);
        }

        Matrix(size_t cols, size_t rows, const T& val = T{})
             : data_(nullptr),
               num_cols_(cols),
               num_rows_(rows)
        {
            if (cols == 0 || rows == 0)
                assert (0 && "Matrix dimensions must be positive");

            data_ = new T[num_cols_ * num_rows_];

            std::fill (data_, data_ + (num_cols_ * num_rows_), val);
        }

        Matrix(std::initializer_list<std::initializer_list<T>> init)
                : data_(nullptr),
                  num_cols_(init.size() > 0 ? init.begin()->size() : 0),
                  num_rows_(init.size())
        {
            if (num_rows_ == 0 || num_cols_ == 0)
                assert (0 && "Matrix cannot be empty");

            for (const auto& row : init)
                if (row.size() != num_cols_)
                    assert (0 && "Rows sizes must be equal to num columns");

            data_ = new T[num_cols_ * num_rows_];
            size_t index = 0;
            for (const auto& row : init)
                for (const auto& elem : row)
                    data_[index++] = elem;
        }

        template <typename It>
        Matrix(size_t cols, size_t rows, It start, It fin)
             : data_(nullptr),
               num_cols_(cols),
               num_rows_(rows)
        {
            if (cols == 0 || rows == 0)
                assert (0 && "Matrix cannot be empty");

            data_ = new T[num_cols_ * num_rows_];

            It curr = start;
            size_t i = 0;

            for (; i < num_cols_ * num_rows_ && curr != fin; ++i, ++curr)
                data_[i] = *curr;

            if (i != num_cols_ * num_rows_ || curr != fin)
            {
                delete[] data_;
                assert (0 && "Iterator range size doesnt match matrix dimensions");
            }
        }

        Matrix(const Matrix& other) : data_(nullptr)
        {
            copy_matrix (other);
        }

        Matrix(Matrix&& other) noexcept
              : data_(other.data_),
                num_cols_(other.num_cols_),
                num_rows_(other.num_rows_) { other.data_ = nullptr; }

        Matrix& operator= (const Matrix& other)
        {
            if (this != &other)
            {
                delete[] data_;
                copy_matrix (other);
            }

            return *this;
        }

        Matrix& operator= (Matrix&& other) noexcept
        {
            if (this != &other)
            {
                delete[] data_;
                data_ = other.data_;
                num_cols_ = other.num_cols_;
                num_rows_ = other.num_rows_;

                other.data_ = nullptr;
            }
            return *this;
        }

        ~Matrix()
        {
            delete[] data_;
        }

        //====== METHODS ======//

        size_t ncols() const { return num_cols_; }
        size_t nrows() const { return num_rows_; }

        bool is_valid() const { return data_ != nullptr &&
                                       num_cols_ > 0    &&
                                       num_rows_ > 0; }

        void dump() const
        {
            for (size_t i = 0; i < num_rows_; ++i)
            {
                for (size_t j = 0; j < num_cols_; ++j)
                    std::cout << std::setw(4) << (*this)[i][j] << " ";

                std::cout << std::endl;
            }
        }

        Matrix& negate() &
        {
            for (size_t i = 0; i < num_rows_ * num_cols_; ++i)
                data_[i] = -data_[i];

            return *this;
        }

        Matrix& transpose() &
        {
            if (is_square())
            {
                for (size_t i = 0; i < num_rows_; ++i)
                    for (size_t j = i + 1; j < num_cols_; ++j)
                        std::swap ((*this)[i][j], (*this)[j][i]);
            }
            else
            {
                Matrix transposed{num_cols_, num_rows_};

                for (size_t i = 0; i < num_rows_; ++i)
                    for (size_t j = 0; j < num_cols_; ++j)
                        transposed[j][i] = (*this)[i][j];

                std::swap (*this, transposed);
            }

            return *this;
        }

        bool equal (const Matrix& other) const
        {
            if (num_rows_ != other.num_rows_ ||
                num_cols_ != other.num_cols_   )
                return false;

            return std::equal (data_,
                               data_ + num_rows_ * num_cols_,
                               other.data_);
        }

        bool is_square() const
        { return (num_rows_ == num_cols_); }

        T trace() const
        {
            if (!is_square())
                assert (0 && "Trace can be find only for square matrix");

            T sum{};

            for (size_t i = 0; i < num_rows_; ++i)
                sum += (*this)[i][i];

            return sum;
        }

        T det() const
        {
            if (!is_square())
                assert (0 && "Determinant can be find only for square matrix");

            Matrix copy(*this);
            T det{1};

            const size_t nr = num_rows_;

            for (size_t i = 0; i < nr; ++i)
            {
                size_t max_r = i;
                auto max_data = std::abs (copy[i][i]);

                for (size_t j = i + 1; j < nr; ++j)
                {
                    auto abs_data = std::abs (copy[j][i]);
                    if (abs_data > max_data)
                    {
                        max_r = j;
                        max_data = abs_data;
                    }
                }

                if (max_data < std::numeric_limits<T>::epsilon())
                    return T{0};

                if (max_r != i)
                {
                    for (size_t j = 0; j < nr; ++j)
                        std::swap (copy[i][j], copy[max_r][j]);

                    det = -det;
                }

                const T elem = copy[i][i];
                det *= elem;

                for (size_t j = i + 1; j < nr; ++j)
                {
                    const T coeff = copy[j][i] / elem;

                    for (size_t k = i + 1; k < nr; ++k)
                        copy[j][k] -= copy[i][k] * coeff;
                }
            }

            return det;
        }
    };
}
