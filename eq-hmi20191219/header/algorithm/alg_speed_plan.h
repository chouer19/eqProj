#ifndef ALG_SPEED_PLAN_H
#define ALG_SPEED_PLAN_H


#include "header/system/pub_types.h"
#include "alg_interface.hpp"
#include "header/system/sys_interface.hpp"
#include"header/protocol/pro_interface.h"


enum{ TASK_PERIOD_SPEEDPLAN = 50 };

class ISPEEDPLAN_BILL: implements ISPEEDPLAN
{
public:
    ISPEEDPLAN_BILL(SYSTEM *_sys, PROTOCOL *_pro, ILONCTRL *_lonctrl, RUN_CORE _rcore);
    virtual ~ISPEEDPLAN_BILL();

    virtual void install();
    virtual bool active();
    virtual void deactive();
    virtual bool activated();
    virtual uint8_t error();

    virtual void setPath(path_pt_t _path);
    virtual void setDirection(DIRECTION _dir);
    virtual void setSetSpeed(double _setSpeed);

    TaskFuncDeclare(ISPEEDPLAN_BILL, speedplan, (TASK_PERIOD_SPEEDPLAN+10))

private:
    void speed_proc_bill();

private:   
    SYSTEM *sys;
    PROTOCOL *pro;
    ILONCTRL *lonctrl;
    RUN_CORE run_core;
    bool m_to_control;
    bool m_activated;

    path_pt_t m_path;
    path_pt_t getPath();

    DIRECTION direction=FORWARD;
    DIRECTION getDirection();

    double setSpeed=0;
    double getSetSpeed();

    double maxSetAcc=0.1;//车辆加速度
    double minSetNormalAcc=-0.5;//减速度
    const double systemMaxSpeed=40/3.6;

    std::mutex m_mutex;

};
#endif // ALG_SPEED_PLAN_H
