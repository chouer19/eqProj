#include "header/algorithm/alg_speed_plan.h"

ISPEEDPLAN_BILL::ISPEEDPLAN_BILL(SYSTEM *_sys,PROTOCOL *_pro, ILONCTRL *_lon,RUN_CORE _rcore)
    :ISPEEDPLAN(pub_time, TASK_PERIOD_SPEEDPLAN)
    , sys(_sys)
    , pro(_pro)
    , lonctrl(_lon)
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

ISPEEDPLAN_BILL::~ISPEEDPLAN_BILL()
{
    abort();
    sys->taskList_p[run_core]->erase(taskId);
    m_isAlive = false;
    m_thread.join();

}

void ISPEEDPLAN_BILL::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[run_core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);


}
bool ISPEEDPLAN_BILL::active()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_to_control = true;
    m_activated = true;
}
void ISPEEDPLAN_BILL::deactive()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_to_control = false;
    m_activated = false;
}
bool ISPEEDPLAN_BILL::activated()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_activated;
}
uint8_t ISPEEDPLAN_BILL::error()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_error_code;
}

void ISPEEDPLAN_BILL::setPath(path_pt_t _path)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_path=_path;
}

void ISPEEDPLAN_BILL::setDirection(DIRECTION _dir)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    direction=_dir;
}

void ISPEEDPLAN_BILL::setSetSpeed(double _setSpeed)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    setSpeed=_setSpeed;
}

path_pt_t ISPEEDPLAN_BILL::getPath()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_path;
}

DIRECTION ISPEEDPLAN_BILL::getDirection()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return direction;
}

double ISPEEDPLAN_BILL::getSetSpeed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return setSpeed;
}

void ISPEEDPLAN_BILL::err_check()
{

}

void ISPEEDPLAN_BILL::task_func()
{
    speed_proc_bill();
}


void ISPEEDPLAN_BILL::speed_proc_bill()
{
    double _currentSpeed = pro->vehicle->getVspd();
    path_pt_t _path=getPath();
    DIRECTION _direction=getDirection();
    double _setSpeed= getSetSpeed();

    if(_direction==FORWARD)
    {
        if(_currentSpeed<=0.15/3.6)
            maxSetAcc=1;
        else
            maxSetAcc=0.1;
    }
    else
        maxSetAcc=0.1;


    if(_currentSpeed<=2/3.6)
        minSetNormalAcc=-2;
    else
        minSetNormalAcc=-0.5;

    if(_path.pt.size()>0)
    {
        if(_currentSpeed>_setSpeed)
        {

            double deacc=MAX(-2.5,MIN(0,-(_currentSpeed-_setSpeed)*3.6));

            for(int i=0;i<_path.pt.size();i++)
            {
                _path.pt[i].vspd=sqrt(MAX(2*deacc*_path.pt[i].distance+_currentSpeed*_currentSpeed,_setSpeed));
            }
        }
        else
        {

            double acc=MIN(maxSetAcc,MAX(0,(_setSpeed-_currentSpeed)*0.22));
            for(int i=0;i<_path.pt.size();i++)
            {
                 _path.pt[i].vspd=MIN(sqrt(MAX(0.0,2*acc*_path.pt[i].distance+_currentSpeed*_currentSpeed)),_setSpeed);
            }

        }

    }
    else
    {
        lonctrl->setAimAcc(-4.5);
    }






}


