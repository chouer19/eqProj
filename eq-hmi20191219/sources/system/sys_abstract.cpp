/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include"header/system/sys_abstract.hpp"
#define SYS_GLOBALS
#include "header/system/pub_globals.h"
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
SYSTEM::SYSTEM()
{
	taskList_p[0] = NULL;
	taskList_p[1] = NULL; 
    taskList_p[2] = NULL;

    taskList_p[0] = new(std::nothrow) TASKLIST(RUN_CORE_P0);
    taskList_p[1] = new(std::nothrow) TASKLIST(RUN_CORE_P1);
    taskList_p[2] = new(std::nothrow) TASKLIST(RUN_CORE_P2);

    telecom=new(std::nothrow)ITELECOM_AUXILIARY(taskList_p[0]);

    if((NULL != taskList_p[0])
            && (NULL != taskList_p[1])
            && (NULL != taskList_p[2]))
	{
		taskList_p[0]->clear();
		taskList_p[1]->clear();
        taskList_p[2]->clear();
	}
	else
	{
		delete taskList_p[0];
		taskList_p[0] = NULL;
		delete taskList_p[1];
        taskList_p[1] = NULL;
        delete taskList_p[2];
        taskList_p[2] = NULL;

	}
}

SYSTEM::~SYSTEM()
{
    delete taskList_p[0];
    taskList_p[0] = NULL;
    delete taskList_p[1];
    taskList_p[1] = NULL;
    delete taskList_p[2];
    taskList_p[2] = NULL;

}

/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */
bool initSystem(void)
{
    sys = new(std::nothrow) SYSTEM();
    if(NULL == sys)
		return false;
	else
		return true;
	
}
