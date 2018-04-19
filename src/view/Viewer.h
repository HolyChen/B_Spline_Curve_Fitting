#ifndef _VIEWER_H_
#define _VIEWER_H_

#include <QGLViewer/qglviewer.h>

#include "../curve/ParaCurve.h"
#include "../curve/view/CurveView.h"

#include "utility"
#include "../curve/BSplineCurve.h"

class Viewer : public QGLViewer
{
    Q_OBJECT

private:
    using _Dt = double;
    using _ParaPointTrait = ParaPointTrait<_Dt>;
    using _ParaPoint = CurvePoint<_Dt, _ParaPointTrait>;
    using _ParaCurve = ParaCurve<_ParaPoint>;
    using _ParaCurveView = CurveView<_ParaCurve>;

    using _BSplinePointTrait = BSplinePointTrait<_Dt>;
    using _BSplinePoint = CurvePoint<_Dt, _BSplinePointTrait>;
    using _BSplineCurve = BSplineCurve<_BSplinePoint>;
    using _BSplineCurveView = CurveView<_BSplineCurve>;

    using _BoundBox = BoundBox<_Dt>;

public:
    Viewer();
    ~Viewer();

     /// Add a parameterized curve to show.
     /// \param curve the curve
    void add_para_curve(const _ParaCurve *curve);

    /// Add a B spline curve to show
    /// \param curve
    void add_B_spline_curve(const _BSplineCurve *curve);

    /// Clear all curves
    void clear_curves();

protected:
    void init() override;
    void draw() override;

private:
    void _draw_original_curve();
    void _draw_b_curve();

    void _update_view();

    /// Set scene of the viewer.
    /// \param box the bound box of the curve
    void _set_scene(const _BoundBox &box);

private:
    std::vector<_ParaCurveView*> _para_curve_view;
    std::vector<_BSplineCurveView*> _b_curve_view;

};


#endif
