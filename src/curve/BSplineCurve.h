//
// Created by haochuanchen on 18-4-19.
//

#ifndef B_SPLINE_BSPLINECURVE_H
#define B_SPLINE_BSPLINECURVE_H

#include "base_type/PointTraits.h"
#include "ParaCurve.h"
#include "util/BSplineFunction.h"

template <typename _PointType = CurvePoint<double, BSplinePointTrait<double>>>
struct BSplineCurve : public ParaCurve<_PointType>
{
    using _Dt = typename _PointType::_Dt;
    using _Pt = _PointType;
    using _Base = ParaCurve<_Pt>;

    static_assert(std::is_base_of<BSplinePointTrait<_Dt>, typename _Pt::_Tr>::value);

protected:

    using _Base::_vertices;
    using _Base::_para_type;

    /// control points of the B spline
    std::vector<Vertex<_Dt>> _ctrlpts;
    /// degree(order - 1) of the B spline
    int _degree;
    /// knot vector
    std::vector<_Dt> _knots;

public:
    BSplineCurve() = default;

    /// Create B spline curve defined on `control_points` and knot vector(`knots`).
    /// \param degree degree(order - 1) of the B spline
    /// \param control_points control points of the B spline
    /// \param knots the knots vector of the B spline
    BSplineCurve(int degree, const std::vector<Vector3X<_Dt>>& control_points, const std::vector<_Dt>& knots)
        : _degree(degree), _ctrlpts(control_points)
    {
        _check_knots_ascending(knots);
        this->_knots = knots;
        _normalize_knots(this->_knots);
    }

    /// Create B spline curve defined on `control_points`.
    /// The knot vector will be generated uniformly using the given degree and the number of control points.
    /// \param degree degree(order - 1) of the B spline
    /// \param control_points
    BSplineCurve(int degree, const std::vector<Vector3X<_Dt>>& control_points)
            : _degree(degree), _ctrlpts(control_points)
    {
        _knots = generate_uniform_knot_vector();
    }

public: // generate curve vertex

    /// Recalculate B spline curve point. After all properties, including control points, knot vector and degree, have
    /// been set, call this method to update curve.
    /// Pre: set `ctrlpts`, `knots` and `degree`
    /// See: *The NURBS Book* Algorithm A3.1
    /// \param sample_rate sample rate
    void recalculate_curve(_Dt sample_rate = _Dt(0.01)) override
    {
        if (sample_rate <= 0)
        {
            throw std::invalid_argument("sample rate must be greater than zero but less than 1.0.");
        }

        _vertices.clear();

        BSplineFunction<_Dt> bf(_ctrlpts.size() - 1, _degree, _knots);

        auto func_values = new _Dt[_degree + 1];

        _Pt point;

        int count = 0;

        for (_Dt u = sample_rate * count, end = _knots[_knots.size() - 1]; u <= end; u = sample_rate * count)
        {
            auto span = bf.find_span(u);
            bf.basis_funcs(span, u, func_values);
            point.vertex = Vector3X<_Dt>();

            for (int i = 0; i <= _degree; i++)
            {
                point.vertex += func_values[i] * _ctrlpts[span - _degree + i];
            }

            point.trait.u = u;
            point.trait.span = span;
            _vertices.emplace_back(point);

            count++;
        }

        delete[] func_values;
    }

    /// Get uniformly distributed knot vector according to control point and degree.
    /// Pre: set `_ctrlpts` and `_degree`
    /// \return uniformly distributed knot vector
    std::vector<_Dt> generate_uniform_knot_vector()
    {
        // min and max knot vector values
        _Dt knot_min = _Dt(0.0);
        _Dt knot_max = _Dt(1.0);

        int n = int(_ctrlpts.size()) - 1;

        // equation to use: m = n + p + 1
        // p: _degree, n + 1: number of control points; m + 1: number of knots
        int m = _degree + n + 1;

        // calculate a uniform interval for middle knots
        // number of segments in the middle
        int num_segments = (m - (_degree + 1) * 2) + 2;
        // spacing between the knots (uniform)
        _Dt spacing = (knot_max - knot_min) / num_segments;

        std::vector<_Dt> knot_vector(m + 1);

        // first _degree + 1 knots are `knot_min`
        for (int i = 0; i <= _degree; i++)
        {
            knot_vector[i] = knot_min;
        }

        // middle knots
        for (int i = _degree + 1; i <= n; i++)
        {
            knot_vector[i] = spacing * (i - _degree);
        }

        // Last _degree+1 knots are `knot_max`
        for (int i = n + 1; i <= m; i++)
        {
            knot_vector[i] = knot_max;
        }

        return knot_vector;
    }

public: // getter and setter

    /// Get the degree of the B spline curve
    /// \return degree of the curve
    int get_degree() const
    {
        return _degree;
    }

    /// Set the degree of the B spline curve
    /// \param degree
    void set_degree(int degree)
    {
        this->_degree = degree;
    }

    /// Get the knot vector of the B spline curve.
    /// \return knot vector
    const std::vector<_Dt>& get_knot_vector() const
    {
        return _knots;
    }

    /// Set knot vector of the B spline curve. The knot vector will be normalized automatically.
    /// \param knots knot vector
    void set_knot_vector(const std::vector<_Dt>& knots)
    {
        _check_knots_ascending(knots);
        this->_knots = knots;
        _normalize_knots(this->_knots);
    }

    /// Get the control points of the B spline curve.
    /// \return control points
    const std::vector<Vector3X<_Dt>>& get_control_points() const
    {
        return _ctrlpts;
    }

    /// Set the control points of the B spline curve.
    /// \param control_points control points
    void set_control_points(const std::vector<Vector3X<_Dt>>& control_points)
    {
        this->_ctrlpts = control_points;
    }

protected:

    void _check_knots_ascending(const std::vector<_Dt>& knot_vec)
    {
        _Dt last_knot = knot_vec[0];
        for (int i = 1; i < knot_vec.size(); i++)
        {
            if (last_knot > knot_vec[i])
            {
                throw std::logic_error("the knot vector is not ascending.");
            }
        }
    }

    void _normalize_knots(std::vector<_Dt>& knot_vec)
    {
        if (knot_vec.size() < 2)
        {
            return;
        }
        _Dt knot_min = knot_vec[0], knot_max = knot_vec[knot_vec.size() - 1];
        _Dt knot_range = knot_max - knot_min;

        if (std::abs(knot_range) == 0.0)
        {
            for (int i = 0; i < knot_vec.size(); i++)
            {
                knot_vec[i] = 1.0;
            }
            return;
        }

        for (int i = 0; i < knot_vec.size(); i++)
        {
            knot_vec[i] = (knot_vec[i] - knot_min) / knot_range;
        }
    }
};


#endif //B_SPLINE_BSPLINECURVE_H
