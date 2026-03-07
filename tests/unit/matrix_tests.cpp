#include "matrix.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
    // mixed relative/absolute tolerance for floating-point checks
    double scaled_tol (double lhs, double rhs, double rel = 1e-9, double abs = 1e-9)
    {
        return std::max (abs, std::max (std::abs (lhs), std::abs (rhs)) * rel);
    }

    long long permutation_sign (const std::vector<int>& perm)
    {
        size_t inversion_count = 0;

        for (size_t i = 0; i < perm.size(); ++i)
            for (size_t j = i + 1; j < perm.size(); ++j)
                if (perm[i] > perm[j])
                    ++inversion_count;

        return (inversion_count % 2 == 0) ? 1LL : -1LL;
    }

    // exact 5x5 determinant - leibniz formula
    long long det_reference_5x5 (const mtrx::Matrix<int>& mt)
    {
        const size_t n = mt.nrows();
        EXPECT_EQ (n, 5);
        EXPECT_EQ (mt.ncols(), 5);

        std::vector<int> perm (n);
        for (size_t i = 0; i < n; ++i)
            perm[i] = static_cast<int> (i);

        long long det = 0;
        do
        {
            long long term = permutation_sign (perm);
            for (size_t row = 0; row < n; ++row)
                term *= static_cast<long long> (mt[row][perm[row]]);

            det += term;
        }
        while (std::next_permutation (perm.begin(), perm.end()));

        return det;
    }
}

TEST (MatrixConstructor, SizeConstructorWithValue)
{
    mtrx::Matrix<double> m (3, 2, 5);

    EXPECT_EQ (m.ncols(), 3);
    EXPECT_EQ (m.nrows(), 2);

    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j)
            EXPECT_EQ (m[i][j], 5);
}

TEST (MatrixConstructor, SizeConstructorDefaultValue)
{
    mtrx::Matrix<double> m (4, 3);

    EXPECT_EQ (m.ncols(), 4);
    EXPECT_EQ (m.nrows(), 3);

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 4; ++j)
            EXPECT_EQ (m[i][j], 0);
}

TEST (MatrixConstructor, InitializerList2x2)
{
    mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    EXPECT_EQ (m.ncols(), 2);
    EXPECT_EQ (m.nrows(), 2);
    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][1], 2);
    EXPECT_EQ (m[1][0], 3);
    EXPECT_EQ (m[1][1], 4);
}

TEST (MatrixConstructor, InitializerList3x3)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    EXPECT_EQ (m.ncols(), 3);
    EXPECT_EQ (m.nrows(), 3);
    EXPECT_EQ (m[1][1], 5);
    EXPECT_EQ (m[2][2], 9);
}

TEST (MatrixConstructor, IteratorConstructor)
{
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    mtrx::Matrix<double> m (3, 2, data.begin(), data.end());

    EXPECT_EQ (m.ncols(), 3);
    EXPECT_EQ (m.nrows(), 2);
    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][2], 3);
    EXPECT_EQ (m[1][1], 5);
}

TEST (MatrixConstructor, IteratorConstructorWrongSize)
{
    std::vector<double> data = {1, 2, 3};

    EXPECT_THROW (
        mtrx::Matrix<double> m (2, 2, data.begin(), data.end()),
        std::invalid_argument
    );
}

TEST (MatrixConstructor, IteratorConstructorTooMany)
{
    std::vector<double> data = {1, 2, 3, 4, 5};

    EXPECT_THROW (
        mtrx::Matrix<double> m (2, 2, data.begin(), data.end()),
        std::invalid_argument
    );
}

TEST (MatrixConstructor, InitializerListUnequalRows)
{
    EXPECT_THROW (
        (mtrx::Matrix<double> {{1, 2, 3}, {4, 5}}),
        std::invalid_argument
    );
}

TEST (MatrixConstructor, SingleElement)
{
    mtrx::Matrix<double> m = {{42}};

    EXPECT_EQ (m.ncols(), 1);
    EXPECT_EQ (m.nrows(), 1);
    EXPECT_EQ (m[0][0], 42);
}

// big-5:

TEST (MatrixBig5, CopyConstructor)
{
    mtrx::Matrix<double> m1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    mtrx::Matrix<double> m2 (m1);  // copy ctor

    EXPECT_EQ (m2.ncols(), 3);
    EXPECT_EQ (m2.nrows(), 2);
    EXPECT_EQ (m2[0][1], 2);
    EXPECT_EQ (m2[1][2], 6);

    m2[0][0] = 999;
    EXPECT_EQ (m1[0][0], 1);
    EXPECT_EQ (m2[0][0], 999);
}

TEST (MatrixBig5, MoveConstructor)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 (std::move (m1));   // move ctor

    EXPECT_EQ (m2.ncols(), 2);
    EXPECT_EQ (m2.nrows(), 2);
    EXPECT_EQ (m2[0][0], 1);
    EXPECT_EQ (m2[1][1], 4);
}

TEST (MatrixBig5, CopyAssignment)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 = {
        {5, 6, 7},
        {8, 9, 10}
    };

    m2 = m1;

    EXPECT_EQ (m2.ncols(), 2);
    EXPECT_EQ (m2.nrows(), 2);
    EXPECT_EQ (m2[0][0], 1);
    EXPECT_EQ (m2[1][1], 4);

    m2[0][0] = 999;
    EXPECT_EQ (m1[0][0], 1);
    EXPECT_EQ (m2[0][0], 999);
}

TEST (MatrixBig5, MoveAssignment)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 = {
        {5, 6, 7}
    };

    m2 = std::move (m1);

    EXPECT_EQ (m2.ncols(), 2);
    EXPECT_EQ (m2.nrows(), 2);
    EXPECT_EQ (m2[0][0], 1);
    EXPECT_EQ (m2[1][1], 4);
}

TEST (MatrixBig5, SelfAssignment)
{
    mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    m = m;

    EXPECT_EQ (m.ncols(), 2);
    EXPECT_EQ (m.nrows(), 2);
    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[1][1], 4);
}

// accessoring:

TEST (MatrixAccessor, OperatorBracketRead)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][2], 3);
    EXPECT_EQ (m[1][1], 5);
}

TEST (MatrixAccessor, OperatorBracketWrite)
{
    mtrx::Matrix<double> m (3, 2, 0);

    m[0][0] = 10;
    m[1][2] = 20;

    EXPECT_EQ (m[0][0], 10);
    EXPECT_EQ (m[1][2], 20);
}

TEST (MatrixAccessor, ConstOperatorBracket)
{
    const mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[1][1], 4);
}

TEST (MatrixAccessor, ConstRowProxy)
{
    const mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    const auto row = m[0];
    EXPECT_EQ (row[1], 2);
}

TEST (MatrixAccessor, BoundaryAccess)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][2], 3);
    EXPECT_EQ (m[2][0], 7);
    EXPECT_EQ (m[2][2], 9);

    EXPECT_EQ (m.at (0, 0), 1);
    EXPECT_EQ (m.at (0, 2), 3);
    EXPECT_EQ (m.at (2, 0), 7);
    EXPECT_EQ (m.at (2, 2), 9);
}

TEST (MatrixAccessor, NcolsNrows)
{
    mtrx::Matrix<double> m (5, 3, 0);

    EXPECT_EQ (m.ncols(), 5);
    EXPECT_EQ (m.nrows(), 3);
}

TEST (MatrixAccessor, IsSquare)
{
    mtrx::Matrix<double> m1 (3, 3, 0);
    mtrx::Matrix<double> m2 (3, 4, 0);

    EXPECT_TRUE (m1.is_square());
    EXPECT_FALSE (m2.is_square());
}

// modifier:

TEST (MatrixModifier, DoubleNegate)
{
    mtrx::Matrix<double> m = {{1, -2}, {3, -4}};
    mtrx::Matrix<double> original (m);

    m.negate().negate();

    EXPECT_TRUE (m == original);
}

TEST (MatrixModifier, Negate)
{
    mtrx::Matrix<double> m = {
        {1, -2, 3},
        {-4, 5, -6}
    };

    m.negate();

    EXPECT_EQ (m[0][0], -1);
    EXPECT_EQ (m[0][1], 2);
    EXPECT_EQ (m[0][2], -3);
    EXPECT_EQ (m[1][0], 4);
    EXPECT_EQ (m[1][1], -5);
    EXPECT_EQ (m[1][2], 6);
}

TEST (MatrixModifier, TransposeSquare)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    m.transpose();

    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][1], 4);
    EXPECT_EQ (m[0][2], 7);
    EXPECT_EQ (m[1][0], 2);
    EXPECT_EQ (m[1][1], 5);
    EXPECT_EQ (m[2][0], 3);
}

TEST (MatrixModifier, TransposeNonSquare)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {4, 5, 6}
    };

    m.transpose();

    EXPECT_EQ (m.ncols(), 2);
    EXPECT_EQ (m.nrows(), 3);
    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[0][1], 4);
    EXPECT_EQ (m[1][0], 2);
    EXPECT_EQ (m[2][0], 3);
}

TEST (MatrixModifier, DoubleTranspose)
{
    mtrx::Matrix<double> m = {{1, 2, 3}, {4, 5, 6}};
    mtrx::Matrix<double> original (m);

    m.transpose().transpose();

    EXPECT_TRUE (m == original);
}

TEST (MatrixModifier, Transpose1x1)
{
    mtrx::Matrix<double> m = {{7}};
    m.transpose();

    EXPECT_EQ (m[0][0], 7);
    EXPECT_EQ (m.ncols(), 1);
    EXPECT_EQ (m.nrows(), 1);
}

// determinant:

TEST (MatrixDeterminant, Det1x1)
{
    mtrx::Matrix<double> m = {{5}};
    EXPECT_NEAR (m.det(), 5.0, scaled_tol (m.det(), 5.0));
}

TEST (MatrixDeterminant, NeedsPivoting)
{
    mtrx::Matrix<double> m = {
        {0, 1},
        {1, 0}
    };

    EXPECT_NEAR (m.det(), -1.0, scaled_tol (m.det(), -1.0));
}

TEST (MatrixDeterminant, NeedsPivoting3x3)
{
    mtrx::Matrix<double> m = {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    };

    EXPECT_NEAR (m.det(), -1.0, scaled_tol (m.det(), -1.0));
}

TEST (MatrixDeterminant, Generic3x3)
{
    mtrx::Matrix<double> m = {
        { 2,  3,  1},
        { 1, -1,  2},
        { 3,  2, -1}
    };

    EXPECT_NEAR (m.det(), 20.0, scaled_tol (m.det(), 20.0));
}

TEST (MatrixDeterminant, LargeRandom)
{
    constexpr size_t N = 10;
    std::mt19937 gen(123);
    std::uniform_real_distribution<double> small_dist(-3.0, 3.0);
    std::uniform_real_distribution<double> diag_dist(1.0, 3.0);

    // unit lower-triangular matrix => det(L) = 1
    mtrx::Matrix<double> L (N, N, 0.0);
    for (size_t i = 0; i < N; ++i)
    {
        L[i][i] = 1.0;
        for (size_t j = 0; j < i; ++j)
            L[i][j] = small_dist (gen);
    }

    // upper-triangular matrix => det(U) is product of diagonal
    mtrx::Matrix<double> U (N, N, 0.0);
    double expected_det = 1.0;
    for (size_t i = 0; i < N; ++i)
    {
        U[i][i] = diag_dist (gen);
        expected_det *= U[i][i];
        for (size_t j = i + 1; j < N; ++j)
            U[i][j] = small_dist (gen);
    }

    // det(L * U) = det(L) * det(U)
    mtrx::Matrix<double> A (N, N, 0.0);
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            for (size_t k = 0; k < N; ++k)
                A[i][j] = A[i][j] + L[i][k] * U[k][j];

    EXPECT_NEAR (A.det(), expected_det, scaled_tol (A.det(), expected_det));
}

TEST (MatrixDeterminant, RandomTransposeInvariant)
{
    constexpr size_t N = 6;

    // check: det(A) == det(A^T)
    for (uint32_t seed = 1; seed <= 20; ++seed)
    {
        std::mt19937 gen(seed);
        std::uniform_real_distribution<double> dist(-3.0, 3.0);

        mtrx::Matrix<double> mt (N, N, 0.0);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                mt[i][j] = dist (gen);

        mtrx::Matrix<double> transposed (mt);
        transposed.transpose();

        const double det_mt = mt.det();
        const double det_transposed = transposed.det();

        EXPECT_NEAR (det_mt, det_transposed, scaled_tol (det_mt, det_transposed, 1e-7, 1e-7));
    }
}

TEST (MatrixDeterminant, Identity2x2)
{
    mtrx::Matrix<double> m = {
        {1, 0},
        {0, 1}
    };

    EXPECT_NEAR (m.det(), 1.0, scaled_tol (m.det(), 1.0));
}

TEST (MatrixDeterminant, Simple2x2)
{
    mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    EXPECT_NEAR (m.det(), -2.0, scaled_tol (m.det(), -2.0));
}

TEST (MatrixDeterminant, Diagonal3x3)
{
    mtrx::Matrix<double> m = {
        {2, 0, 0},
        {0, 3, 0},
        {0, 0, 4}
    };

    EXPECT_NEAR (m.det(), 24.0, scaled_tol (m.det(), 24.0));
}

TEST (MatrixDeterminant, Singular3x3)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_NEAR (m.det(), 0.0, scaled_tol (m.det(), 0.0));
}

TEST (MatrixDeterminant, Identity5x5)
{
    mtrx::Matrix<double> m (5, 5, 0);
    for (size_t i = 0; i < 5; ++i)
        m[i][i] = 1;

    EXPECT_NEAR (m.det(), 1.0, scaled_tol (m.det(), 1.0));
}

TEST (MatrixDeterminant, UpperTriangular4x4)
{
    mtrx::Matrix<double> m = {
        {2, 3, 1, 5},
        {0, 4, 2, 7},
        {0, 0, 3, 6},
        {0, 0, 0, 5}
    };

    EXPECT_NEAR (m.det(), 120.0, scaled_tol (m.det(), 120.0));
}

// trace:

TEST (MatrixTrace, Identity3x3)
{
    mtrx::Matrix<double> m = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    EXPECT_EQ (m.trace(), 3);
}

TEST (MatrixTrace, Diagonal4x4)
{
    mtrx::Matrix<double> m (4, 4, 0);
    for (size_t i = 0; i < 4; ++i)
        m[i][i] = i + 1;

    EXPECT_EQ (m.trace(), 10);
}

TEST (MatrixTrace, Generic3x3)
{
    mtrx::Matrix<double> m = {
        {5, 2, 1},
        {3, 7, 4},
        {6, 8, 9}
    };

    EXPECT_EQ (m.trace(), 21);
}

// comparison:

TEST (MatrixComparison, EqualMatrices)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 = {
        {1, 2},
        {3, 4}
    };

    EXPECT_TRUE (m1 == m2);
}

TEST (MatrixComparison, DifferentMatrices)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 = {
        {1, 2},
        {3, 5}
    };

    EXPECT_FALSE (m1 == m2);
}

TEST (MatrixComparison, DifferentSizes)
{
    mtrx::Matrix<double> m1 = {
        {1, 2},
        {3, 4}
    };

    mtrx::Matrix<double> m2 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_FALSE (m1 == m2);
}

// exceptions:

TEST (MatrixExceptions, ZeroCols)
{
    EXPECT_THROW (
        mtrx::Matrix<double> m (0, 2),
        std::invalid_argument );
}

TEST (MatrixExceptions, ZeroRows)
{
    EXPECT_THROW (
        mtrx::Matrix<double> m (2, 0),
        std::invalid_argument );
}

TEST (MatrixExceptions, OutOfRangeRow)
{
    mtrx::Matrix<double> m (2, 2, 0.0);
    EXPECT_THROW ((void)m[2][0], std::out_of_range);
}

TEST (MatrixExceptions, OutOfRangeCol)
{
    mtrx::Matrix<double> m (2, 2, 0.0);
    EXPECT_THROW ((void)m[0][2], std::out_of_range);
}

TEST (MatrixExceptions, ConstOutOfRangeRow)
{
    const mtrx::Matrix<double> m = {{1, 2}, {3, 4}};
    EXPECT_THROW ((void)m[5][0], std::out_of_range);
}

TEST (MatrixExceptions, DetNonSquare)
{
    mtrx::Matrix<double> m = {{1, 2, 3}, {4, 5, 6}};
    EXPECT_THROW ((void)m.det(), std::invalid_argument);
}

TEST (MatrixExceptions, TraceNonSquare)
{
    mtrx::Matrix<double> m = {{1, 2, 3}, {4, 5, 6}};
    EXPECT_THROW ((void)m.trace(), std::invalid_argument);
}

TEST (MatrixExceptions, ConstOutOfRangeCol)
{
    const mtrx::Matrix<double> m = {{1, 2}, {3, 4}};
    EXPECT_THROW ((void)m[0][5], std::out_of_range);
}

TEST (MatrixExceptions, ConstAtOutOfRange)
{
    const mtrx::Matrix<double> m = {{1, 2}, {3, 4}};
    EXPECT_THROW ((void)m.at (2, 0), std::out_of_range);
    EXPECT_THROW ((void)m.at (0, 2), std::out_of_range);
}

// at()

TEST (MatrixAt, ReadAccess)
{
    mtrx::Matrix<double> m = {{1, 2}, {3, 4}};

    EXPECT_EQ (m.at (0, 0), 1);
    EXPECT_EQ (m.at (0, 1), 2);
    EXPECT_EQ (m.at (1, 0), 3);
    EXPECT_EQ (m.at (1, 1), 4);
}

TEST (MatrixAt, WriteAccess)
{
    mtrx::Matrix<double> m (2, 2, 0.0);

    m.at (0, 1) = 42;
    m.at (1, 0) = 99;

    EXPECT_EQ (m.at (0, 1), 42);
    EXPECT_EQ (m.at (1, 0), 99);
}

TEST (MatrixAt, ConstAccess)
{
    const mtrx::Matrix<double> m = {{5, 6}, {7, 8}};

    EXPECT_EQ (m.at (1, 1), 8);
}

TEST (MatrixAt, OutOfRange)
{
    mtrx::Matrix<double> m (2, 2, 0.0);

    EXPECT_THROW ((void)m.at (2, 0), std::out_of_range);
    EXPECT_THROW ((void)m.at (0, 2), std::out_of_range);
}

// Matrix<int> tests:

TEST (MatrixInt, SizeConstructor)
{
    mtrx::Matrix<int> m (3, 2, 7);

    EXPECT_EQ (m.ncols(), 3);
    EXPECT_EQ (m.nrows(), 2);

    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j)
            EXPECT_EQ (m[i][j], 7);
}

TEST (MatrixInt, InitializerList)
{
    mtrx::Matrix<int> m = {
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_EQ (m[0][0], 1);
    EXPECT_EQ (m[1][2], 6);
}

TEST (MatrixInt, CopyConstructor)
{
    mtrx::Matrix<int> m1 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m2 (m1);

    EXPECT_TRUE (m1 == m2);

    m2[0][0] = 999;
    EXPECT_EQ (m1[0][0], 1);
}

TEST (MatrixInt, MoveConstructor)
{
    mtrx::Matrix<int> m1 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m2 (std::move (m1));

    EXPECT_EQ (m2[0][0], 1);
    EXPECT_EQ (m2[1][1], 4);
}

TEST (MatrixInt, CopyAssignment)
{
    mtrx::Matrix<int> m1 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m2 = {{5, 6, 7}};

    m2 = m1;

    EXPECT_TRUE (m1 == m2);
}

TEST (MatrixInt, MoveAssignment)
{
    mtrx::Matrix<int> m1 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m2 = {{5, 6, 7}};

    m2 = std::move (m1);

    EXPECT_EQ (m2[1][1], 4);
}

TEST (MatrixInt, At)
{
    mtrx::Matrix<int> m = {{10, 20}, {30, 40}};

    EXPECT_EQ (m.at (0, 1), 20);

    m.at (1, 0) = 99;
    EXPECT_EQ (m.at (1, 0), 99);
}

TEST (MatrixInt, Negate)
{
    mtrx::Matrix<int> m = {{1, -2}, {3, -4}};
    m.negate();

    EXPECT_EQ (m[0][0], -1);
    EXPECT_EQ (m[0][1], 2);
    EXPECT_EQ (m[1][0], -3);
    EXPECT_EQ (m[1][1], 4);
}

TEST (MatrixInt, TransposeSquare)
{
    mtrx::Matrix<int> m = {{1, 2}, {3, 4}};
    m.transpose();

    EXPECT_EQ (m[0][1], 3);
    EXPECT_EQ (m[1][0], 2);
}

TEST (MatrixInt, TransposeNonSquare)
{
    mtrx::Matrix<int> m = {{1, 2, 3}, {4, 5, 6}};
    m.transpose();

    EXPECT_EQ (m.ncols(), 2);
    EXPECT_EQ (m.nrows(), 3);
    EXPECT_EQ (m[0][1], 4);
    EXPECT_EQ (m[2][0], 3);
}

TEST (MatrixInt, Trace)
{
    mtrx::Matrix<int> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    EXPECT_EQ (m.trace(), 15);
}

TEST (MatrixInt, Comparison)
{
    mtrx::Matrix<int> m1 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m2 = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> m3 = {{1, 2}, {3, 5}};

    EXPECT_TRUE (m1 == m2);
    EXPECT_FALSE (m1 == m3);
}

// multiply:

TEST (MatrixMultiply, Basic2x2)
{
    mtrx::Matrix<int> a = {{1, 2}, {3, 4}};
    mtrx::Matrix<int> b = {{5, 6}, {7, 8}};

    auto c = a * b;

    EXPECT_EQ (c.nrows(), 2);
    EXPECT_EQ (c.ncols(), 2);
    EXPECT_EQ (c[0][0], 19);
    EXPECT_EQ (c[0][1], 22);
    EXPECT_EQ (c[1][0], 43);
    EXPECT_EQ (c[1][1], 50);
}

TEST (MatrixMultiply, NonSquare)
{
    mtrx::Matrix<int> a = {{1, 2, 3}, {4, 5, 6}};
    mtrx::Matrix<int> b = {{7, 8}, {9, 10}, {11, 12}};

    auto c = a * b;

    EXPECT_EQ (c.nrows(), 2);
    EXPECT_EQ (c.ncols(), 2);
    EXPECT_EQ (c[0][0], 58);
    EXPECT_EQ (c[0][1], 64);
    EXPECT_EQ (c[1][0], 139);
    EXPECT_EQ (c[1][1], 154);
}

TEST (MatrixMultiply, ResultDimensions)
{
    mtrx::Matrix<int> a (4, 3, 1);  // 3x4
    mtrx::Matrix<int> b (2, 4, 1);  // 4x2

    auto c = a * b;

    EXPECT_EQ (c.nrows(), 3);
    EXPECT_EQ (c.ncols(), 2);
}

TEST (MatrixMultiply, IdentityRight)
{
    mtrx::Matrix<int> a = {{1, 2, 3}, {4, 5, 6}};

    mtrx::Matrix<int> I (3, 3, 0);
    for (size_t i = 0; i < 3; ++i)
        I[i][i] = 1;

    EXPECT_TRUE ((a * I) == a);
}

TEST (MatrixMultiply, IdentityLeft)
{
    mtrx::Matrix<int> a = {{1, 2, 3}, {4, 5, 6}};

    mtrx::Matrix<int> I (2, 2, 0);
    for (size_t i = 0; i < 2; ++i)
        I[i][i] = 1;

    EXPECT_TRUE ((I * a) == a);
}

TEST (MatrixMultiply, IncompatibleDimensions)
{
    mtrx::Matrix<int> a = {{1, 2, 3}, {4, 5, 6}};   // 2x3
    mtrx::Matrix<int> b = {{1, 2}, {3, 4}};         // 2x2

    EXPECT_THROW ((void)(a * b), std::invalid_argument);
}

TEST (MatrixMultiply, Associativity)
{
    // (A * B) * C == A * (B * C) for integer matrices
    std::mt19937 gen (77);
    std::uniform_int_distribution<int> dist (-5, 5);

    mtrx::Matrix<int> A (3, 2, 0);
    mtrx::Matrix<int> B (4, 3, 0);
    mtrx::Matrix<int> C (2, 4, 0);

    for (size_t i = 0; i < A.nrows(); ++i)
        for (size_t j = 0; j < A.ncols(); ++j)
            A[i][j] = dist (gen);

    for (size_t i = 0; i < B.nrows(); ++i)
        for (size_t j = 0; j < B.ncols(); ++j)
            B[i][j] = dist (gen);

    for (size_t i = 0; i < C.nrows(); ++i)
        for (size_t j = 0; j < C.ncols(); ++j)
            C[i][j] = dist (gen);

    auto lhs = (A * B) * C;
    auto rhs = A * (B * C);

    EXPECT_TRUE (lhs == rhs);
}

TEST (MatrixMultiply, DoubleBasic)
{
    mtrx::Matrix<double> a = {{1.5, 2.0}, {0.5, 3.0}};
    mtrx::Matrix<double> b = {{2.0, 0.0}, {1.0, 4.0}};

    auto c = a * b;

    EXPECT_NEAR (c[0][0], 5.0,  scaled_tol (c[0][0], 5.0));
    EXPECT_NEAR (c[0][1], 8.0,  scaled_tol (c[0][1], 8.0));
    EXPECT_NEAR (c[1][0], 4.0,  scaled_tol (c[1][0], 4.0));
    EXPECT_NEAR (c[1][1], 12.0, scaled_tol (c[1][1], 12.0));
}

TEST (MatrixInt, Det1x1)
{
    mtrx::Matrix<int> m = {{-7}};
    EXPECT_EQ (m.det(), -7);
}

TEST (MatrixInt, DetIdentity)
{
    mtrx::Matrix<int> m = {{1, 0}, {0, 1}};

    EXPECT_EQ (m.det(), 1);
}

TEST (MatrixInt, DetDiagonal)
{
    mtrx::Matrix<int> m = {
        {3, 0, 0},
        {0, 4, 0},
        {0, 0, 5}
    };

    EXPECT_EQ (m.det(), 60);
}

TEST (MatrixInt, DetSingular)
{
    mtrx::Matrix<int> m = {
        {1, 2, 3},
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_EQ (m.det(), 0);
}

TEST (MatrixInt, Det2x2)
{
    mtrx::Matrix<int> m = {{3, 8}, {4, 6}};
    EXPECT_EQ (m.det(), -14);
}

TEST (MatrixInt, DetNeedsPivoting)
{
    mtrx::Matrix<int> m = {
        {0, 1, 2},
        {1, 0, 3},
        {4, 0, 5}
    };

    EXPECT_EQ (m.det(), 7);
}

TEST (MatrixInt, DetNegativeElements)
{
    mtrx::Matrix<int> m = {
        {-2,  3,  1},
        { 5, -4, -2},
        { 1,  6, -3}
    };

    EXPECT_EQ (m.det(), 25);
}

TEST (MatrixInt, DetRegression6x6)
{
    mtrx::Matrix<int> m = {
        { 5,  6,  6,  5,  5,  2},
        {-6,  4,  6,  3, -1,  6},
        { 2,  5,  1, -3,  4, -3},
        { 2, -2, -6,  1, -4,  6},
        { 6, -3, -4,  2,  6,  6},
        { 5, -5,  4, -6, -3,  3}
    };

    EXPECT_EQ (m.det(), -473035);
}

TEST (MatrixInt, DetLargeRandom)
{
    constexpr size_t N = 10;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> small_dist(-3, 3);
    std::uniform_int_distribution<int> diag_dist(1, 3);

    mtrx::Matrix<int> L (N, N, 0);
    for (size_t i = 0; i < N; ++i)
    {
        L[i][i] = 1;
        for (size_t j = 0; j < i; ++j)
            L[i][j] = small_dist (gen);
    }

    mtrx::Matrix<int> U (N, N, 0);
    int expected_det = 1;
    for (size_t i = 0; i < N; ++i)
    {
        U[i][i] = diag_dist (gen);
        expected_det *= U[i][i];
        for (size_t j = i + 1; j < N; ++j)
            U[i][j] = small_dist (gen);
    }

    mtrx::Matrix<int> A (N, N, 0);
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            for (size_t k = 0; k < N; ++k)
                A[i][j] = A[i][j] + L[i][k] * U[k][j];

    EXPECT_EQ (A.det(), expected_det);
}

TEST (MatrixInt, DetLargeRandomBatch)
{
    constexpr size_t N = 8;

    // same known-det construction, but across many seeds
    for (uint32_t seed = 1; seed <= 40; ++seed)
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> small_dist(-3, 3);
        std::uniform_int_distribution<int> diag_dist(1, 3);

        mtrx::Matrix<int> L (N, N, 0);
        for (size_t i = 0; i < N; ++i)
        {
            L[i][i] = 1;
            for (size_t j = 0; j < i; ++j)
                L[i][j] = small_dist (gen);
        }

        mtrx::Matrix<int> U (N, N, 0);
        int expected_det = 1;
        for (size_t i = 0; i < N; ++i)
        {
            U[i][i] = diag_dist (gen);
            expected_det *= U[i][i];

            for (size_t j = i + 1; j < N; ++j)
                U[i][j] = small_dist (gen);
        }

        mtrx::Matrix<int> A (N, N, 0);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                for (size_t k = 0; k < N; ++k)
                    A[i][j] += L[i][k] * U[k][j];

        EXPECT_EQ (A.det(), expected_det);
    }
}

TEST (MatrixInt, DetSwapRowsFlipsSign)
{
    mtrx::Matrix<int> m = {
        {2, 1, 3},
        {1, 0, 4},
        {5, 2, 1}
    };

    mtrx::Matrix<int> swapped (m);
    for (size_t col = 0; col < swapped.ncols(); ++col)
        std::swap (swapped[0][col], swapped[2][col]);

    EXPECT_EQ (m.det(), 9);
    EXPECT_EQ (swapped.det(), -9);
}

TEST (MatrixInt, DetTransposeInvariantRandom)
{
    constexpr size_t N = 5;

    for (uint32_t seed = 100; seed <= 140; ++seed)
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(-3, 3);

        mtrx::Matrix<int> mt (N, N, 0);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                mt[i][j] = dist (gen);

        mtrx::Matrix<int> transposed (mt);
        transposed.transpose();

        EXPECT_EQ (mt.det(), transposed.det());
    }
}

TEST (MatrixInt, DetAgainstReference5x5Random)
{
    constexpr size_t N = 5;

    // compare bareiss result against independent exact oracle
    for (uint32_t seed = 200; seed <= 240; ++seed)
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(-4, 4);

        mtrx::Matrix<int> mt (N, N, 0);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                mt[i][j] = dist (gen);

        const long long reference_det = det_reference_5x5 (mt);

        ASSERT_GE (reference_det, static_cast<long long> (std::numeric_limits<int>::min()));
        ASSERT_LE (reference_det, static_cast<long long> (std::numeric_limits<int>::max()));

        EXPECT_EQ (mt.det(), static_cast<int> (reference_det));
    }
}
