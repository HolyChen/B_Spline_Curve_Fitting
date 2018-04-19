//
// Created by haochuanchen on 18-4-7.
//

#ifndef B_SPLINE_BOUNDBOX_H
#define B_SPLINE_BOUNDBOX_H

/// Bound box of a curve.
/// \tparam _DataType data type of the coordinate, default double.
template <typename _DataType = double>
struct BoundBox
{
public:
    using _Dt = _DataType;

public:

    _Dt x_min;
    _Dt x_max;

    _Dt y_min;
    _Dt y_max;

    _Dt z_min;
    _Dt z_max;
};

#endif //B_SPLINE_BOUNDBOX_H
