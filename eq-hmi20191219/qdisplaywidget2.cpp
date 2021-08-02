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

    LOAD_POS _lastLoadPoint=getLastLoadPoint();
    LOAD_POS _loadPoint=getLoadPoint();
    LOAD_POS _autoLoadPoint=getAutoLoadPoint();
    DIRC_POINT _reachPoint=getReachPoint();
    vector<point_lidarmap_t> _left_boundary_position_read=getLeftBoundary();
    vector<point_lidarmap_t> _right_boundary_position_read=getRightBoundary();

    //map boundary
    pen.setWidthF(0.1*rate);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for(int i=0;i<_left_boundary_position_read.size();i++)
    {
        QPointF p=ToDisplay_Correlation(point_2d_t(_left_boundary_position_read[i].p.x,_left_boundary_position_read[i].p.y));
        painter.drawPoint(p);
    }

    for(int i=0;i<_right_boundary_position_read.size();i++)
    {
        QPointF p=ToDisplay_Correlation(point_2d_t(_right_boundary_position_read[i].p.x,_right_boundary_position_read[i].p.y));
        painter.drawPoint(p);
    }

    //last load area
    if(_lastLoadPoint.isValid)
    {
        pen.setWidthF(0.5*rate);
        pen.setColor(Qt::red);
        painter.setPen(pen);

        QPointF p=ToDisplay_Correlation(_lastLoadPoint.basePoint.pos_xy);
        painter.drawPoint(p);

        pen.setWidthF(0.1*rate);
        pen.setColor(Qt::red);
        painter.setPen(pen);

        for(int k=0;k<_lastLoadPoint.endpoint_xy.size();k++)
        {
            if(k<_lastLoadPoint.endpoint_xy.size()-1)
            {
                QPointF p0=ToDisplay_Correlation(_lastLoadPoint.endpoint_xy[k]);
                QPointF p1=ToDisplay_Correlation(_lastLoadPoint.endpoint_xy[k+1]);
                painter.drawLine(p0,p1);
            }
            else
            {
                QPointF p0=ToDisplay_Correlation(_lastLoadPoint.endpoint_xy[_lastLoadPoint.endpoint_xy.size()-1]);
                QPointF p1=ToDisplay_Correlation(_lastLoadPoint.endpoint_xy[0]);
                painter.drawLine(p0,p1);
            }
        }
    }

//    painter.setBrush(Qt::black);
//    painter.drawRect(rect());

    //manual-calibrate load area
    if(getShowLoadArea()&&_loadPoint.isValid)
    {
        pen.setWidthF(0.5*rate);
        pen.setColor(Qt::blue);
        painter.setPen(pen);

        QPointF p=ToDisplay_Correlation(_loadPoint.basePoint.pos_xy);
        painter.drawPoint(p);

        pen.setWidthF(0.1*rate);
        pen.setColor(Qt::blue);
        painter.setPen(pen);

        for(int k=0;k<_loadPoint.endpoint_xy.size();k++)
        {
            if(k<_loadPoint.endpoint_xy.size()-1)
            {
                QPointF p0=ToDisplay_Correlation(_loadPoint.endpoint_xy[k]);
                QPointF p1=ToDisplay_Correlation(_loadPoint.endpoint_xy[k+1]);
                painter.drawLine(p0,p1);
            }
            else
            {
                QPointF p0=ToDisplay_Correlation(_loadPoint.endpoint_xy[_loadPoint.endpoint_xy.size()-1]);
                QPointF p1=ToDisplay_Correlation(_loadPoint.endpoint_xy[0]);
                painter.drawLine(p0,p1);
            }
        }
    }

    //auto-calibrate load area
    if(/*getShowAutoLoadArea()&&*/_autoLoadPoint.isValid)
    {
        pen.setWidthF(0.5*rate);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);

        QPointF p=ToDisplay_Correlation(_autoLoadPoint.basePoint.pos_xy);
        painter.drawPoint(p);

        pen.setWidthF(0.1*rate);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);

        for(int k=0;k<_autoLoadPoint.endpoint_xy.size();k++)
        {
            if(k<_autoLoadPoint.endpoint_xy.size()-1)
            {
                QPointF p0=ToDisplay_Correlation(_autoLoadPoint.endpoint_xy[k]);
                QPointF p1=ToDisplay_Correlation(_autoLoadPoint.endpoint_xy[k+1]);
                painter.drawLine(p0,p1);
            }
            else
            {
                QPointF p0=ToDisplay_Correlation(_autoLoadPoint.endpoint_xy[_autoLoadPoint.endpoint_xy.size()-1]);
                QPointF p1=ToDisplay_Correlation(_autoLoadPoint.endpoint_xy[0]);
                painter.drawLine(p0,p1);
            }
        }
    }

    //surrd truck
    pen.setWidthF(1*rate);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);

    surrd_vehicle_packet_t _surrdTrucks=getSurrdTrucks();
    for(size_t i=0;i<_surrdTrucks.number;i++)
    {
        QPointF p=ToDisplay_Correlation(point_2d_t(_surrdTrucks.vehicles[i].x,_surrdTrucks.vehicles[i].y));
        painter.drawPoint(p);

        string id (reinterpret_cast<char const*>(_surrdTrucks.vehicles[i].id), 10);
        double dis=sqrt(_surrdTrucks.vehicles[i].x*_surrdTrucks.vehicles[i].x+_surrdTrucks.vehicles[i].y*_surrdTrucks.vehicles[i].y);
        QString stringshow=QString::fromStdString(id)+" "+QString::number(dis)+"m";
        painter.drawText(QPointF(p.x()+15,p.y()),stringshow);
    }



    //reach area
    vector<point_2d_t> reachArea=calcReachShapePos(_reachPoint.pos_xy,_reachPoint.heading);

    pen.setWidthF(0.1*rate);
    pen.setColor(Qt::green);
    painter.setPen(pen);

    for(int k=0;k<reachArea.size();k++)
    {

        if(k<reachArea.size()-1)
        {
            QPointF p0=ToDisplay_Correlation(reachArea[k]);
            QPointF p1=ToDisplay_Correlation(reachArea[k+1]);
            painter.drawLine(p0,p1);
        }
        else
        {
            QPointF p0=ToDisplay_Correlation(reachArea[reachArea.size()-1]);
            QPointF p1=ToDisplay_Correlation(reachArea[0]);
            painter.drawLine(p0,p1);
        }

    }

    //origin point
    pen.setWidthF(0.5*rate);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    QPointF pO=ToDisplay_Correlation(point_2d_t(0,0));
    painter.drawPoint(pO);

    pen.setWidthF(0.1*rate);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    QPointF pI=ToDisplay_Correlation(_reachPoint.pos_xy);
    painter.drawLine(pO,pI);




    QWidget::paintEvent(event);

}

vector<point_3d_t> QDisplayWidget2::getMapBoundaryData()
{
    mutex.lock();
    vector<point_3d_t> temp=mapBoundaryData;
    mutex.unlock();
    return temp;
}

LOAD_POS QDisplayWidget2::getLoadPoint()
{
    mutex.lock();
    LOAD_POS temp=loadPoint;
    mutex.unlock();
    return temp;
}

LOAD_POS QDisplayWidget2::getLastLoadPoint()
{
    mutex.lock();
    LOAD_POS temp=lastLoadPoint;
    mutex.unlock();
    return temp;
}

LOAD_POS QDisplayWidget2::getAutoLoadPoint()
{
    mutex.lock();
    LOAD_POS temp=autoLoadPoint;
    mutex.unlock();
    return temp;
}

DIRC_POINT QDisplayWidget2::getReachPoint()
{
    mutex.lock();
    DIRC_POINT temp=reachPoint;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowLoadArea()
{
    mutex.lock();
    bool temp=showLoadArea;
    mutex.unlock();
    return temp;
}

bool QDisplayWidget2::getShowAutoLoadArea()
{
    mutex.lock();
    bool temp=showAutoLoadArea;
    mutex.unlock();
    return temp;
}

vector<point_lidarmap_t> QDisplayWidget2::getLeftBoundary()
{
    mutex.lock();
    vector<point_lidarmap_t> temp=left_boundary_position_read;
    mutex.unlock();
    return temp;
}

vector<point_lidarmap_t> QDisplayWidget2::getRightBoundary()
{
    mutex.lock();
    vector<point_lidarmap_t> temp=right_boundary_position_read;
    mutex.unlock();
    return temp;
}

surrd_vehicle_packet_t QDisplayWidget2::getSurrdTrucks()
{
    mutex.lock();
    surrd_vehicle_packet_t temp=surrdTrucks;
    mutex.unlock();
    return temp;
}

//vector<point_2d_t> QDisplayWidget2::calcVehShapePos(point_2d_t basePoint, double heading)
//{
//    vector<point_2d_t> points;
//    heading=heading*PI/180;


//    double theta0=heading-angle_f;
//    double theta1=heading+angle_f;
//    double theta2=heading-angle_r+PI;
//    double theta3=heading+angle_r+PI;

//    double x0=basePoint.x+length_f*sin(theta0);
//    double y0=basePoint.y+length_f*cos(theta0);
//    points.push_back(point_2d_t(x0,y0));

//    double x1=basePoint.x+length_f*sin(theta1);
//    double y1=basePoint.y+length_f*cos(theta1);
//    points.push_back(point_2d_t(x1,y1));

//    double x2=basePoint.x+length_r*sin(theta2);
//    double y2=basePoint.y+length_r*cos(theta2);
//    points.push_back(point_2d_t(x2,y2));

//    double x3=basePoint.x+length_r*sin(theta3);
//    double y3=basePoint.y+length_r*cos(theta3);
//    points.push_back(point_2d_t(x3,y3));

//    return points;
//}

vector<point_2d_t> QDisplayWidget2::calcReachShapePos(point_2d_t basePoint, double heading)
{
    vector<point_2d_t> points;
    heading=heading*PI/180;

    double theta0=heading-Rangle_f;
    double theta1=heading+Rangle_f;
    double theta2=heading-Rangle_r+PI;
    double theta3=heading+Rangle_r+PI;

    double x0=basePoint.x+Rlength_f*sin(theta0);
    double y0=basePoint.y+Rlength_f*cos(theta0);
    points.push_back(point_2d_t(x0,y0));

    double x1=basePoint.x+Rlength_f*sin(theta1);
    double y1=basePoint.y+Rlength_f*cos(theta1);
    points.push_back(point_2d_t(x1,y1));

    double x2=basePoint.x+Rlength_r*sin(theta2);
    double y2=basePoint.y+Rlength_r*cos(theta2);
    points.push_back(point_2d_t(x2,y2));

    double x3=basePoint.x+Rlength_r*sin(theta3);
    double y3=basePoint.y+Rlength_r*cos(theta3);
    points.push_back(point_2d_t(x3,y3));

    return points;
}




void QDisplayWidget2::mouseDoubleClick()
{
//    QPointF P=ToDisplay_Correlation(POINT2D(location.x,location.y));
//    offsetx-=P.x()-this->width()/2;
//    offsety-=P.y()-this->height()/2;
    offsetx=0;
    offsety=0;
}

void QDisplayWidget2::ZoomIn()
{
    mutex.lock();
    rate*=1.25;
    mutex.unlock();
}

void QDisplayWidget2::ZoomOut()
{
    mutex.lock();
    rate/=1.25;
    mutex.unlock();
}

void QDisplayWidget2::setShowLoadArea(bool _flag)
{
    mutex.lock();
    showLoadArea=_flag;
    mutex.unlock();
}

void QDisplayWidget2::setShowAutoLoadArea(bool _flag)
{
    mutex.lock();
    showAutoLoadArea=_flag;
    mutex.unlock();
}

void QDisplayWidget2::setMapBoundaryData(vector<point_3d_t> _mapBoundaryData)
{
    mutex.lock();
    mapBoundaryData=_mapBoundaryData;
    mutex.unlock();
}

void QDisplayWidget2::setLoadPoint(LOAD_POS _loadPoint)
{
    mutex.lock();
    loadPoint=_loadPoint;
    mutex.unlock();
}

void QDisplayWidget2::setLastLoadPoint(LOAD_POS _loadPoint)
{
    mutex.lock();
    lastLoadPoint=_loadPoint;
    mutex.unlock();
}

void QDisplayWidget2::setAutoLoadPoint(LOAD_POS _autoLoadPoint)
{
    mutex.lock();
    autoLoadPoint=_autoLoadPoint;
    mutex.unlock();
}

void QDisplayWidget2::setReachPoint(DIRC_POINT _reachPoint)
{
    mutex.lock();
    reachPoint=_reachPoint;
    mutex.unlock();
}

void QDisplayWidget2::setLeftBoundary(vector<point_lidarmap_t> _left_boundary_position_read)
{
    mutex.lock();
    left_boundary_position_read=_left_boundary_position_read;
    mutex.unlock();
}

void QDisplayWidget2::setRightBoundary(vector<point_lidarmap_t> _right_boundary_position_read)
{
    mutex.lock();
    right_boundary_position_read=_right_boundary_position_read;
    mutex.unlock();
}

void QDisplayWidget2::setSurrdTrucks(surrd_vehicle_packet_t _surrdTrucks)
{
    mutex.lock();
    surrdTrucks=_surrdTrucks;
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

QPointF QDisplayWidget2::ToDisplay_Correlation(point_2d_t inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((inputPoint.x*rate+displayRect.width()/2.0)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2.0)+offsety);

    return outputPoint;
}

QPointF QDisplayWidget2::ToDisplay_Correlation(point_3d_t inputPoint)
{
    QPointF outputPoint;

    outputPoint.setX((inputPoint.x*rate+displayRect.width()/2.0)+offsetx);
    outputPoint.setY((-inputPoint.y*rate+displayRect.height()/2.0)+offsety);

    return outputPoint;
}

point_2d_t QDisplayWidget2::CounterDisplay_Correlation(QPointF inputPoint)
{
    point_2d_t outputPoint;

    outputPoint.x=(inputPoint.x()-offsetx-displayRect.width()/2.0)/rate;
    outputPoint.y=-(inputPoint.y()-offsety-displayRect.height()/2.0)/rate;

    return outputPoint;
}

