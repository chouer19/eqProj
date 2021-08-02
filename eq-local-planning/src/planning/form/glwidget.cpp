#include "eqDriving/planning/form/glwidget.hpp"
#include "eqDriving/planning/form/helper.hpp"

#include <QPainter>
#include <QTimer>

#include <iostream>
using namespace std;

//! [0]
GLWidget::GLWidget(Helper *helper, QWidget *parent)
    : QOpenGLWidget(parent), helper(helper)
{
    elapsed = 0;
    setFixedSize(200, 200);
    setFixedSize(0, 0);
    setAutoFillBackground(false);
}
//! [0]

//! [1]
void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}
//! [1]

//! [2]
void GLWidget::paintEvent(QPaintEvent *event)
{
    //QPainter painter;
    //painter.begin(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    //helper->paint(&painter, event, elapsed);
    ////QBrush background(QColor(64, 32, 64));
    ////painter.fillRect(10,10,30,30,background);
    //painter.end();
    //cout << "aaaaaa\n";
}
//! [2]
