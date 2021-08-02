#ifndef PRO_GNSS_RECEIVER_H
#define PRO_GNSS_RECEIVER_H

#include"pro_interface.h"
#include "header/system/sys_interface.hpp"
#include "header/drive/dri_interface.hpp"

enum { TASK_PERIOD_GNSS_RECEIVER = 10 };

using namespace GeographicLib;

class IGNSS_RECEIVER_718D:implements IGNSS_RECEIVER
{
public:
    IGNSS_RECEIVER_718D(DRIVE *_dri, SYSTEM *_sys, RUN_CORE _core);
    ~IGNSS_RECEIVER_718D();

    virtual void install();
    virtual void parse();
    nav_t getGnssOutput();
    nav_t getReferencePos();
    virtual nav_t getBasePoint();
    virtual void setBasePointOffset(point_3d_t _point);
    virtual void setRefPointOffset(point_3d_t _point);
    virtual void setTerminalInfo(TERMINAL_INFO _info);

    TaskFuncDeclare(IGNSS_RECEIVER_718D, gnss, (TASK_PERIOD_GNSS_RECEIVER+10))
private:
    DRIVE *dri;
    SYSTEM *sys;
    RUN_CORE core;
    std::mutex m_mutex;

    nav_t gnssOutput;
    void setGnssOutput(nav_t _gnssOutput);

    nav_t basePoint;
    void setBasePoint(nav_t _basePoint);

    utc_time UTCToBeijing(utc_time _utc_time);
    point_3d_t Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData);

    nav_t referencePos;
    void setReferencePos(nav_t _referencePos);

//    double lat0=39.8825;
//    double lon0=110.257;
//    double alt0=1376.49;

    point_3d_t basePointOffset;
    point_3d_t getBasePointOffset();

    point_3d_t refPointOffset;
    point_3d_t getRefPointOffset();

//    double lonoffset_buckget=8.46;
//    double latoffset_buckget=-1.34;
//    double altoffset_buckget=0;

//    double lonoffset_basepoint=1.66;
//    double latoffset_basepoint=-1.34;
//    double altoffset_basepoint=-2;

    bool getParasSet();
    bool paras_set=false;

    TERMINAL_INFO terminalInfo;
    TERMINAL_INFO getTerminalInfo();
};

#endif // PRO_GNSS_RECEIVER_H
