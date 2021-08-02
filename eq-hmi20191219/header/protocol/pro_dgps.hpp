#ifndef PRO_DGPS_HPP
#define PRO_DGPS_HPP

#include"header/system/pub_types.h"
#include "header/system/sys_interface.hpp"
#include "header/drive/dri_interface.hpp"
#include "pro_interface.h"


enum { TASK_PERIOD_DGPS = 10 };

class IDGPS_INS_D : implements IDGPS
{
public:
    IDGPS_INS_D(DRIVE *_dri,IVEH *_veh, SYSTEM *_sys, RUN_CORE _core)
        :IDGPS(pub_time, TASK_PERIOD_DGPS)
        ,dri(_dri)
        ,veh(_veh)
        ,sys(_sys)
        ,core(_core)
        ,m_error_code(0)
        ,m_over_time_duty(0)
        ,m_task_over_time_cnt(0)
    {
        for(uint8_t i = 0; i < 25; i++)
            m_over_time_record[i] = 0;

    }
    virtual ~IDGPS_INS_D()
    {
        abort();
        sys->taskList_p[core]->erase(taskId);
        m_isAlive = false;
        m_thread.join();
    }

    virtual void install();

    virtual void parse(void *frm);

    virtual point_3d_t position();

    virtual float heading();
    virtual float pitch();
    virtual float pitchAver();
    virtual float roll();

    virtual float headingSTD();
    virtual float pitchSTD();
    virtual float rollSTD();

    virtual double latitude();
    virtual double longitude();
    virtual double altitude();

    virtual double latitudeSTD();
    virtual double longitudeSTD();
    virtual double altitudeSTD();

    virtual float vsdNorth();
    virtual float vsdEast();
    virtual float vsdUp();

    virtual float vsdNorthSTD();
    virtual float vsdEastSTD();
    virtual float vsdUpSTD();

    virtual string RTKStatus();
    virtual string AlignStatus();
    virtual uint8_t anglesPositionType();

    virtual float yaw_rate();
    virtual float roll_rate();
    virtual float pitch_rate();

    virtual float lon_accel();
    virtual float lat_accel();
    virtual float alt_accel();

    virtual nav_t fram_data();
    virtual uint8_t imuWorkStatus();
    virtual vector<nav_t> fram_data_1s();


    TaskFuncDeclare(IDGPS_INS_D, dgps, (TASK_PERIOD_DGPS+10))

private:
    DRIVE *dri;
    SYSTEM *sys;
    IVEH *veh;
    RUN_CORE core;

    nav_t frm_data;
    vector<nav_t> frm_data_1s;
    std::mutex m_mutex;

    point_3d_t blh2xyz(double B, double L, double H);
    double getLOInDegree(double dLIn);
    point_3d_t Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData);


    qint64 lastSendTick=0;
    vector<double> pitchlist;
    double m_pitchAver;
};

#endif // PRO_DGPS_HPP
