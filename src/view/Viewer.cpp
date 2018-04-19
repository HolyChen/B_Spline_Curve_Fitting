#include "Viewer.h"

Viewer::Viewer()
{
}


Viewer::~Viewer()
{
    clear_curves();
}

void Viewer::clear_curves()
{
    for (auto& p : _para_curve_view)
    {
        delete p;
    }
    _para_curve_view.clear();

    for (auto& p : _b_curve_view)
    {
        delete p;
    }
    _b_curve_view.clear();
}

void Viewer::add_para_curve(const _ParaCurve *curve)
{
    _para_curve_view.push_back(new _ParaCurveView(curve, 4.0f, 0x000000FF));

    _update_view();
}

void Viewer::add_B_spline_curve(const _BSplineCurve *curve)
{
    _b_curve_view.push_back(new _BSplineCurveView(curve, 4.0f, 0x00FFFF99));

    _update_view();
}


void Viewer::init()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Restore previous viewer state.
    restoreStateFromFile();
    setBackgroundColor(QColor(255, 255, 255, 0));
}

void Viewer::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(3.0f, 1.0f);
    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    // Show curve there
    {
        _draw_original_curve();
        _draw_b_curve();
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();
}

void Viewer::_draw_original_curve()
{
    if (_para_curve_view.empty())
    {
        return;
    }
    for (auto& curve_view : _para_curve_view)
    {
        curve_view->show_curve();
    }
}

void Viewer::_draw_b_curve()
{
    if (_b_curve_view.empty())
    {
        return;
    }
    for (auto& curve_view : _b_curve_view)
    {
        curve_view->show_curve();
    }
}


void Viewer::_update_view()
{
    _BoundBox box;

    box.x_min = box.y_min = box.z_min = std::numeric_limits<_Dt>::max();
    box.x_max = box.y_max = box.z_max = -std::numeric_limits<_Dt>::max();

    for (auto& curve_view : _para_curve_view)
    {
        const auto& b = curve_view->get_bound_box();
        box.x_min = std::min(box.x_min, b.x_min);
        box.y_min = std::min(box.y_min, b.y_min);
        box.z_min = std::min(box.z_min, b.z_min);

        box.x_max = std::max(box.x_max, b.x_max);
        box.y_max = std::max(box.y_max, b.y_max);
        box.z_max = std::max(box.z_max, b.z_max);
    }

    for (auto& curve_view : _b_curve_view)
    {
        const auto& b = curve_view->get_bound_box();
        box.x_min = std::min(box.x_min, b.x_min);
        box.y_min = std::min(box.y_min, b.y_min);
        box.z_min = std::min(box.z_min, b.z_min);

        box.x_max = std::max(box.x_max, b.x_max);
        box.y_max = std::max(box.y_max, b.y_max);
        box.z_max = std::max(box.z_max, b.z_max);
    }

    if (box.z_max == 0.0)
    {
        box.z_max = 1.0;
    }

    _set_scene(box);
    paintGL();
}

void Viewer::_set_scene(const _BoundBox &box)
{
    double x_length = (box.x_max - box.x_min) / 2;
    double y_length = (box.y_max - box.y_min) / 2;
    double z_length = (box.z_max - box.z_min) / 2;

    double x_center = box.x_min + x_length;
    double y_center = box.y_min + y_length;
    double z_center = box.z_min + z_length;

    double radius = std::sqrt(x_length * x_length + y_length * y_length + z_length * z_length);

    setSceneCenter(qglviewer::Vec(x_center, y_center, z_center));
    setSceneRadius(radius);
    showEntireScene();
}

