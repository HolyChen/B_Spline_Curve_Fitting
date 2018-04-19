//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_PARACURVE_H
#define B_SPLINE_PARACURVE_H

#include "Curve.h"

template <typename _PointType = CurvePoint<double, ParaPointTrait<double>>>
struct ParaCurve : public Curve<_PointType>
{
public:
    using _Dt = typename _PointType::_Dt;
    using _Pt = _PointType;
    using _Base = Curve<_Pt>;

    static_assert(std::is_base_of<ParaPointTrait<_Dt>, typename _Pt::_Tr>::value);

protected: // field

    using _Base::_vertices;
    using _Base::_is_periodic;

    // Type of parameterization
    ParameterizationMethod _para_type;

public: // constructors and destructor

    ParaCurve()
    {
    }

    virtual ~ParaCurve()
    {
    }

public: // parameterization
    /**
     * Uniform parameterization
     * The parameterization factor is proportional to 1.
     */
    void uniform_parameterization()
    {
        _para_type = ParameterizationMethod ::UNIFORM;

        if (_vertices.empty())
        {
            return;
        }

        auto num_v = _vertices.size();

        _Dt step = 1.0 / (num_v - 1);

        for (int i = 0; i < num_v; i++)
        {
            _vertices[i].trait.u = i * step;
        }
    }

    /**
     * Chordal parameterization
     * The parameterization factor is proportional to segment length.
     */
    void chordal_parameterization()
    {


        if (_vertices.empty())
        {
            return;
        }

        auto num_v = _vertices.size();

        _Dt chord_length = 0;
        std::vector<_Dt> segment_length;
        for (int i = 1; i < num_v; i++)
        {
            chord_length += (_vertices[i].vertex - _vertices[i - 1].vertex).length();
            segment_length.push_back(chord_length);
        }

        _vertices[0].trait.u = 0;
        for (int i = 1; i < num_v; i++)
        {
            _vertices[i].trait.u = segment_length[i - 1] / chord_length;
        }
    }

    /**
     * Centripetal parameterization
     * The parameterization factor is proportional to sqrt(segment length).
     */
    void centripetal_parameterization()
    {
        if (_vertices.empty())
        {
            return;
        }

        auto num_v = _vertices.size();

        _Dt chord_length = 0;
        std::vector<_Dt> segment_length;

        for (int i=1; i<num_v; i++)
        {
            chord_length += std::sqrt((_vertices[i].vertex - _vertices[i - 1].vertex).length());
            segment_length.push_back(chord_length);
        }

        _vertices[0].trait.u = 0;
        for (int i = 1; i < num_v; i++)
        {
            _vertices[i].trait.u = segment_length[i - 1] / chord_length;
        }
    }

public: // generate curve vertex
    /// Recalculate vertices on the curve.
    /// \param sample_rate the sample spacing between two adjacent vertices.
    virtual void recalculate_curve(_Dt sample_rate = _Dt(0.01))
    {
        // pass
    }

public: // getter & setter

    /// Get parameterization method of the parameterized curve.
    /// \return the parameterization method
    ParameterizationMethod set_para_type() const
    {
        return _para_type;
    }

};

#endif //B_SPLINE_PARACURVE_H
