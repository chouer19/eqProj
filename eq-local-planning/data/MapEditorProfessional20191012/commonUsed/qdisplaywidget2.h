#ifndef QDISPLAYWIDGET2_H
#define QDISPLAYWIDGET2_H

#include"qdisplaywidget1.h"
#include"QMutex"
#include<math.h>

class QDisplayWidget2 : public QDisplayWidget1
{
    Q_OBJECT
public:
    QDisplayWidget2(QWidget *parent = nullptr);

    virtual void mouseDown();
    virtual void mouseMove();
    virtual void mouseUp();
    virtual void mouseDoubleClick();
    virtual void wheelRoll(QWheelEvent *event);

    int indexStart=0;

    int indexEnd=0;

    void setBsplineData(QList<MAP_POINT> dataList);
    void setShowType(bool flag);
    void setShowHeading(bool flag);
    void setShowSpeedLimit(bool flag);
    void setShowIndex(bool flag);
    void setShowZ(bool flag);


private:
    virtual QPointF ToDisplay_Correlation(POINT2D inputPoint);
    virtual QPointF ToDisplay_Correlation(POINT3D inputPoint);
    virtual POINT2D CounterDisplay_Correlation(QPointF inputPoint);
    virtual void paintEvent(QPaintEvent *event);

    bool isMouseMove=false;
    bool isSetMiddle=false;
    float rate=0.24;
    float offsetx=0;
    float offsety=0;
    float originalx;
    float originaly;
    float originalfx;
    float originalfy;
    QRect displayRect;
    QMutex mutex;

    QList<MAP_POINT> bSplineData;
    QList<MAP_POINT> getBsplineData();

    bool showType=false;
    bool getShowType();

    bool showHeading=false;
    bool getShowHeading();

    bool showSpeedLimit=false;
    bool getShowSpeedLimit();

    bool showIndex=false;
    bool getShowIndex();

    bool showZ=false;
    bool getShowZ();

};

#endif // QDISPLAYWIDGET2_H
