#ifndef QDISPLAYWIDGET2_H
#define QDISPLAYWIDGET2_H

#include"qdisplaywidget1.h"
#include"QMutex"



class QDisplayWidget2 : public QDisplayWidget1
{
    Q_OBJECT
public:
    QDisplayWidget2(QWidget *parent = nullptr);

    virtual void mouseDown();
    virtual void mouseMove();
    virtual void mouseUp();
    virtual void mouseDoubleClick();
    void ZoomIn();
    void ZoomOut();
    void setShowLoadArea(bool _flag);
    void setShowAutoLoadArea(bool _flag);

    void setMapBoundaryData(vector<point_3d_t> _mapBoundaryData);

    void setLoadPoint(LOAD_POS _loadPoint);
    void setLastLoadPoint(LOAD_POS _loadPoint);     
    void setAutoLoadPoint(LOAD_POS _autoLoadPoint);
    void setReachPoint(DIRC_POINT _reachPoint);

    void setLeftBoundary(vector<point_lidarmap_t> _left_boundary_position_read);
    void setRightBoundary(vector<point_lidarmap_t> _right_boundary_position_read);
    void setSurrdTrucks(surrd_vehicle_packet_t _surrdTrucks);


private:
    virtual QPointF ToDisplay_Correlation(point_2d_t inputPoint);
    virtual QPointF ToDisplay_Correlation(point_3d_t inputPoint);
    virtual point_2d_t CounterDisplay_Correlation(QPointF inputPoint);
    virtual void paintEvent(QPaintEvent *event);

    bool isMouseMove=false;
    bool isSetMiddle=false;
    float rate=15;
    float offsetx=0;
    float offsety=0;
    float originalx;
    float originaly;
    float originalfx;
    float originalfy;
    QRect displayRect;
    QMutex mutex;

    vector<point_3d_t> mapBoundaryData;
    vector<point_3d_t> getMapBoundaryData();

    LOAD_POS loadPoint;
    LOAD_POS getLoadPoint();

    LOAD_POS lastLoadPoint;
    LOAD_POS getLastLoadPoint();

    LOAD_POS autoLoadPoint;
    LOAD_POS getAutoLoadPoint();


    DIRC_POINT reachPoint;
    DIRC_POINT getReachPoint();

    bool showLoadArea=false;
    bool getShowLoadArea();

    bool showAutoLoadArea=false;
    bool getShowAutoLoadArea();

    vector<point_lidarmap_t> left_boundary_position_read;
    vector<point_lidarmap_t> getLeftBoundary();

    vector<point_lidarmap_t> right_boundary_position_read;
    vector<point_lidarmap_t> getRightBoundary();

    surrd_vehicle_packet_t surrdTrucks;
    surrd_vehicle_packet_t getSurrdTrucks();

//    const double length_f=sqrt(pow(0.5*lat,2)+pow(lonF,2));
//    const double length_r=sqrt(pow(0.5*lat,2)+pow(lonR,2));
//    const double angle_f=atan(0.5*lat/lonF);
//    const double angle_r=atan(0.5*lat/lonR);


    const double RlonF=0.75;
    const double RlonR=0.75;
    const double Rlat=1.5;
    const double Ralt=0.5;

    const double Rlength_f=sqrt(pow(0.5*Rlat,2)+pow(RlonF,2));
    const double Rlength_r=sqrt(pow(0.5*Rlat,2)+pow(RlonR,2));
    const double Rangle_f=atan(0.5*Rlat/RlonF);
    const double Rangle_r=atan(0.5*Rlat/RlonR);

//    vector<point_2d_t> calcVehShapePos(point_2d_t basePoint,double heading);
    vector<point_2d_t> calcReachShapePos(point_2d_t basePoint,double heading);

};

#endif // QDISPLAYWIDGET2_H
