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





    QWidget::paintEvent(event);


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



QPointF QDisplayWidget1::ToDisplay_Correlation(point_2d_t inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((-inputPoint.x*rate+displayRect.width()/2)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2)+offsety);

    return outputPoint;
}

QPointF QDisplayWidget1::ToDisplay_Correlation(point_3d_t inputPoint)
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
