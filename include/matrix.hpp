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
        class MemoryGuard
        {
            T* ptr_;

        public:
            explicit MemoryGuard(size_t size) : ptr_(new T[size]) {}

            ~MemoryGuard() { delete[] ptr_; }

            T* get() const { return ptr_; }

            // transfers ownership to caller and returns pointer
            // after release guard no longer manages the memory
            T* release()
            {
                T* temp = ptr_;
                ptr_ = nullptr;

                return temp;
            }

            MemoryGuard(const MemoryGuard&) = delete;
            MemoryGuard& operator= (const MemoryGuard&) = delete;
        };

        T* data_;
        size_t num_cols_;
        size_t num_rows_;

        void copy_matrix (const Matrix& other)
        {
            num_cols_ = other.num_cols_;
            num_rows_ = other.num_rows_;

            MemoryGuard guard(num_cols_ * num_rows_);

            std::copy (other.data_, other.data_ + (num_cols_ * num_rows_), guard.get());

            data_ = guard.release();
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

            MemoryGuard guard(num_cols_ * num_rows_);

            std::fill (guard.get(), guard.get() + (num_cols_ * num_rows_), val);

            data_ = guard.release();
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

            MemoryGuard guard(num_cols_ * num_rows_);

            size_t index = 0;
            for (const auto& row : init)
                for (const auto& elem : row)
                    guard.get()[index++] = elem;

            data_ = guard.release();
        }

        template <typename It>
        Matrix(size_t cols, size_t rows, It start, It fin)
             : data_(nullptr),
               num_cols_(cols),
               num_rows_(rows)
        {
            if (cols == 0 || rows == 0)
                assert (0 && "Matrix cannot be empty");

            MemoryGuard guard(num_cols_ * num_rows_);

            It curr = start;
            size_t i = 0;

            for (; i < num_cols_ * num_rows_ && curr != fin; ++i, ++curr)
                guard.get()[i] = *curr;

            if (i != num_cols_ * num_rows_ || curr != fin)
                throw std::invalid_argument("Iterator range size doesnt match matrix dimensions");

            data_ = guard.release();
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
                Matrix temp(other);

                std::swap (data_, temp.data_);
                std::swap (num_cols_, temp.num_cols_);
                std::swap (num_rows_, temp.num_rows_);
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

        size_t ncols() const noexcept { return num_cols_; }
        size_t nrows() const noexcept { return num_rows_; }

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
                Matrix transposed{num_rows_, num_cols_};

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

        bool is_square() const noexcept
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

    private:
        size_t find_main_row (const Matrix& mt, size_t col) const
        {
            size_t max_row = col;
            auto max_data = std::abs (mt[col][col]);

            for (size_t row = col + 1; row < mt.nrows(); ++row)
            {
                auto abs_data = std::abs (mt[row][col]);
                if (abs_data > max_data)
                {
                    max_row = row;
                    max_data = abs_data;
                }
            }

            return max_row;
        }

        bool is_degenerate (const Matrix& mt, size_t col) const
        {
            return std::abs (mt[col][col]) < std::numeric_limits<T>::epsilon();
        }

        void swap_rows (Matrix& mt, size_t r1, size_t r2) const
        {
            for (size_t col = 0; col < mt.ncols(); ++col)
                std::swap (mt[r1][col], mt[r2][col]);
        }

        const T make_zeros_under_main_row (Matrix& mt, size_t main_row) const
        {
            const T main = mt[main_row][main_row];

            for (size_t row = main_row + 1; row < mt.nrows(); ++row)
            {
                const T coeff = mt[row][main_row] / main;

                for (size_t col = main_row + 1; col < mt.ncols(); ++col)
                    mt[row][col] -= mt[main_row][col] * coeff;
            }

            return main;
        }

    public:
        T det() const
        {
            if (!is_square())
                assert (0 && "Determinant can be find only for square matrix");

            Matrix copy_mt(*this);
            T det{1};

            size_t sign_count = 0;

            for (size_t row = 0; row < num_rows_; ++row)
            {
                size_t main_row = find_main_row (copy_mt, row);

                if (row != main_row)
                {
                    swap_rows (copy_mt, row, main_row);
                    ++sign_count;
                }

                if (is_degenerate (copy_mt, row))
                return T{0};

                det *= make_zeros_under_main_row (copy_mt, row);
            }

            return (sign_count % 2 == 0) ? det : -det;
        }
    };
}
