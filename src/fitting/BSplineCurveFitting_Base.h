//
// Created by haochuanchen on 18-4-19.
//

#ifndef B_SPLINE_BSPLINECURVEFITTING_BASE_H
#define B_SPLINE_BSPLINECURVEFITTING_BASE_H

#include "../curve/BSplineCurve.h"

class BSplineCurveFitting_Base
{
public:
    using _Dt = double;
    using _Vt = Vertex<_Dt>;

    /// Input curve point type
    using _In_Pt = CurvePoint<_Dt, ParaPointTrait<_Dt>>;
    /// Input curve type
    using _In_Ct = ParaCurve<_In_Pt>;

    /// Output curve point type
    using _Out_Pt = CurvePoint<_Dt, BSplinePointTrait<_Dt>>;
    using _Out_Ct = BSplineCurve<_Out_Pt>;

public:
    /// Initial B spline curve fitting.
    /// \param curve_to_fit a parameterized curve.
    /// \param degree degree(order - 1) of the B spline curve
    /// \param n_control_point the number of control points of the fitted curve, NO more than the number of vertices
    /// on the `curve_to_fit`
    BSplineCurveFitting_Base(const _In_Ct& curve_to_fit, int degree, int n_control_point);

    /// Fit the curve.
    /// \return get fitted B Spline curve
    _Out_Ct fitting();

protected:
    /// Select knot vector to fit curve.
    /// See: *The NURBS Book* (Sect. 9.4.1)
    virtual std::vector<_Dt> select_knot_vector() = 0;

    /// Use minimum squared optimization to get closest control points.
    /// See: *The NURBS Book* (Sect. 9.4.1)
    /// \param knots knot vector
    /// \return control points
    virtual std::vector<_Vt> minimum_squared_optimize(const std::vector<_Dt>& knots);

protected:


protected: // --------- field ---------
    /// Original discrete curve to fit
    _In_Ct _src_curve;

    /// (`_n` + 1) is the number of control points
    int _n;

    /// degree(order - 1) of the B spline
    int _degree;
};


#endif //B_SPLINE_BSPLINECURVEFITTING_BASE_H
