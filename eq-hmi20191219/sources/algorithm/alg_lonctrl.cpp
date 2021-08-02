/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include "header/algorithm/alg_lonctrl.hpp"
#include <cmath>
#include <thread>


/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */


/*
 * =====================================================================
 * Global type definitions ('typedef')
 * =====================================================================
 */


/*
 * =====================================================================
 * Global constant definitions
 * =====================================================================
 */

/*
 * =====================================================================
 * Private constant definitions ('static')
 * =====================================================================
 */
static QTime t1;

/*
 * =====================================================================
 * Global variable definitions
 * =====================================================================
 */



/*
 * =====================================================================
 * Private variable variable definitions('static')
 * =====================================================================
 */


/*
 * =====================================================================
 * Private function prototypes ('static', comment on the definitions)
 * =====================================================================
 */
/*
 * =====================================================================
 * Private function definitions ('static')
 * =====================================================================
 */
ILONCTRL_TLI65::ILONCTRL_TLI65(SYSTEM *_sys, PROTOCOL *_pro, RUN_CORE _rcore)
:ILONCTRL(pub_time, TASK_PERIOD_LONCTRL)
, sys(_sys)
, pro(_pro)
, m_run_core(_rcore)
, m_to_control(CON_OFF)
, m_error_code(0)
, m_isAlive(false)
, m_task_over_time_cnt(0)
, m_over_time_duty(0)
{

    for(uint8_t i = 0; i < 25; i++)
        m_over_time_record[i] = 0;
}

ILONCTRL_TLI65::~ILONCTRL_TLI65()
{
    abort();
    sys->taskList_p[m_run_core]->erase(taskId);
    m_isAlive = false;
    m_thread.join();
}


void ILONCTRL_TLI65::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[m_run_core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);
}

void ILONCTRL_TLI65::setDirection(DIRECTION dir)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    direction=dir;
}

void ILONCTRL_TLI65::setAimAcc(double _aimAcc)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    aimAcc=_aimAcc;
}

err_t ILONCTRL_TLI65::error()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_error_code;
}


void ILONCTRL_TLI65::task_func()
{

    err_check();


}


void ILONCTRL_TLI65::err_check()
{

}

void ILONCTRL_TLI65::error_proc()
{

}

bool ILONCTRL_TLI65::getToControl()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return (m_to_control==CON_ON);
}

DIRECTION ILONCTRL_TLI65::getDirection()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return direction;
}

double ILONCTRL_TLI65::getAimAcc()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return aimAcc;
}


/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */
