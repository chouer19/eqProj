#include "qdisplaywidget1.h"


QDisplayWidget1::QDisplayWidget1(QWidget *parent) : QWidget(parent)
{

}

void QDisplayWidget1::paintEvent(QPaintEvent *event)
{
    displayRect=this->rect();
    QPainter painter(this);
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);

    QPixmap car;
    car.load(":/new/picture/resource/GL8.png");

    double length=5.25;
    double width=1.9;

    QPointF leftTop=ToDisplay_Correlation(POINT2D(width/2,4.25));

    painter.drawPixmap(leftTop.x(),leftTop.y(),width*rate,length*rate,car);

    if(isShowMapData)
    {
        for(int i=0;i<mapDisData.size();i++)
        {
            QPointF p=ToDisplay_Correlation(mapDisData[i].point);

            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPie(p.x()-1,p.y()-1,2,2,0,360*16);
        }
    }


    if(isShowTrack)
    {
        for(int j=0;j<track.size();j++)
        {
            //        if(j==track.size()/2||j==0||j==track.size()-1)
            {
                for(int i=0;i<track[j].size();i++)
                {
                    QPointF p=ToDisplay_Correlation(track[j][i].point);

                    pen.setColor(Qt::blue);
                    painter.setPen(pen);
                    painter.drawPoint(p);

                }
            }
        }
    }
//    for(int j=0;j<candidateLeft.size();j++)
//    {

//       if(j==candidateLeft.size()/2||j==0||j==candidateLeft.size()-1)
//        {
//            for(int i=0;i<candidateLeft[j].size();i++)
//            {
//                QPointF p=ToDisplay_Correlation(candidateLeft[j][i].point);

//                pen.setColor(Qt::green);
//                painter.setPen(pen);
//                painter.drawPoint(p);
//                QString string=QString::number(candidateLeft[j][i].aimSpeed);

//                //            if(i%10==0)
//                //                painter.drawText(p,string);

//            }
//        }
//    }

//    for(int j=0;j<candidateRight.size();j++)
//    {

//       if(j==candidateRight.size()/2||j==0||j==candidateRight.size()-1)
//        {
//            for(int i=0;i<candidateRight[j].size();i++)
//            {
//                QPointF p=ToDisplay_Correlation(candidateRight[j][i].point);

//                pen.setColor(Qt::yellow);
//                painter.setPen(pen);
//                painter.drawPoint(p);
//                QString string=QString::number(candidateRight[j][i].aimSpeed);

//                //            if(i%10==0)
//                //                painter.drawText(p,string);

//            }
//        }
//    }


    if(isShowChosedTrack)
    {
        for(int i=0;i<chosedTrack.size()-1;i++)
        {
            QPointF p=ToDisplay_Correlation(chosedTrack[i].point);
            QPointF p1=ToDisplay_Correlation(chosedTrack[i+1].point);

            pen.setColor(Qt::darkRed);
            painter.setPen(pen);
            painter.drawLine(p,p1);
            QString string=QString::number(chosedTrack[i].aimSpeed*3.6,10,2);

            if(i%10==0)
                painter.drawText(p,string);

        }

    }



    QWidget::paintEvent(event);


}

void QDisplayWidget1::setMapData(QList<MAP_POINT> dataList)
{
    mapDisData=dataList;
}

void QDisplayWidget1::setLocateData(INS_OPVT2AHR_DATA data)
{
    location=data;
}

void QDisplayWidget1::setTrackData(QList<QList<TRACK_POINT>> dataList)
{
    track=dataList;
}

void QDisplayWidget1::setCandidateleft(QList<QList<TRACK_POINT> > dataList)
{
    candidateLeft=dataList;
}

void QDisplayWidget1::setCandidateright(QList<QList<TRACK_POINT> > dataList)
{
    candidateRight=dataList;
}

void QDisplayWidget1::setChosedTrack(QList<TRACK_POINT> data)
{
    chosedTrack=data;
}

void QDisplayWidget1::mouseDown()
{
    if(isMouseInWidget)
    {
        isMouseMove=true;
        originalfx=offsetx;
        originalfy=offsety;
        originalx=cursor().pos().x();
        originaly=cursor().pos().y();
    }
}

void QDisplayWidget1::mouseMove()
{
    if(isMouseMove)
    {
        offsetx=originalfx+cursor().pos().x()-originalx;
        offsety=originalfy+cursor().pos().y()-originaly;

    }
}

void QDisplayWidget1::mouseUp()
{

    isMouseMove=false;
}

void QDisplayWidget1::mouseDoubleClick()
{
    if(isMouseInWidget)
    {
        offsetx=0;
        offsety=0;
    }
}

void QDisplayWidget1::wheelRoll(QWheelEvent *event)
{
    if(isMouseInWidget)
    {
        if(event->delta()>0)
            rate*=1.1;
        else
            rate*=0.9;
    }
}

QPointF QDisplayWidget1::ToDisplay_Correlation(POINT2D inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((-inputPoint.x*rate+displayRect.width()/2)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2)+offsety);

    return outputPoint;
}

QPointF QDisplayWidget1::ToDisplay_Correlation(POINT3D inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((-inputPoint.x*rate+displayRect.width()/2)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2)+offsety);

    return outputPoint;
}

void QDisplayWidget1::enterEvent(QEvent *event)
{
    isMouseInWidget=true;
}

void QDisplayWidget1::leaveEvent(QEvent *event)
{
    isMouseInWidget=false;
}
