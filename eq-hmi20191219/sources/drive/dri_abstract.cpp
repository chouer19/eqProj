
/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include "header/drive/dri_abstract.hpp"
#define DRI_GLOBALS
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
DRIVE::DRIVE()
{


    rs232_ins_d_com1 = new(std::nothrow) ISERIALPORT_INS_D_COM1(COM_CHAN0, ttyUSB0);
    txt=new(std::nothrow)IFILE_TXT();



    bool success = ((NULL != can[0]) && (NULL != rs232_ins_d_com1)&&(txt!=NULL));
	
	if(success)
	{

//        for(uint8_t i = 0; i < CAN_NUM; i++)
//		{
//			can[i]->init();
//            can[i]->installIntcFunc();

//            can[i]->install();
//            can[i]->start();

//		}

//        rs232_ins_d_com1->init();
//        rs232_ins_d_com1->open();

	}
	else
	{


        delete rs232_ins_d_com1;
        rs232_ins_d_com1 = NULL;
        delete txt;
        txt=NULL;

	}
	
}

DRIVE::~DRIVE()
{

    delete rs232_ins_d_com1;
    rs232_ins_d_com1 = NULL;
    delete txt;
    txt=NULL;

}

/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */

/* board init */
bool initDrive(void)
{
    drive = new(std::nothrow) DRIVE();
    if(NULL == drive)
	{

		return false;
	}
	
	return true;
}
