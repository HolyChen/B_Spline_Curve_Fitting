//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_CURVE_H
#define B_SPLINE_CURVE_H

#include <vector>
#include <QString>
#include <iostream>

#include "base_type/CurvePoint.h"
#include "io/CurveIO.h"

#include "base_type/utility.h"
#include "base_type/BoundBox.h"

template <typename _CurvePoint = CurvePoint<double, BasePointTrait<double>>>
class Curve
{
public:
    using _Pt = _CurvePoint;
    using _Tr = typename _CurvePoint::_Tr;
    using _Dt = typename _Pt::_Dt;

// ----------- filed -----------

protected:
    /// vertices of the curve
    std::vector<_Pt> _vertices;

    /// is periodic curve
    bool _is_periodic = false;

    /// the normalized bound box of the curve
    BoundBox<_Dt> _box;

private:
    /// I/O
    CurveIO<_Pt> _io;

// ----------- Method -----------

public: // constructors and destructor

    Curve()
    {
    }

    virtual ~Curve()
    {
    }

public:

    /// Read curve module from file, then initialize properties.
    /// \param filename filename of the curve module
    /// \param normalize is normalize the curve to [-1, 1]^3, default true
    void init_from_file(const QString& filename, bool normalize = true)
    {
        _read_from_file(filename);

        if (normalize)
        {
            _normalize();
        }

        _box = _cal_bound_box();

        _check_periodic();
    }

public: // getter & setter

    /// Get the bound box of the curve.
    /// \return
    const BoundBox<_Dt>& get_bound_box() const
    {
        return _box;
    }

    /// Get the vertices of the curve
    /// \return the vertices
    const std::vector<_Pt>& get_vertices() const
    {
        return _vertices;
    }

    /// Get the vertices of the curve
    /// \return the vertices
    std::vector<_Pt>& get_vertices()
    {
        return _vertices;
    }

protected:

    /// Check whether the curve is periodic.
    /// Someone think the distance between double end should be greater than the maximum gap between all the adjacent
    /// points, we adopt this opinion.
    void _check_periodic()
    {
        double max_dist = 0, dist;
        for (int i = 1; i < _vertices.size(); i++)
        {
            dist = (_vertices[i].vertex - _vertices[i - 1].vertex).squared_length();
            if (max_dist < dist)
            {
                max_dist = dist;
            }
        }

        dist = (_vertices[0].vertex - _vertices.back().vertex).squared_length();
        if (dist <= max_dist)
        {
            _is_periodic = true;
        }

        // add 3 points to make C2 continuous
        if (_is_periodic)
        {
            _vertices.push_back(_vertices[0]);
//            _vertices.push_back(_vertices[1]);
//            _vertices.push_back(_vertices[2]);
        }
    }

    /// Calculate bounded box of the curve.
    /// \return box the bound box
    BoundBox<_Dt> _cal_bound_box() const
    {
        BoundBox<_Dt> box;

        // extract bound box
        auto x_min = std::numeric_limits<_Dt>::max(), x_max = _Dt(0),
             y_min = std::numeric_limits<_Dt>::max(), y_max = _Dt(0),
             z_min = std::numeric_limits<_Dt>::max(), z_max = _Dt(0);

        for (const auto& vertex : _vertices)
        {
            x_min = vertex.vertex.x < x_min ? vertex.vertex.x : x_min;
            x_max = vertex.vertex.x > x_max ? vertex.vertex.x : x_max;

            y_min = vertex.vertex.y < y_min ? vertex.vertex.y : y_min;
            y_max = vertex.vertex.y > y_max ? vertex.vertex.y : y_max;

            z_min = vertex.vertex.z < z_min ? vertex.vertex.z : z_min;
            z_max = vertex.vertex.z > z_max ? vertex.vertex.z : z_max;
        }

        box.x_max = x_max;
        box.x_min = x_min;
        box.y_max = y_max;
        box.y_min = y_min;
        box.z_max = z_max;
        box.z_min = z_min;

        return box;
    }

    /// Normalize the curve
    void _normalize()
    {
        auto origin_box = _cal_bound_box();

        auto x_mid = (origin_box.x_min + origin_box.x_max) / 2.0,
             y_mid = (origin_box.y_min + origin_box.y_max) / 2.0,
             z_mid = (origin_box.z_min + origin_box.z_max) / 2.0;

        auto x_length = (origin_box.x_max - origin_box.x_min) / 2.0,
             y_length = (origin_box.y_max - origin_box.y_min) / 2.0,
             z_length = (origin_box.z_max - origin_box.z_min) / 2.0;

        x_length = x_length == 0.0 ? 0.5 : x_length;
        y_length = y_length == 0.0 ? 0.5 : y_length;
        z_length = z_length == 0.0 ? 0.5 : z_length;

        auto max_length = x_length;
        max_length = y_length > max_length ? y_length : max_length;
        max_length = z_length > max_length ? z_length : max_length;

        for (auto& vertex : _vertices)
        {
            vertex.vertex.x = (vertex.vertex.x - x_mid) / max_length;
            vertex.vertex.y = (vertex.vertex.y - y_mid) / max_length;
            vertex.vertex.z = (vertex.vertex.z - z_mid) / max_length;
        }
    }

// ----------- I/O -----------
protected: // generate curve vertex
    /// Read curve model from file.
    /// \param filename the filename
    virtual void _read_from_file(const QString& filename)
    {
        _io.read_from_file(*this, filename);
    }
};

#endif //B_SPLINE_CURVE_H
