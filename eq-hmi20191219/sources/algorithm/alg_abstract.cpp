
/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include "header/algorithm/alg_abstract.hpp"
#define ALG_GLOBALS
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
ALGORITHM::ALGORITHM()
:lonctrl(NULL), latctrl(NULL)
{
    lonctrl = new(std::nothrow) ILONCTRL_TLI65(sys, protocol, RUN_CORE_P2);
    latctrl = new(std::nothrow) ILATCTRL_TLI65(sys, protocol, RUN_CORE_P2);
    speed   = new(std::nothrow) ISPEEDPLAN_BILL(sys,protocol,lonctrl,RUN_CORE_P2);
    path    = new(std::nothrow) IPATHPLAN_BILL(sys,protocol,speed,RUN_CORE_P2);
    if((NULL == lonctrl) || (NULL == latctrl)|| (NULL == speed)||(NULL ==path))
	{

		delete lonctrl;
		lonctrl = NULL;
		delete latctrl;
		latctrl = NULL;
        delete speed;
        speed=NULL;
        delete path;
        path=NULL;
	}
}

ALGORITHM::~ALGORITHM()
{
    delete lonctrl;
    lonctrl = NULL;
    delete latctrl;
    latctrl = NULL;
    delete speed;
    speed=NULL;
    delete path;
    path=NULL;
}

/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */



bool initAlgorithm(void)
{
    algorithm = new(std::nothrow) ALGORITHM();
    if(NULL == algorithm)
		return false;
	else
		return true;
}
