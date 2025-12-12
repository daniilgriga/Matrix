#include "matrix.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>

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

// determinant:

TEST (MatrixDeterminant, Identity2x2)
{
    mtrx::Matrix<double> m = {
        {1, 0},
        {0, 1}
    };

    EXPECT_EQ (m.det(), 1);
}

TEST (MatrixDeterminant, Simple2x2)
{
    mtrx::Matrix<double> m = {
        {1, 2},
        {3, 4}
    };

    EXPECT_EQ (m.det(), -2);
}

TEST (MatrixDeterminant, Diagonal3x3)
{
    mtrx::Matrix<double> m = {
        {2, 0, 0},
        {0, 3, 0},
        {0, 0, 4}
    };

    EXPECT_EQ (m.det(), 24);
}

TEST (MatrixDeterminant, Singular3x3)
{
    mtrx::Matrix<double> m = {
        {1, 2, 3},
        {1, 2, 3},
        {4, 5, 6}
    };

    EXPECT_EQ (m.det(), 0);
}

TEST (MatrixDeterminant, Identity5x5)
{
    mtrx::Matrix<double> m (5, 5, 0);
    for (size_t i = 0; i < 5; ++i)
        m[i][i] = 1;

    EXPECT_EQ (m.det(), 1);
}

TEST (MatrixDeterminant, UpperTriangular4x4)
{
    mtrx::Matrix<double> m = {
        {2, 3, 1, 5},
        {0, 4, 2, 7},
        {0, 0, 3, 6},
        {0, 0, 0, 5}
    };

    EXPECT_EQ (m.det(), 120);
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

    EXPECT_TRUE (m1.equal (m2));
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

    EXPECT_FALSE (m1.equal (m2));
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

    EXPECT_FALSE (m1.equal (m2));
}
