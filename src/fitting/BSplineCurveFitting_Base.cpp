//
// Created by haochuanchen on 18-4-19.
//

#include "BSplineCurveFitting_Base.h"

#include <Eigen/Sparse>

using SpMat = typename Eigen::SparseMatrix<BSplineCurveFitting_Base::_Dt>;
using Triplet = typename Eigen::Triplet<BSplineCurveFitting_Base::_Dt>;


BSplineCurveFitting_Base::BSplineCurveFitting_Base(const _In_Ct& curve_to_fit, int degree, int n_control_point)
    : _src_curve(curve_to_fit), _degree(degree), _n(n_control_point - 1)
{
}

BSplineCurveFitting_Base::_Out_Ct
BSplineCurveFitting_Base::fitting()
{
    auto knot_vector = select_knot_vector();
    auto control_point = minimum_squared_optimize(knot_vector);
    return BSplineCurveFitting_Base::_Out_Ct(_degree, control_point, knot_vector);
}

std::vector<BSplineCurveFitting_Base::_Vt>
BSplineCurveFitting_Base::minimum_squared_optimize(const std::vector<_Dt>& knots)
{
    int m = _src_curve.get_vertices().size() - 1;
    int row = m - 1;
    int col = _n - 1;

    // Build matrix N
    SpMat N(row, col);

    std::vector<Triplet> coefficients;

    _Dt *func_values = new _Dt[_degree + 1];

    BSplineFunction bf(_n, _degree, knots);

    const auto& src_vertices = _src_curve.get_vertices();

    // rhs: R
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(col, 3);

    _Dt u, value;
    _Dt N_0_p_uk, N_n_p_uk;

    Eigen::Vector3d Q0, Qm;
    Q0 << src_vertices[0].vertex.x, src_vertices[0].vertex.y, src_vertices[0].vertex.z;
    Qm << src_vertices[m].vertex.x, src_vertices[m].vertex.y, src_vertices[m].vertex.z;


    for (int k = 1; k <= m - 1; k++)
    {
        u = src_vertices[k].trait.u;
        int span = bf.find_span(u);
        bf.basis_funcs(span, u, func_values);

        if (span > _degree)
        {
            N_0_p_uk = _Dt(0.0);
        }
        else
        {
            N_0_p_uk = func_values[_degree - span];
        }

        if (span < _n)
        {
            N_n_p_uk = _Dt(0.0);
        }
        else
        {
            N_n_p_uk = func_values[_degree - (span - _n)];
        }

        _Dt Rk_x = src_vertices[k].vertex.x - N_0_p_uk * Q0(0) - N_n_p_uk * Qm(0);
        _Dt Rk_y = src_vertices[k].vertex.y - N_0_p_uk * Q0(1) - N_n_p_uk * Qm(1);
        _Dt Rk_z = src_vertices[k].vertex.z - N_0_p_uk * Q0(2) - N_n_p_uk * Qm(2);


        for (int j = span - _degree; j <= span; j++)
        {
            value = func_values[j - span + _degree];

            if (j > 0 && j <= _n - 1)
            {
                // set N's coefficients
                coefficients.emplace_back(k - 1, j - 1, value);

                // set R
                R(j - 1, 0) += value * Rk_x;
                R(j - 1, 1) += value * Rk_y;
                R(j - 1, 2) += value * Rk_z;
            }
        }

    }

    N.setFromTriplets(coefficients.begin(), coefficients.end());

    Eigen::SparseQR<SpMat, Eigen::AMDOrdering<int>> qr(N.transpose() * N);

    Eigen::MatrixXd P = qr.solve(R);

    delete[] func_values;

    std::vector<_Vt> result;

    // P0 = Q0, Pn = Qm

    // P0
    result.emplace_back(src_vertices[0].vertex.x, src_vertices[0].vertex.y, src_vertices[0].vertex.z);

    for (int i = 1; i <= _n - 1; i++)
    {
        result.emplace_back(P(i - 1, 0), P(i - 1, 1), P(i - 1, 2));
    }

    // Pn
    result.emplace_back(src_vertices[m].vertex.x, src_vertices[m].vertex.y, src_vertices[m].vertex.z);

    return result;
}
