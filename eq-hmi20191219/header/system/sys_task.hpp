/**
 * File Name: svc_task.h
 * Create on: Jul 31, 2017
 *    Author: LiFei
 *   Version: V1.0, Jul 31, 2017, LiFei
 */

#ifndef SVC_TASK_H_
#define SVC_TASK_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "pub_types.h"
#include "sys_interface.hpp"


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
class TASKLIST : implements ITASKLIST
{
public:
    TASKLIST(RUN_CORE _c);
	~TASKLIST();
	
	virtual bool empty(); 
	virtual Node<TASK*>* push_back(TASK *data);
	virtual void erase(Node<TASK*>* pos);
	virtual TASK* data(Node<TASK*>* pos);
	virtual void clear();
    virtual size_t size();
    virtual RUN_CORE getCore() {return m_core;}
    virtual void exec();

private:
    List<TASK*> *taskList;
    PropertyDeclare(RUN_CORE, core, setCore)
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



#endif /* SVC_TASK_H_ */
