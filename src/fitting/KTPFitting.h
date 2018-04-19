//
// Created by haochuanchen on 18-4-19.
//

#ifndef B_SPLINE_KTPFITTING_H
#define B_SPLINE_KTPFITTING_H

#include "BSplineCurveFitting_Base.h"

class KTPFitting : public BSplineCurveFitting_Base
{
public:
    using _Base = BSplineCurveFitting_Base;

    using _Dt = _Base::_Dt;
    using _Vt = _Base::_Vt;

public:
    using BSplineCurveFitting_Base::BSplineCurveFitting_Base;

    /// KTP Algorithm.
    /// See: *The NURBS Book* (Sect. 9.4.1)
    std::vector<_Dt> select_knot_vector() override;
};


#endif //B_SPLINE_KTPFITTING_H
