#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include "Viewer.h"
#include "../curve/BSplineCurve.h"

class CentralWidget : public QMainWindow
{
    Q_OBJECT

private:
    using _Dt = double;
    using _ParaPointTrait = ParaPointTrait<_Dt>;
    using _ParaPoint = CurvePoint<_Dt, _ParaPointTrait>;
    using _ParaCurve = ParaCurve<_ParaPoint>;

    using _BSplinePointTrait = BSplinePointTrait<_Dt>;
    using _BSplinePoint = CurvePoint<_Dt, _BSplinePointTrait>;
    using _BSplineCurve = BSplineCurve<_BSplinePoint>;

public:
    explicit CentralWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);
    ~CentralWidget() final;

public:
    void open();

private:
    void _init_curve_fitting();

    void _create_actions();
    void _create_menus();
    void _create_context_menu();

private:

private:
    // ------- File -------
    QMenu *fileMenu = nullptr;
    QAction *openAction = nullptr;

    // ------ Viewer ------
    Viewer *viewer = nullptr;

private:
    // curve
    _ParaCurve *curve = nullptr;

    // fitted B spline curves
    std::vector<_BSplineCurve*> b_spline_curves;
};

#endif // CENTRALWIDGET_H
