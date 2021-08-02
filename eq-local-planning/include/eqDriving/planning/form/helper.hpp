
#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <iostream>

using namespace std;

#include "eqDriving/vehicle/State.hpp"

//! [0]
class Helper
{
public:
    Helper();

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);
    void paint(QPainter *painter, eqDriving::vehicle::State state, const vector<double> xs, const vector<double> ys,const QColor color, const int width);
    void paintCar(QPainter *painter);
    void resizePoints(QPointF *points, size_t length);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
};
//! [0]

#endif
