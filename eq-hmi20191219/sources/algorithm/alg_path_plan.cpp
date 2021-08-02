#ifndef ALG_PATH_PLAN_CPP
#define ALG_PATH_PLAN_CPP
#include "header/algorithm/alg_path_plan.hpp"

IPATHPLAN_BILL::IPATHPLAN_BILL(SYSTEM *_sys, PROTOCOL *_pro, ISPEEDPLAN *_speed,RUN_CORE _rcore)
    :IPATHPLAN(pub_time, TASK_PERIOD_PATHPLAN)
    , sys(_sys)
    , pro(_pro)
    , speed(_speed)
    , run_core(_rcore)
    , m_to_control(true)
    , m_activated(false)
    , m_error_code(0)
    , m_isAlive(false)
    , m_over_time_duty(0)
    , m_task_over_time_cnt(0)
{

    for(uint8_t i = 0; i < 25; i++)
        m_over_time_record[i] = 0;
}

IPATHPLAN_BILL::~IPATHPLAN_BILL()
{
    abort();
    sys->taskList_p[run_core]->erase(taskId);
    m_isAlive = false;
    m_thread.join();

}

void IPATHPLAN_BILL::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[run_core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);


}
bool IPATHPLAN_BILL::active()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_to_control = true;
    m_activated = true;
}
void IPATHPLAN_BILL::deactive()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_to_control = false;
    m_activated = false;
}
bool IPATHPLAN_BILL::activated()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_activated;
}
uint8_t IPATHPLAN_BILL::error()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_error_code;
}

void IPATHPLAN_BILL::setDirection(DIRECTION _dir)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    direction=_dir;
}

void IPATHPLAN_BILL::err_check()
{

}

void IPATHPLAN_BILL::task_func()
{

    path_proc_bill();

}


void IPATHPLAN_BILL::path_proc_bill()
{
    path_pt_t _path;
    double sumDis=0;
    for(int i=0;i<500;i++)
    {
        path_t p;
        if(getDirection()==FORWARD)
            p.x=i*0.1;
        else
            p.x=-i*0.1;

        p.y=0;
        p.z=0;
        p.distance=sumDis;
        _path.pt.push_back(p);

        sumDis+=0.1;

    }
    _path.time_stamp=pub_time->getTimeMs();

    speed->setPath(_path);

}

DIRECTION IPATHPLAN_BILL::getDirection()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return direction;
}


#endif // ALG_PATH_PLAN_CPP
