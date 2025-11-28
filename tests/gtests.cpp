#include "matrix.hpp"
#include "processor.hpp"

#include <gtest/gtest.h>
#include <sstream>

double calc_det_helper (const std::string& data)
{
    std::istringstream istr (data);
    return proc::calc_det (istr);
}

TEST (CalcDetTest, SmallZeroMatrix)
{
    std::string data = "2 0 0"
                        " 0 0";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (0.0, det);
}

TEST (CalcDetTest, BigZeroMatrix)
{
    std::string data = "5 0 0 0 0 0"
                        " 0 0 0 0 0"
                        " 0 0 0 0 0"
                        " 0 0 0 0 0"
                        " 0 0 0 0 0";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (0.0, det);
}

TEST (CalcDetTest, ThreeByThreeIdentityMatrix)
{
    std::string data = "3 1 0 0"
                        " 0 1 0"
                        " 0 0 1";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (1.0, det);
}

TEST (CalcDetTest, FourByFourIdentityMatrix)
{
    std::string data = "4 1 0 0 0"
                        " 0 1 0 0"
                        " 0 0 1 0"
                        " 0 0 0 1";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (1.0, det);
}

TEST (CalcDetTest, FiveByFiveIdentityMatrix)
{
    std::string data = "5 1 0 0 0 0"
                        " 0 1 0 0 0"
                        " 0 0 1 0 0"
                        " 0 0 0 1 0"
                        " 0 0 0 0 1";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (1.0, det);
}

TEST (CalcDetTest, OneElemMatrix)
{
    std::string data = "1 7";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (7.0, det);
}

TEST (CalcDetTest, TwoByTwoMatrix)
{
    std::string data = "2 2 5"
                        " 4 8";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (-4.0, det);
}

TEST (CalcDetTest, ThreeByThreeMatrix)
{
    std::string data = "3 7  1  1"
                        " 19 6  5"
                        " 2  11 3";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (-109.0, det);
}

TEST (CalcDetTest, FourByFourMatrix)
{
    std::string data = "4 2  1  7 3"
                        " 12 11 8 6"
                        " 3  9  1 5"
                        " 6  5  1 12";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (4421.0, det);
}

TEST (CalcDetTest, FiveByFiveMatrix)
{
    std::string data = "5 4  13 -11 3  3"
                        " 3  4   3  4 16"
                        " 3  6   7  6  7"
                        " -9 5   3  1  9"
                        " 2  5   6  8 -3";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (73445.0, det);
}

TEST (CalcDetTest, SixBySixMatrix)
{
    std::string data = "6 4  3  -12 4  3 -2"
                        " 3  7   3  4 16 -2"
                        " 3  6   7  7  7 -6"
                        " -9 7   1  1  9  3"
                        " 9  3   6  5 -3 -9"
                        " -9 -2 -4  9 11  2";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (106614.0, det);
}

TEST (CalcDetTest, SevenBySevenMatrix)
{
    std::string data = "7  6   4 -17  4  1 -8  1"
                        "  6   3   5  3 11 -1  3"
                        "  5   4   4  2  3 -5  4"
                        " -2   3   5  4  4  6 -3"
                        "  1   2   5  7 -3 -9 -4"
                        " -4  -5  -6  4 16  3  3"
                        "  8  -6  -3 -6  6  4  1";

    double det = calc_det_helper (data);
    EXPECT_DOUBLE_EQ (-2784800.0, det);
}
