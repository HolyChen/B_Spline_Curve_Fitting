#include "CentralWidget.h"
#include "../fitting/KTPFitting.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>

CentralWidget::CentralWidget(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    viewer = new Viewer();
    setCentralWidget(viewer);
    setWindowTitle("B-spline curve fitting");

    _create_actions();
    _create_menus();
    _create_context_menu();
}

CentralWidget::~CentralWidget()
{
    _init_curve_fitting();
}

// --------- GUI initialization ---------
void CentralWidget::_create_actions()
{
    // ------- File -------
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing mesh file"));
    connect(openAction, &QAction::triggered, this, &CentralWidget::open);
}

void CentralWidget::_create_menus()
{
    // ------- File -------
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
}

void CentralWidget::_create_context_menu()
{
    // ------- View -------
    viewer->setContextMenuPolicy(Qt::ActionsContextMenu);
}


// --------- Actions' slots ---------

/**
 * Open a *.cd|*.obj file which contains 2d B-Spline graph.
 */
void CentralWidget::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select File..."), "./curves", tr("Curve Data(*.cd *obj)"));

    if(!fileName.isEmpty())
    {
        _init_curve_fitting();

        curve = new _ParaCurve();

        curve->init_from_file(fileName);

        viewer->clear_curves();
        if (!curve->get_vertices().empty())
        {
            viewer->add_para_curve(curve);
        }

        // TODO: TEMP
        curve->chordal_parameterization();

        auto fit = KTPFitting(*curve, 3, 5);
        auto b_curve = new _BSplineCurve(fit.fitting());
        b_curve->recalculate_curve(0.01);
        b_spline_curves.push_back(b_curve);

        viewer->add_B_spline_curve(b_curve);
    }
}


void CentralWidget::_init_curve_fitting()
{
    if (curve != nullptr)
    {
        delete curve;
        curve = nullptr;
    }

    for (const auto b_curve : b_spline_curves)
    {
        if (b_curve != nullptr)
        {
            delete b_curve;
        }
    }
    b_spline_curves.clear();
}
