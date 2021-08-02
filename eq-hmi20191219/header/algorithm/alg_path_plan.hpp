#ifndef ALG_PATH_PLAN_HPP
#define ALG_PATH_PLAN_HPP


#include "header/system/pub_types.h"
#include "alg_interface.hpp"
#include "header/system/sys_interface.hpp"
#include"header/protocol/pro_interface.h"

enum{ TASK_PERIOD_PATHPLAN = 50 };

class IPATHPLAN_BILL: implements IPATHPLAN
{
public:
    IPATHPLAN_BILL(SYSTEM *_sys, PROTOCOL *_pro, ISPEEDPLAN *_speed,RUN_CORE _rcore);
    virtual ~IPATHPLAN_BILL();

    virtual void install();
    virtual bool active();
    virtual void deactive();
    virtual bool activated();
    virtual uint8_t error();
    virtual void setDirection(DIRECTION _dir);

    TaskFuncDeclare(IPATHPLAN_BILL, pathplan, (TASK_PERIOD_PATHPLAN+10))

private:
    void path_proc_bill();

    SYSTEM *sys;
    PROTOCOL *pro;
    ISPEEDPLAN *speed;

    RUN_CORE run_core;
    bool m_to_control;
    bool m_activated;
    path_pt_t m_path;

    std::mutex m_mutex;

    DIRECTION direction=FORWARD;
    DIRECTION getDirection();

};


#endif // ALG_PATH_PLAN_HPP
