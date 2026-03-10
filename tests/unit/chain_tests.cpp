#include "matrix_chain.hpp"

#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

// add() and size():

TEST (MatrixChainAdd, EmptyChain)
{
    mtrx::MatrixChain chain;

    EXPECT_EQ (chain.size(), 0);
}

TEST (MatrixChainAdd, SingleMatrix)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);

    EXPECT_EQ (chain.size(), 1);
}

TEST (MatrixChainAdd, ThreeMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);
    chain.add (5, 50);

    EXPECT_EQ (chain.size(), 3);
}

TEST (MatrixChainAdd, IncompatibleDimensions)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);

    EXPECT_THROW (chain.add (5, 4), std::invalid_argument);
}

// solve():

TEST (MatrixChainSolve, TwoMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 30);
    chain.add (30, 5);

    EXPECT_EQ (chain.solve(), 1500u);
}

TEST (MatrixChainSolve, ThreeMatricesOptimal)
{
    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);
    chain.add (5, 50);

    EXPECT_EQ (chain.solve(), 7500u);
}

TEST (MatrixChainSolve, FourMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 30);
    chain.add (30, 5);
    chain.add (5, 60);
    chain.add (60, 10);

    EXPECT_EQ (chain.solve(), 5000u);
}

TEST (MatrixChainSolve, CachedResult)
{
    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);
    chain.add (5, 50);

    uint64_t first  = chain.solve();
    uint64_t second = chain.solve();

    EXPECT_EQ (first, second);
}

TEST (MatrixChainSolve, CacheInvalidatedOnAdd)
{
    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);

    uint64_t before = chain.solve();

    chain.add (5, 50);

    uint64_t after = chain.solve();

    EXPECT_NE (before, after);
}

// naive_cost():

TEST (MatrixChainNaive, TwoMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 30);
    chain.add (30, 5);

    EXPECT_EQ (chain.naive_cost(), 1500u);
}

TEST (MatrixChainNaive, OptimalBetterThanNaive)
{
    // dims = [1, 10, 100, 1]
    // naive: (A0·A1)·A2 = 1*10*100 + 1*100*1 = 1000 + 100 = 1100
    // opt:   A0·(A1·A2) = 10*100*1 + 1*10*1  = 1000 + 10  = 1010
    mtrx::MatrixChain chain;
    chain.add (1, 10);
    chain.add (10, 100);
    chain.add (100, 1);

    EXPECT_GT (chain.naive_cost(), chain.solve());
}

// optimal_brackets():

TEST (MatrixChainBrackets, TwoMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 30);
    chain.add (30, 5);
    chain.solve();

    EXPECT_EQ (chain.optimal_brackets(), "(A0 A1)");
}

TEST (MatrixChainBrackets, ThreeMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);
    chain.add (5, 50);
    chain.solve();

    EXPECT_EQ (chain.optimal_brackets(), "((A0 A1) A2)");
}

TEST (MatrixChainBrackets, WithoutSolveThrows)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);
    chain.add (3, 4);

    EXPECT_THROW (chain.optimal_brackets(), std::logic_error);
}

// multiply():

TEST (MatrixChainMultiply, TwoMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);
    chain.add (3, 2);
    chain.solve();

    mtrx::Matrix<int> a = {{3, -1, 4}, {2, 5, -2}};
    mtrx::Matrix<int> b = {{7, 0}, {-3, 6}, {1, -5}};

    mtrx::Matrix<int> expected = {{28, -26}, {-3, 40}};

    std::vector<mtrx::Matrix<int>> mats = {a, b};
    EXPECT_TRUE (chain.multiply (mats) == expected);
}

TEST (MatrixChainMultiply, ThreeMatricesResult)
{
    mtrx::MatrixChain chain;
    chain.add (2, 2);
    chain.add (2, 2);
    chain.add (2, 2);
    chain.solve();

    mtrx::Matrix<int> e = {{1, 0}, {0, 1}};
    mtrx::Matrix<int> a  = {{-7, 11}, {4, 9}};

    // E * A * E == A
    std::vector<mtrx::Matrix<int>> mats = {e, a, e};
    EXPECT_TRUE (chain.multiply (mats) == a);
}

TEST (MatrixChainMultiply, WithoutSolveThrows)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);
    chain.add (3, 2);

    mtrx::Matrix<int> a = {{6, -2, 1}, {0, 3, 8}};
    mtrx::Matrix<int> b = {{5, 7}, {-4, 2}, {9, -6}};

    std::vector<mtrx::Matrix<int>> mats = {a, b};
    EXPECT_THROW (chain.multiply (mats), std::logic_error);
}

TEST (MatrixChainMultiply, WrongNumberOfMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);
    chain.add (3, 2);
    chain.solve();

    mtrx::Matrix<int> a = {{2, -9, 4}, {7, 1, -3}};

    std::vector<mtrx::Matrix<int>> mats = {a};
    EXPECT_THROW (chain.multiply (mats), std::invalid_argument);
}

// dims ctor:

TEST (MatrixChainDims, BasicConstruction)
{
    std::vector<size_t> dims = {10, 100, 5, 50};
    mtrx::MatrixChain chain (dims);

    EXPECT_EQ (chain.size(), 3);
}

TEST (MatrixChainDims, SolveMatchesAdd)
{
    std::vector<size_t> dims = {10, 30, 5, 60, 10};
    mtrx::MatrixChain chain (dims);

    mtrx::MatrixChain chain2;
    chain2.add (10, 30);
    chain2.add (30, 5);
    chain2.add (5, 60);
    chain2.add (60, 10);

    EXPECT_EQ (chain.solve(), chain2.solve());
}

TEST (MatrixChainDims, TooFewDimensions)
{
    std::vector<size_t> dims = {10};

    EXPECT_THROW (mtrx::MatrixChain chain (dims), std::invalid_argument);
}

// optimal_order():

TEST (MatrixChainOrder, TwoMatrices)
{
    mtrx::MatrixChain chain;
    chain.add (10, 30);
    chain.add (30, 5);
    chain.solve();

    std::vector<size_t> expected = {0};

    EXPECT_EQ (chain.optimal_order(), expected);
}

TEST (MatrixChainOrder, ThreeMatricesLeftAssoc)
{

    mtrx::MatrixChain chain;
    chain.add (10, 100);
    chain.add (100, 5);
    chain.add (5, 50);
    chain.solve();

    std::vector<size_t> expected = {0, 1};

    EXPECT_EQ (chain.optimal_order(), expected);
}

TEST (MatrixChainOrder, FourMatrices)
{
    std::vector<size_t> dims = {30, 35, 15, 5, 10};
    mtrx::MatrixChain chain (dims);
    chain.solve();

    std::vector<size_t> order = chain.optimal_order();
    std::vector<size_t> expected = {1, 0, 2};

    EXPECT_EQ (order.size(), 3);
    EXPECT_EQ (chain.optimal_order(), expected);
}

TEST (MatrixChainOrder, WithoutSolveThrows)
{
    mtrx::MatrixChain chain;
    chain.add (2, 3);
    chain.add (3, 4);

    EXPECT_THROW (chain.optimal_order(), std::logic_error);
}
