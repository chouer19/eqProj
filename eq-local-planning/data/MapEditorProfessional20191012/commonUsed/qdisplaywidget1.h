#ifndef QDISPLAYWIDGET1_H
#define QDISPLAYWIDGET1_H

#include<QWidget>
#include"commonUsed/typedefs.h"
#include <QPainter>
#include<QPicture>
#include<QDebug>

class QDisplayWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit QDisplayWidget1(QWidget *parent = nullptr);

    //
    bool isMouseInWidget=false;

    void setMapData(QList<MAP_POINT> dataList);
    void setLocateData(INS_OPVT2AHR_DATA data);
    void setTrackData(QList<QList<TRACK_POINT>> dataList);
    void setCandidateleft(QList<QList<TRACK_POINT>> dataList);
    void setCandidateright(QList<QList<TRACK_POINT>> dataList);
    void setChosedTrack(QList<TRACK_POINT> data);

    virtual void mouseDown();
    virtual void mouseMove();
    virtual void mouseUp();
    virtual void mouseDoubleClick();
    virtual void wheelRoll(QWheelEvent *event);


    bool isShowMapData=true;
    bool isShowTrack=false;
    bool isShowCandidateLeft=false;
    bool isShowCandidateRight=false;
    bool isShowChosedTrack=true;

signals:

public slots:

protected:
    //
    QList<MAP_POINT> mapDisData;
    INS_OPVT2AHR_DATA location;


    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    virtual QPointF ToDisplay_Correlation(POINT2D inputPoint);
    virtual QPointF ToDisplay_Correlation(POINT3D inputPoint);
    virtual void paintEvent(QPaintEvent *event);
    QList<QList<TRACK_POINT>> track;
    QList<QList<TRACK_POINT>> candidateLeft;
    QList<QList<TRACK_POINT>> candidateRight;
    QList<TRACK_POINT> chosedTrack;



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
