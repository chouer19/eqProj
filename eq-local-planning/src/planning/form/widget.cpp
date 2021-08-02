#include "eqDriving/planning/form/widget.hpp"
#include "eqDriving/planning/form/helper.hpp"

#include "eqDriving/planning/frenet.hpp"
#include "eqDriving/vehicle/Control.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include <ctime>
#include <cstdlib>

//! [0]
Widget::Widget(Helper *helper, QWidget *parent)
    : QWidget(parent), helper(helper)
{
    elapsed = 10;
    elapsed = 100;
    setFixedSize(900, 600);

    frenet = eqDriving::planning::Frenet("for_plamform_load_to_birang.map");
    state = eqDriving::vehicle::State();

    state.X(frenet.mapX()[elapsed]);
    state.Y(frenet.mapY()[elapsed]);
    state.Yaw(frenet.mapYaw()[elapsed] + 0.3);
    //for(int i=0;i<frenet.mapX().size();i+=10){
    //    cout << frenet.mapX()[i] << endl;
    //}
    //cout << "init yaw is : " << state.Yaw() << endl;

    state.Velocity(6.8, 0, 0);
    frenet.updateState(state);
}
//! [0]

//! [2]
void Widget::paintEvent(QPaintEvent *event)
{
    srand (time(NULL));
    int ran = rand() % 7 + 1;
    Vector3d cmder ;
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //helper->paint(&painter, event, elapsed);
    painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));

    helper->paint(&painter, state, frenet.mapX(), frenet.mapY(), Qt::red, 2);
    vector<double> xs,ys;
    int baseline = 32;

    frenet.calcPath(-6, baseline, xs, ys);
    helper->paint(&painter, state, xs, ys, Qt::yellow, 1);

    frenet.calcPath(-5, baseline, xs, ys);
    helper->paint(&painter, state, xs, ys, Qt::green, 1);

    frenet.calcPath(-4, baseline, xs, ys);
    helper->paint(&painter, state, xs, ys, Qt::blue, 1);

    frenet.calcPath(-3, baseline, xs, ys);
    if(ran == 1){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::yellow, 1);

    frenet.calcPath(-2, baseline, xs, ys);
    if(ran == 2){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::green, 1);

    frenet.calcPath(-1, baseline, xs, ys);
    if(ran == 3){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::blue, 1);

    frenet.calcPath(0, baseline, xs, ys);
    if(ran == 4){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::white, 1);

    frenet.calcPath(1, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 1,baseline, xs,ys);   
    if(ran == 5){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::yellow, 1);

    frenet.calcPath(2, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 2,baseline, xs,ys);   
    if(ran == 6){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::green, 1);

    frenet.calcPath(3, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 3,baseline, xs,ys);   
    if(ran == 7){
        cmder = eqDriving::vehicle::purepursuit(state, xs, ys, [](double v){return v*1+6;});}
    helper->paint(&painter, state, xs, ys, Qt::blue, 1);

    frenet.calcPath(4, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 4,baseline, xs,ys);   
    helper->paint(&painter, state, xs, ys, Qt::yellow, 1);

    frenet.calcPath(5, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 5,baseline, xs,ys);   
    helper->paint(&painter, state, xs, ys, Qt::green, 1);

    frenet.calcPath(6, baseline, xs, ys);
    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 6,baseline, xs,ys);   
    helper->paint(&painter, state, xs, ys, Qt::blue, 1);

    eqDriving::planning::calcPath(frenet.mapX(), frenet.mapY(),frenet.mapYaw(),state.X(), state.Y(), state.Yaw(), 10,baseline, xs,ys);   
    helper->paint(&painter, state, xs, ys, Qt::red, 3);

    state.Update(cmder);
    frenet.updateState(state);
    if(frenet.done()){
        state.X(frenet.mapX()[20]);
        state.Y(frenet.mapY()[20]);
        state.Yaw(frenet.mapYaw()[20] + (rand()%101 - 50) * 0.005);
    }
    helper->paintCar(&painter);
    painter.end();
}
//! [2]
//! [1]
void Widget::animate()
{
    //elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}
//! [1]

