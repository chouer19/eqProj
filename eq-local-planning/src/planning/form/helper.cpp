#include "eqDriving/planning/form/helper.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <cmath>

#include "thirdParty/Eigen/Dense"

Helper::Helper()
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    //background = QBrush(QColor(64, 32, 64));
    background = QBrush(QColor(0, 0, 0));
    circleBrush = QBrush(gradient);
    //circlePen = QPen(Qt::black);
    circlePen = QPen(Qt::white);
    circlePen.setWidth(2);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
}

void Helper::paintCar(QPainter *painter){
    QPointF *points = new QPointF[4];
    points[0] = QPointF(-6,-1.9);
    points[1] = QPointF(0,-1.9);
    points[2] = QPointF(0,1.9);
    points[3] = QPointF(-6,1.9);
    resizePoints(points,4);
    //points[0] = QPointF(281,270);
    //points[1] = QPointF(319,270);
    //points[2] = QPointF(319,330);
    //points[3] = QPointF(281,330);
    painter->setBrush(QBrush(Qt::yellow));
    painter->drawPolygon(points, 4);
    delete points;
}

void Helper::paint(QPainter *painter, eqDriving::vehicle::State state, const vector<double> xs, const vector<double> ys, const QColor color, const int width){
    Eigen::Vector3d base(sin(state.Yaw()),cos(state.Yaw()),0);
    Eigen::Vector3d proj;
    QPointF *points = new QPointF[xs.size()];
    double forward, left;
    for(int i=0;i<xs.size();i++){
        proj << xs[i]-state.X(), ys[i]-state.Y(),0;
        forward = base.dot(proj);
        left = base.cross(proj)[2];
        points[i] = QPointF(forward, left);
    }
    //cout << "a=================a\n";
    //cout << state.Yaw() << endl;
    //cout << state.Y() << endl;
    //cout << forward << endl;
    //cout << proj << endl;

    resizePoints(points,xs.size());
    painter->setPen(QPen(color, width));
    painter->drawPolyline(points, xs.size());
    painter->drawPoints(points, ys.size());
    delete points;
}

void Helper::resizePoints(QPointF *points, size_t length){
    double pixelbymeter = 10;
    for(int i=0; i< length; i++){
        points[i].setX(points[i].x()*pixelbymeter + 180.0);
        points[i].setY(600 - (points[i].y()*pixelbymeter + 300.0));
    }
}


void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->fillRect(event->rect(), background);
    painter->translate(100, 100);

    painter->save();
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    painter->rotate(elapsed * 0.030);

    qreal r = elapsed / 1000.0;
    int n = 30;
    for (int i = 0; i < n; ++i) {
        painter->rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 0 + 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter->drawEllipse(QRectF(radius, -circleRadius,
                                    circleRadius * 2, circleRadius * 2));
    }
    painter->restore();

    painter->setPen(textPen);
    painter->setFont(textFont);
}
