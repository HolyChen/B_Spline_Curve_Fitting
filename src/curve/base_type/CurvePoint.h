//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_CURVEPOINT_H
#define B_SPLINE_CURVEPOINT_H

#include "Vector3X.h"

#include "PointTraits.h"

template <typename _DataType = double,
          typename _Trait = BasePointTrait<_DataType>>
struct CurvePoint
{
public:
    using _Dt = _DataType;
    using _Tr = _Trait;
public:
    /// vertex of the point
    Vertex<_Dt> vertex;

    /// trait of the point
    _Tr trait;
};


#endif //B_SPLINE_CURVEPOINT_H
