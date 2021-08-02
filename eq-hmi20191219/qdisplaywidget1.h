#ifndef QDISPLAYWIDGET1_H
#define QDISPLAYWIDGET1_H

#include<QWidget>
#include <QPainter>
#include<QPicture>
#include<QDebug>
#include"header/system/pub_types.h"

class QDisplayWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit QDisplayWidget1(QWidget *parent = nullptr);

    //
    bool isMouseInWidget=false;



    virtual void mouseDown();
    virtual void mouseMove();
    virtual void mouseUp();
    virtual void mouseDoubleClick();



    bool isShowMapData=true;
    bool isShowTrack=false;
    bool isShowCandidateLeft=false;
    bool isShowCandidateRight=false;
    bool isShowChosedTrack=true;

signals:

public slots:

protected:
    //



    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    virtual QPointF ToDisplay_Correlation(point_2d_t inputPoint);
    virtual QPointF ToDisplay_Correlation(point_3d_t inputPoint);
    virtual void paintEvent(QPaintEvent *event);



    bool isMouseMove=false;
    bool isSetMiddle=false;
    float rate=8.6;
    float offsetx=0;
    float offsety=0;
    float originalx;
    float originaly;
    float originalfx;
    float originalfy;
    QRect displayRect;

};

#endif // QDISPLAYWIDGET1_H
