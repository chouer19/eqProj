/**
 * File Name: svc_task.c
 * Create on: Jul 31, 2017
 *    Author: LiFei
 *   Version: V1.0, Jul 31, 2017, LiFei
 */

/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include"header/system/sys_task.hpp"

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
TASKLIST::TASKLIST(RUN_CORE _c)
    :m_core(_c)
{
    taskList = new List<TASK*>();
}

TASKLIST::~TASKLIST()
{
    delete taskList;
}

bool TASKLIST::empty()
{
	return taskList->empty();
}

Node<TASK*>* TASKLIST::push_back(TASK *data)
{
	return taskList->push_back(data);
}
void TASKLIST::erase(Node<TASK*>* pos)
{
	taskList->erase(pos);
}
TASK* TASKLIST::data(Node<TASK*>* pos)
{
	return taskList->data(pos);
}
void TASKLIST::clear()
{
	taskList->clear();
}
size_t TASKLIST::size()
{
	return taskList->size();
}

void TASKLIST::exec()
{
	for(Node<TASK*> *_p = taskList->front(); _p != NULL; _p = _p->_pNext)
	{
		if(_p->_data->started())
        {
			size_t startTime = _p->_data->getStartTime();
            uint8_t period = _p->_data->periodTime();
            if(_p->_data->time->getDiffTimeMs(startTime) >= period)
            {
                _p->_data->task_start();
                //_p->_data->setStartTime(endTime);
			}
//			else if(endTime < startTime)
//			{
//				if((SIZE_MAX_VALUE - startTime) + endTime >= period)
//				{
//                    _p->_data->task_start();
//					_p->_data->setStartTime(endTime);
//				}
//			}
            else{ }
		}
		else
		{
			_p->_data->setStartTime(0);
			_p->_data->setEndTime(0);
		}	
	}
}


/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */

