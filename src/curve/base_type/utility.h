//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_UTILITY_H
#define B_SPLINE_UTILITY_H

/// Curve parameterization method.
/// See: *The NURBS BOOK* (Sect. 9.1)
enum class ParameterizationMethod
{
    /// un-parameterized.
    NONE,
    /// uniform parameterized
    UNIFORM,
    /// chordal parameterized
    CHORDAL,
    /// centripetal parameterized
    CENTRIPETAL,
};


#endif //B_SPLINE_UTILITY_H
