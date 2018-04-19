//
// Created by haochuanchen on 18-4-18.
//

#include "../src/curve/util/BSplineFunction.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace std;

namespace // BSplineFunction A2.1
{
TEST(BSplineFunction_A_2_1, inner1)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_EQ(4, bs.find_span(2.5));
}

TEST(BSplineFunction_A_2_1, inner2)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_EQ(4, bs.find_span(2));
}

TEST(BSplineFunction_A_2_1, inner3)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_EQ(7, bs.find_span(4));}

TEST(BSplineFunction_A_2_1, right)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_EQ(8, bs.find_span(5));
}

TEST(BSplineFunction_A_2_1, out_left)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_THROW(bs.find_span(-1), std::out_of_range);
}

TEST(BSplineFunction_A_2_1, out_right)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    auto p = 2;
    long n = U.size() - p - 1;

    auto bs = BSplineFunction<double>(n, p, U);

    EXPECT_THROW(bs.find_span(6), std::out_of_range);
}
}

namespace // BSplineFunction A2.2
{
TEST(BSplineFunction_A_2_2, case_degree_2)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    int p = 2;
    long n = U.size() - p - 1;
    auto u = 2.5;

    auto bs = BSplineFunction<double>(n, p, U);

    auto i = bs.find_span(u);
    double N[p + 1];

    double EN[] = { 1.0 / 8.0, 6.0 / 8.0, 1.0 / 8.0 };

    bs.basis_funcs(i, u, N);

    for (int j = 0; j <= p; j++)
    {
        EXPECT_DOUBLE_EQ(N[j], EN[j]);
    }
}

TEST(BSplineFunction_A_2_2, case_degree_1)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    int p = 1;
    long n = U.size() - p - 1;
    auto u = 2.5;

    auto bs = BSplineFunction<double>(n, p, U);

    auto i = bs.find_span(u);

    double N[p + 1];

    double EN[] = { 1.0 / 2.0, 1.0 / 2.0};

    bs.basis_funcs(i, u, N);

    for (int j = 0; j <= p; j++)
    {
        EXPECT_DOUBLE_EQ(N[j], EN[j]);
    }
}

TEST(BSplineFunction_A_2_2, case_bezier)
{
    vector<double> U = { 0, 0, 1, 1 };
     int p = 1;
    long n = U.size() - p - 1;
    auto u = 0.5;

    auto bs = BSplineFunction<double>(n, p, U);

    auto i = bs.find_span(u);

    double N[p + 1];

    double EN[] = { 0.5, 0.5 };

    bs.basis_funcs(i, u, N);

    for (int j = 0; j <= p; j++)
    {
        EXPECT_DOUBLE_EQ(N[j], EN[j]);
    }
}
}

namespace // BSplineFunction A2.3
{
TEST(BSplineFunction_A_2_3, case_degree_2)
{
    vector<double> U = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
     int p = 2;
    long n = U.size() - p - 1;
    auto u = 2.5;

     int der_order = 2;

    auto ders = new double*[der_order + 1];
    for (int i = 0; i < der_order + 1; i++)
    {
        ders[i] = new double[p + 1];
    }

    auto bs = BSplineFunction<double>(n, p, U);

    auto i = bs.find_span(u);

    bs.ders_basis_funcs(i, u, der_order, ders);

    EXPECT_DOUBLE_EQ(0.125, ders[0][0]);
    EXPECT_DOUBLE_EQ(-0.5, ders[1][0]);
    EXPECT_DOUBLE_EQ(1, ders[2][0]);

    EXPECT_DOUBLE_EQ(0.750, ders[0][1]);
    EXPECT_DOUBLE_EQ(0.0, ders[1][1]);
    EXPECT_DOUBLE_EQ(-2.0, ders[2][1]);

    EXPECT_DOUBLE_EQ(0.125, ders[0][2]);
    EXPECT_DOUBLE_EQ(0.5, ders[1][2]);
    EXPECT_DOUBLE_EQ(1.0, ders[2][2]);
}

TEST(BSplineFunction_A_2_3, case_degree_3)
{
    vector<double> U = { 0, 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5, 5};
    int p = 3;
    long n = U.size() - p - 1;
    auto u = 2.5;

     int der_order = 3;

    auto ders = new double*[der_order + 1];
    for (int i = 0; i < der_order + 1; i++)
    {
        ders[i] = new double[p + 1];
    }

    auto bs = BSplineFunction<double>(n, p, U);

    auto i = bs.find_span(u);

    bs.ders_basis_funcs(i, u, der_order, ders);

    EXPECT_DOUBLE_EQ(0.020833333333333332, ders[0][0]);
    EXPECT_DOUBLE_EQ(-0.125, ders[1][0]);
    EXPECT_DOUBLE_EQ(0.5, ders[2][0]);
    EXPECT_DOUBLE_EQ(-1, ders[3][0]);

    EXPECT_DOUBLE_EQ(0.47916666666666663, ders[0][1]);
    EXPECT_DOUBLE_EQ(-0.625, ders[1][1]);
    EXPECT_DOUBLE_EQ(-0.5, ders[2][1]);
    EXPECT_DOUBLE_EQ(3, ders[3][1]);

    EXPECT_DOUBLE_EQ(0.46875, ders[0][2]);
    EXPECT_DOUBLE_EQ(0.5625, ders[1][2]);
    EXPECT_DOUBLE_EQ(-0.75, ders[2][2]);
    EXPECT_DOUBLE_EQ(-3.5, ders[3][2]);

    EXPECT_DOUBLE_EQ(0.03125, ders[0][3]);
    EXPECT_DOUBLE_EQ(0.1875, ders[1][3]);
    EXPECT_DOUBLE_EQ(0.75, ders[2][3]);
    EXPECT_DOUBLE_EQ(1.5, ders[3][3]);
}
}
