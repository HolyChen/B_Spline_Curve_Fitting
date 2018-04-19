//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_BASEPOINTTRAIT_H
#define B_SPLINE_BASEPOINTTRAIT_H

template <typename _DataType = double>
struct BasePointTrait
{
public:
    using _Dt = _DataType;
};

template <typename _DataType = double>
struct ParaPointTrait : public BasePointTrait<_DataType>
{
public:
    using _Dt = _DataType;

public:
    /// parameter of the vertex
    _Dt u;
};



template <typename _DataType = double>
struct BSplinePointTrait : public ParaPointTrait<_DataType>
{
public:
    using _Dt = _DataType;

public:
    /// knot span index of the parameter `u`
    int span;
};

#endif //B_SPLINE_BASEPOINTTRAIT_H
