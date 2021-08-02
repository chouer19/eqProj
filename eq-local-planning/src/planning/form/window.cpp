#include "eqDriving/planning/form/glwidget.hpp"
#include "eqDriving/planning/form/widget.hpp"
#include "eqDriving/planning/form/window.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

using namespace eqDriving;
using namespace planning;

#include <iostream>
using namespace std;

//! [0]
Window::Window()
{
    setWindowTitle(tr("eq local path generating"));

    //Frenet *frenet = new Frenet("for_plamform_load_to_birang.map");
    Widget *native = new Widget(&helper, this);
    GLWidget *openGL = new GLWidget(&helper, this);
    //QLabel *nativeLabel = new QLabel(tr("Native"));
    //nativeLabel->setAlignment(Qt::AlignHCenter);
    //QLabel *openGLLabel = new QLabel(tr("OpenGL"));
    //openGLLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(native, 0, 0);
    //layout->addWidget(openGL, 0, 1);
    //layout->addWidget(nativeLabel, 1, 0);
    //layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, native, &Widget::animate);
    connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(50);
}
//! [0]
