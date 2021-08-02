#include "qdisplaywidget2.h"

QDisplayWidget2::QDisplayWidget2(QWidget *parent) : QDisplayWidget1(parent)
{

}

void QDisplayWidget2::paintEvent(QPaintEvent *event)
{
    displayRect=this->rect();
    QPainter painter(this);
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(2.5);


    painter.setBrush(Qt::black);
    painter.drawRect(rect());




    for(int i=0;i<mapDisData.size();i++)
    {
        QPointF p=ToDisplay_Correlation(mapDisData[i].point);


        double speed=0;
        if(mapDisData[i].curvature!=0)
            speed=(int)((sqrt(0.8/mapDisData[i].curvature)*3.6)/5)*5;
        else
            speed=999;

        QString string;
        string.clear();
        if(getShowIndex())
        {
            string+=QString::number(i);
            string+=' ';
        }

        if(getShowType())
        {
            string+=QString::number(mapDisData[i].type);
            string+=' ';
        }

        if(getShowHeading())
        {
            string+=QString::number(mapDisData[i].heading);
            string+=' ';
        }

        if(getShowZ())
        {
            string+=QString::number(mapDisData[i].point.z);
            string+=' ';
        }

        if(getShowSpeedLimit())
        {
            string+=QString::number(speed);
            string+=' ';
        }


        if(i<=indexEnd&&i>=indexStart)
        {
            pen.setColor(Qt::white);
            painter.setPen(pen);

            if(mapDisData[i].type>=90)
            {

                painter.drawPie(p.x()-3,p.y()-3,6,6,0,360*16);

            }
            else
            {

                painter.drawPoint(p);
            }

        }
        else
        {

            if(mapDisData[i].type>=90)
            {
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawPie(p.x()-3,p.y()-3,6,6,0,360*16);

            }
            else
            {
                pen.setColor(Qt::red);
                painter.setPen(pen);
                painter.drawPoint(p);
            }

        }



        if(rate>150)
            painter.drawText(p,string);
        else if(rate>10&&i%10==0)
            painter.drawText(p,string);
        else if(rate>1&&i%50==0)
            painter.drawText(p,string);




    }

    QPointF posPoint=ToDisplay_Correlation(POINT2D(location.x,location.y));
    painter.setPen(QPen(Qt::white));
    painter.drawLine(posPoint.x(),0,posPoint.x(),this->height());
    painter.drawLine(0,posPoint.y(),this->width(),posPoint.y());

    QList<MAP_POINT> _bsplineData=getBsplineData();
    for(int i=0;i<_bsplineData.size();i++)
    {
        QPointF p=ToDisplay_Correlation(_bsplineData[i].point);

        pen.setColor(Qt::green);
        painter.setPen(pen);
        painter.drawPoint(p);

    }

    QWidget::paintEvent(event);
    mapDisData.clear();
}

QList<MAP_POINT> QDisplayWidget2::getBsplineData()
{
    QList<MAP_POINT> temp;
    mutex.lock();
    temp=bSplineData;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowType()
{
    bool temp;
    mutex.lock();
    temp=showType;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowHeading()
{
    bool temp;
    mutex.lock();
    temp=showHeading;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowSpeedLimit()
{
    bool temp;
    mutex.lock();
    temp=showSpeedLimit;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowIndex()
{
    bool temp;
    mutex.lock();
    temp=showIndex;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowZ()
{
    bool temp;
    mutex.lock();
    temp=showZ;
    mutex.unlock();
    return temp;
}

void QDisplayWidget2::mouseDoubleClick()
{
//    QPointF P=ToDisplay_Correlation(POINT2D(location.x,location.y));
//    offsetx-=P.x()-this->width()/2;
//    offsety-=P.y()-this->height()/2;
    offsetx=0;
    offsety=0;
}

void QDisplayWidget2::wheelRoll(QWheelEvent *event)
{
    if(isMouseInWidget)
    {
        double ratelast=rate;
        if(event->delta()>0)
        {

            rate*=1.1;
        }
        else
        {

            rate/=1.1;
        }

        offsetx=-offsetx/ratelast*(ratelast-rate)+offsetx;
        offsety=offsety/ratelast*(rate-ratelast)+offsety;
    }
}

void QDisplayWidget2::setBsplineData(QList<MAP_POINT> dataList)
{
    mutex.lock();
    bSplineData=dataList;
    mutex.unlock();
}

void QDisplayWidget2::setShowHeading(bool flag)
{
    mutex.lock();
    showHeading=flag;
    mutex.unlock();
}

void QDisplayWidget2::setShowSpeedLimit(bool flag)
{
    mutex.lock();
    showSpeedLimit=flag;
    mutex.unlock();
}

void QDisplayWidget2::setShowIndex(bool flag)
{
    mutex.lock();
    showIndex=flag;
    mutex.unlock();
}

void QDisplayWidget2::setShowZ(bool flag)
{
    mutex.lock();
    showZ=flag;
    mutex.unlock();
}

void QDisplayWidget2::setShowType(bool flag)
{
    mutex.lock();
    showType=flag;
    mutex.unlock();
}

void QDisplayWidget2::mouseDown()
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

void QDisplayWidget2::mouseMove()
{
    if(isMouseMove)
    {
        offsetx=originalfx+cursor().pos().x()-originalx;
        offsety=originalfy+cursor().pos().y()-originaly;

    }
}

void QDisplayWidget2::mouseUp()
{
     isMouseMove=false;
}

QPointF QDisplayWidget2::ToDisplay_Correlation(POINT2D inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((inputPoint.x*rate+displayRect.width()/2.0)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2.0)+offsety);

    return outputPoint;
}

QPointF QDisplayWidget2::ToDisplay_Correlation(POINT3D inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((inputPoint.x*rate+displayRect.width()/2.0)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2.0)+offsety);

    return outputPoint;
}

POINT2D QDisplayWidget2::CounterDisplay_Correlation(QPointF inputPoint)
{
    POINT2D outputPoint;

    outputPoint.x=(inputPoint.x()-offsetx-displayRect.width()/2.0)/rate;
    outputPoint.y=-(inputPoint.y()-offsety-displayRect.height()/2.0)/rate;

    return outputPoint;
}

