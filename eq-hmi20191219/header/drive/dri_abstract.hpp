
#ifndef DRI_ABSTRACT_H_
#define DRI_ABSTRACT_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */

#include "header/system/pub_types.h"
#include "dri_interface.hpp"
#include "dri_serialport.hpp"
#include"dri_file_txt.h"


/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */


enum{ CAN_NUM = 1};


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

/*  board init   */
extern bool initDrive(void);





#endif /* DRI_ABSTRACT_H_ */
