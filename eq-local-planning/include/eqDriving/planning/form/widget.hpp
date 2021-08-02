#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "eqDriving/planning/frenet.hpp"
#include "eqDriving/vehicle/State.hpp"

//! [0]
class Helper;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Helper *helper, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    eqDriving::planning::Frenet frenet;// new eqDriving::planning::Frenet("for_plamform_load_to_birang.map");
    eqDriving::vehicle::State state;
    Helper *helper;
    int elapsed;
};
//! [0]

#endif
