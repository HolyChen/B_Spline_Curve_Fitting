//
// Created by haochuanchen on 18-4-7.
//

#ifndef B_SPLINE_CURVEVIEW_H
#define B_SPLINE_CURVEVIEW_H

#include <GL/gl.h>

#include "../Curve.h"
#include "../base_type/BoundBox.h"

/**
 * View of curve.
 * Call OpenGL and show
 */
template <typename _Curve>
class CurveView
{
private:
    using _Ct = _Curve;
    using _Pt = typename _Ct::_Pt;
    using _Dt = typename _Pt::_Dt;

    static_assert(std::is_base_of<Curve<typename _Ct::_Pt>, _Ct>::value);

private:
     /// The curve to show
    const _Ct *_curve;

    /// line_width
    float _line_width;

    /// Color
    unsigned long _color;


public:
    /**
     * Initial curve view.
     * @param curve curve to show
     */
    explicit CurveView(const _Ct* curve, float line_width = 2.0f, unsigned long color = 0x000000FF)
        : _curve(curve), _line_width(line_width), _color(color)
    {
    }

    /**
     * Get the bounded box of the curve
     * @return
     */
    const BoundBox<_Dt>& get_bound_box() const
    {
        return _curve->get_bound_box();
    }

    /**
     * Paint the curve using openGL.
     */
    void show_curve() const
    {
        const auto& vertices = _curve->get_vertices();

        glColor4d(color_component(_color, 'r'),
                  color_component(_color, 'g'),
                  color_component(_color, 'b'),
                  color_component(_color, 'a'));

        glLineWidth(_line_width);
        glBegin(GL_LINE_STRIP);

        for (const auto& v : vertices)
        {
            glVertex3d(v.vertex.x, v.vertex.y, v.vertex.z);
        }
        glEnd();
    }

protected:
    /// Get compoonent of color
    /// \param color
    /// \param ch channel, 'r', 'g', 'b', 'a'
    /// \return
    double color_component(unsigned long color, char ch) const
    {
        switch (ch)
        {
            case 'r':
                return (double)((color & 0xFF000000) >> 24) / 255.0;
            case 'g':
                return (double)((color & 0x00FF0000) >> 16) / 255.0;
            case 'b':
                return (double)((color & 0x0000FF00) >> 8) / 255.0;
            case 'a':
                return (double)((color & 0x000000FF) >> 0) / 255.0;
            default:
                break;
        }
        return 0.0;
    }

};

#endif //B_SPLINE_CURVEVIEW_H
