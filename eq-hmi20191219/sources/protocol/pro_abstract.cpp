
/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
//#include "header/protocol/pro_dgps.hpp"
//#include "header/protocol/pro_gnss_receiver.h"
//#include"header/protocol/pro_vehicle.h"
#include"header/protocol/pro_abstract.hpp"
#define PRO_GLOBALS
#include"header/system/pub_globals.h"



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
PROTOCOL::PROTOCOL()
:vehicle(NULL),dgps(NULL)
{
    vehicle = new(std::nothrow)IVEH_TLI65(drive,sys,RUN_CORE_P1);
    dgps = new(std::nothrow) IDGPS_INS_D(drive,vehicle,sys, RUN_CORE_P1);
    gnss_receiver=new(std::nothrow) IGNSS_RECEIVER_718D(drive,sys,RUN_CORE_P1);

    if((NULL != dgps) && (NULL != vehicle)&& (NULL != gnss_receiver))
	{
		
	}
	else
	{
        delete vehicle;
        vehicle = NULL;
        delete dgps;
        dgps = NULL;
        delete gnss_receiver;
        gnss_receiver = NULL;

	}
	
}

PROTOCOL::~PROTOCOL()
{
    delete vehicle;
    vehicle = NULL;
    delete dgps;
    dgps = NULL;
    delete gnss_receiver;
    gnss_receiver = NULL;
}


/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */

bool initProtocol(void)
{
    protocol = new(std::nothrow) PROTOCOL();
    if(NULL == protocol)
		return false;
	else
		return true;
}

