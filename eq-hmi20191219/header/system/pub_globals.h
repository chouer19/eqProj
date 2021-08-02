#ifndef PUB_GLOBALS_H_
#define PUB_GLOBALS_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "pub_types.h"
#include "header/drive/dri_interface.hpp"
#include "sys_interface.hpp"
#include "header/protocol/pro_interface.h"
#include "header/algorithm/alg_interface.hpp"


/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */
#ifdef PUB_GLOBALS
    #define PUB_EXT
#else
    #define PUB_EXT extern
#endif

#ifdef SYS_GLOBALS
    #define SYS_EXT
#else
    #define SYS_EXT extern
#endif

#ifdef DRI_GLOBALS
    #define DRI_EXT
#else
    #define DRI_EXT extern
#endif

#ifdef PRO_GLOBALS
    #define PRO_EXT
#else
    #define PRO_EXT extern
#endif

#ifdef ALG_GLOBALS
    #define ALG_EXT
#else
    #define ALG_EXT extern
#endif

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

/* System globals */
SYS_EXT SYSTEM *sys;

/* Drive globals */
DRI_EXT DRIVE *drive;

/* Protocol globals */
PRO_EXT PROTOCOL *protocol;

/* Algorithm globals */
ALG_EXT ALGORITHM *algorithm;
/*
 * =====================================================================
 * Global function prototypes ('extern', definition&comment in C source)
 * =====================================================================
 */

#endif /* PUB_GLOBALS_H_ */
