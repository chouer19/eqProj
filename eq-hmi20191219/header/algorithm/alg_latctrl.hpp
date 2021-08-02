#ifndef ALG_LATCTRL_H_
#define ALG_LATCTRL_H_

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

enum{ YX_COE_NUM = 4 };

enum{ TASK_PERIOD_LATCTRL = 50 };

enum
{
	ERR_LAT_DYNA_BIT = 0,
	ERR_EPS_BIT = 1,
	ERR_PATH_BIT = 2	
};

enum{ OVER_TIME_LATCTRL = 100 };


class ILATCTRL_TLI65 : implements ILATCTRL
{    
public:
    ILATCTRL_TLI65(SYSTEM *_sys, PROTOCOL *_pro, RUN_CORE _rcore);
    virtual ~ILATCTRL_TLI65();
	
	virtual void install();
    virtual void setToControl(bool _to_con)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_to_control = _to_con? CON_ON: CON_OFF;
    }
    virtual void setDirection(DIRECTION dir);
    virtual err_t error();
	
    TaskFuncDeclare(ILATCTRL_TLI65, latctrl, (TASK_PERIOD_LATCTRL+10))


private:
    SYSTEM *sys;
    PROTOCOL *pro;

    RUN_CORE m_run_core;
	
    DIRECTION direction;

    CON_SWITCH m_to_control;

    void error_proc(const CON_SWITCH &_switch);



#ifdef __linux
    std::mutex m_mutex;
#endif
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



/*
 * =====================================================================
 * Global function prototypes ('extern', definition&comment in C source)
 * =====================================================================
 */



#endif /* ALG_LATCTRL_H_ */
