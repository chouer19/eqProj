#ifndef PRO_ABSTRACT_H_
#define PRO_ABSTRACT_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "header/system/pub_types.h"
#include "pro_interface.h"
#include "header/drive/dri_interface.hpp"
#include "header/system/sys_interface.hpp"
#include"pro_vehicle.h"
#include"pro_dgps.hpp"
#include"pro_gnss_receiver.h"

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
extern bool initProtocol(void);



#endif /* PRO_ABSTRACT_H_ */
