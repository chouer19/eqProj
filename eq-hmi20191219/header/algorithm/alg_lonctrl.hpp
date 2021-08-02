#ifndef ALG_LONCTRL_H_
#define ALG_LONCTRL_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "header/system/pub_types.h"
#include "alg_interface.hpp"
#include "header/system/sys_interface.hpp"
#include "header/protocol/pro_interface.h"

/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */

enum{ VX_COE_NUM = 4 };



enum{ OVER_TIME_LONCTRL = 80 };

enum{ TASK_PERIOD_LONCTRL = 50 };

enum
{
	ERR_LON_DYNA_BIT = 0,
	ERR_EBS_BIT = 1,
	ERR_PWRTRAIN_BIT = 2
};

enum
{
    EMERGENCY_BRAKE_I = -1,
    EMERGENCY_BRAKE_II = -2,
    EMERGENCY_BRAKE_III = -3,
    EMERGENCY_BRAKE_IV = -4,
    EMERGENCY_BRAKE_V = -5,
    EMERGENCY_BRAKE_VI = -6
};
/*
 * =====================================================================
 * Global type definitions ('typedef')
 * =====================================================================
 */
class ILONCTRL_TLI65 : implements ILONCTRL
{
public:
    ILONCTRL_TLI65(SYSTEM *_sys, PROTOCOL *_pro, RUN_CORE _rcore);
    virtual ~ILONCTRL_TLI65();
	
	virtual void install();
    virtual void setToControl(bool _to_con)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_to_control = _to_con? CON_ON: CON_OFF;
    }
    virtual void setDirection(DIRECTION dir);
    virtual void setAimAcc(double _aimAcc);
    virtual err_t error();
	
    TaskFuncDeclare(ILONCTRL_TLI65, lonctrl, (TASK_PERIOD_LONCTRL+10))

private:

    void error_proc();

    SYSTEM *sys;
    PROTOCOL *pro;
    RUN_CORE m_run_core;

    CON_SWITCH m_to_control;
    bool getToControl();

    DIRECTION direction=FORWARD;
    DIRECTION getDirection();

    double aimAcc=0;
    double getAimAcc();

    std::mutex m_mutex;

};

/*
 * =====================================================================
 * Global constant declarations ('extern', definiton&comment in C source)
 * =====================================================================
 */


/*
 * =====================================================================
 * Global variable declarations ('extern', definition&comment in C source)
 * =====================================================================
 */

#endif /* ALG_LONCTRL_H_ */
