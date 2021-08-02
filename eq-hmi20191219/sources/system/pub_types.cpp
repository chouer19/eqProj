/**
 * File Name: pub_types.c
 * Create on: Jul 31, 2017
 *    Author: LiFei
 *   Version: V1.0, Jul 31, 2017, LiFei
 */


/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <exception>
#include <vector>
#include "header/system/pub_types.h"
#ifdef __linux
#endif
//using namespace std;
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
CCTIME *pub_time;



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
/* CTIME class */
CCTIME::CCTIME()
{
#ifdef __linux
    startTime = QTime::currentTime();
    time.year = 2017;
    time.month = 9;
    time.day = 1;
    time.hour = startTime.hour();
    time.minute = startTime.minute();
    time.second = startTime.second();
    time.minisecond = startTime.msec();
    time_ms = 0;
#else
	time.year = 2017;
	time.month = 9;
	time.day = 1;
	time.hour = 0;
	time.minute = 0;
	time.second = 0;
	time.minisecond = 0;
	time_ms = 0;
#endif

}

CCTIME::~CCTIME()
{
	
}

inline TIME CCTIME::getTime(void)
{
#ifdef __linux
    QTime curr = QTime::currentTime();
    std::unique_lock<std::mutex> lck(m_mutex);
    time.hour = curr.hour();
    time.minute = curr.minute();
    time.second = curr.second();
    time.minisecond = curr.msec();
#endif
	return time;
}

size_t CCTIME::getDiffTimeMs(const size_t &_t)
{
//#ifdef __linux
//    QTime curr = QTime::currentTime();
//    std::unique_lock<std::mutex> lck(m_mutex);
//    time_ms = startTime.msecsTo(curr) % SIZE_MAX_VALUE;
//#endif
//	if(time_ms < _t)
//		return ((SIZE_MAX_VALUE - _t) + time_ms);
//	else
//		return (time_ms - _t);
    return QDateTime::currentMSecsSinceEpoch()-_t;
}





#ifndef __linux
TIME CTIME::getDiffTime(const TIME &t0, const TIME &t1)
{
	TIME diff_time;
	uint8_t month_days = 30;	
	diff_time.year = t0.year - t1.year;
	diff_time.month = t0.month - t1.month;
	diff_time.day = t0.day - t1.day;
	diff_time.hour = t0.hour - t1.hour;
	diff_time.minute = t0.minute - t1.minute;
	diff_time.second = t0.second - t1.second;
	diff_time.minisecond = t0.minisecond - t1.minisecond;
	
	if(t0.month == 2)
	{
		if(t0.year % 100 == 0)
		{
			if(t0.year % 400 == 0)
				month_days = 29;
			else
				month_days = 28;
		}
		else
		{
			if(t0.year % 4 == 0)
				month_days = 29;
			else
				month_days = 28;
		}
	}
	else
	{
		if((t0.month == 2) 
		&& (t0.month == 4) 
		&& (t0.month == 6) 
		&& (t0.month == 9) 
		&& (t0.month == 11))
		{
			month_days = 30;
		}
		else
		{
			month_days = 31;
		}
	}	
	
	if(diff_time.minisecond < 0)
	{
		if(diff_time.second <= 0)
		{
			if(diff_time.minute <= 0)
			{
				if(diff_time.hour <= 0)
				{
					if(diff_time.day <= 0)
					{
						if(diff_time.month <= 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 11;
								diff_time.day += 30;
								diff_time.hour += 23;
								diff_time.minute +=59;
								diff_time.second += 59;
								diff_time.minisecond += 1000;
								
							}
						}
						else
						{
							diff_time.month--;
							diff_time.day += (month_days - 1);
							diff_time.hour += 23;
							diff_time.minute += 59;
							diff_time.second += 59;
							diff_time.minisecond += 1000;
						}
					}
					else
					{
						diff_time.day--;
						diff_time.hour += 23;
						diff_time.minute += 59;
						diff_time.second += 59;
						diff_time.minisecond += 1000;
					}
				}
				else
				{
					diff_time.hour--;
					diff_time.minute += 59;
					diff_time.second += 59;
					diff_time.minisecond += 1000;
				}
			}
			else
			{
				diff_time.minute--;
				diff_time.second += 59;
				diff_time.minisecond += 1000;
			}
		}
		else
		{
			diff_time.second--;
			diff_time.minisecond += 1000;
		}
	}
	else
	{
		if(diff_time.second < 0)
		{
			if(diff_time.minute <= 0)
			{
				if(diff_time.hour <= 0)
				{
					if(diff_time.day <= 0)
					{
						if(diff_time.month <= 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 11;
								diff_time.day += 30;
								diff_time.hour += 23;
								diff_time.minute +=59;
								diff_time.second += 60;
								
							}
						}
						else
						{
							diff_time.month--;
							diff_time.day += (month_days - 1);
							diff_time.hour += 23;
							diff_time.minute += 59;
							diff_time.second += 60;
						}
					}
					else
					{
						diff_time.day--;
						diff_time.hour += 23;
						diff_time.minute += 59;
						diff_time.second += 60;
					}
				}
				else
				{
					diff_time.hour--;
					diff_time.minute += 59;
					diff_time.second += 60;
				}
			}
			else
			{
				diff_time.minute--;
				diff_time.second += 60;
			}
		
		}
		else
		{
			if(diff_time.minute < 0)
			{
				if(diff_time.hour <= 0)
				{
					if(diff_time.day <= 0)
					{
						if(diff_time.month <= 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 11;
								diff_time.day += 30;
								diff_time.hour += 23;
								diff_time.minute +=60;
								
							}
						}
						else
						{
							diff_time.month--;
							diff_time.day += (month_days - 1);
							diff_time.hour += 23;
							diff_time.minute += 60;
						}
					}
					else
					{
						diff_time.day--;
						diff_time.hour += 23;
						diff_time.minute += 60;
					}
				}
				else
				{
					diff_time.hour--;
					diff_time.minute += 60;
				}
			
			}
			else
			{
				if(diff_time.hour < 0)
				{
					if(diff_time.day <= 0)
					{
						if(diff_time.month <= 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 11;
								diff_time.day += 30;
								diff_time.hour += 24;
								
							}
						}
						else
						{
							diff_time.month--;
							diff_time.day += (month_days - 1);
							diff_time.hour += 24;
						}
					}
					else
					{
						diff_time.day--;
						diff_time.hour += 24;
					}
				}
				else
				{
					if(diff_time.day < 0)
					{
						if(diff_time.month <= 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 11;
								diff_time.day += 31;
								
							}
						}
						else
						{
							diff_time.month--;
							diff_time.day += month_days;
						}
					}
					else
					{
						if(diff_time.month < 0)
						{
							if(diff_time.year > 0)
							{
								diff_time.year--;
								diff_time.month += 12;							
							}
						}
					}
				}
			
			}
		}
	}
	
	return diff_time;
	
}
#endif

size_t CCTIME::getTimeMs(void)
{
    return QDateTime::currentMSecsSinceEpoch();
//#ifdef __linux
//    QTime curr = QTime::currentTime();
//    std::unique_lock<std::mutex> lck(m_mutex);
//    time_ms = startTime.msecsTo(curr) % SIZE_MAX_VALUE;
//#endif
//	return time_ms;
}

void CCTIME::delayuS(size_t us)
{
	size_t i = 0;
	size_t j = 0;
	
	for(i = 0; i < us; i++)
		for(j = 0; j < 6; j++)
		asm("nop");

}

/* CLIST class */
CLIST::CLIST(size_t iSize, size_t mSize)
{
    //Create a empty list
    list_size = 0;
    item_size = iSize;
    max_size = mSize;

    head.next = &head;
    head.prev = &head;
}

CLIST::~CLIST()
{
    // destroy the whole list
    while(list_size > 0)
        removeNode(head.next);
}


LIST_NODE_TYPE* CLIST::insertNode(LIST_NODE_TYPE *node, char* data)
{
	LIST_NODE_TYPE *new_node = NULL;
	try
	{
		new_node = new LIST_NODE_TYPE;
	}
	catch(std::bad_alloc a)
	{
		return NULL;
	}
	
	if(NULL == node)
	{
		node = &head;
	}	
    
	try
	{
		new_node->data = new char[item_size];
	}
	catch(std::bad_alloc a)
	{
		delete new_node;
		new_node = NULL;
		return NULL;
	}
    
    memcpy(new_node->data, data, item_size);
    // insert new node before node, if node == null, insert on the tail
    new_node->next = node;
    new_node->prev = node->prev;
    new_node->next->prev = new_node;
    new_node->prev->next = new_node;
    
    list_size += 1;
    
    if(0 != max_size)
	    while(list_size > max_size)
	    	removeNode(head.next);
    
    return new_node;
}

void CLIST::removeNode(LIST_NODE_TYPE *node)
{
	if(NULL == node)
		return;
	else
	{
	    // delete node
	    node->next->prev = node->prev;
	    node->prev->next = node->next;
	    if(NULL != node->data)
	    {
	    	delete[] node->data;
	    	node->data = NULL;
	    }	    	
	    delete node;
	    node = NULL;
	    list_size -= 1;
	    return;
	}
}

inline LIST_NODE_TYPE* CLIST::headNode()
{
	 return list_size > 0 ? head.next : NULL;
}

inline LIST_NODE_TYPE* CLIST::tailNode()
{
	return list_size > 0 ? head.prev : NULL; 
}

inline LIST_NODE_TYPE* CLIST::nextNode(LIST_NODE_TYPE *node)
{
	return node->next;
}

inline LIST_NODE_TYPE* CLIST::prevNode(LIST_NODE_TYPE *node)
{
	return node->prev;
}

inline bool CLIST::isListEmpty()
{
	if(list_size == 0)
		return true;
	else
		return false;
	
}


/* CPOLYN class */

void CPOLYN::setPolyn(const vector<double> &_c, float endPoint0, float endPoint1)
{
    coe = _c;
	ep0 = endPoint0;
	ep1 = endPoint1;
}

float CPOLYN::getValue(float x)
{
	//TODO: check data type, LiFei, 2017/09/17
    float result = 0;
	size_t i = 0;
    for(i = 0; i < coe.size(); i++)
        result += coe[i] * pow(x, (float)i);
    return result;
}

float CPOLYN::getEndPoint0()
{
    return ep0;
}

float CPOLYN::getEndPoint1()
{
    return ep1;
}

float CPOLYN::getDerivValue(float x, size_t k)
{
	size_t i = 0;
	size_t j = 0;
	size_t ck = 1;
	float result = 0;
	//TODO: check data type, LiFei, 2017/09/17
    if(k >= coe.size())
		return 0;
    for(i = 0; i < (coe.size() - k); i++)
	{
		ck = 1;
		for(j = i; j < k + i; j++)
			ck *= (j + 1);
		
        result += ck * coe[i+k] * pow(x, (float)i);
	}
		
	return result;	
}

float CPOLYN::getCurvature(float x)
{
	float _1st_deriv = 0;
	float _2nd_deriv = 0;
	_1st_deriv = getDerivValue(x, 1);
	_2nd_deriv = getDerivValue(x, 2);
	return _2nd_deriv / pow((1 + pow(_1st_deriv ,(float)2.0)) ,(float)1.5);
}


/* CPID class */
void CPID_INC::init()
{
    m_last_err = 0;
    m_prev_err = 0;
    m_integ = 0;
}

float CPID_INC::proc(float aim_value, float real_value, float ts)
{
    float output = 0;
    float err = 0;
    err = aim_value - real_value;
    output = m_kp * (err - m_last_err) + m_ki * ts * err + m_kd * (err - 2 * m_last_err + m_prev_err) / ts;
    m_prev_err = m_last_err;
    m_last_err = err;
    return output;
}


void CPID::init()
{
    m_last_err = 0;
    m_prev_err = 0;
    m_integ = 0;
}

float CPID::proc(float aim_value, float real_value, float ts)
{
    float output = 0;
    float err = 0;
    err = aim_value - real_value;
    if(err > m_sep_err)
        m_integ = 0;
    else
        m_integ += m_ki * ts * err;

    m_integ = MAX(MIN(m_integMax, m_integ), m_integMin);
    output = m_kp * err + m_integ + m_kd * (err - m_last_err) / ts;
    m_prev_err = m_last_err;
    m_last_err = err;
    return output;
}


/////////////////////////////////////////////////////////////////////

/* TASK Class */


TASK::TASK(CCTIME *_time, uint8_t _period)
{
	time = _time;
	startTime = 0;
	endTime = 0;
	period = _period;
	taskId = NULL;
	active = false;
	run_flag = false;
}

TASK::~TASK()
{
	
}

void TASK::setTimeSrc(CCTIME *_time)
{
    std::unique_lock<std::mutex> lck(m_mutex);
	time = _time;
}

void TASK::setPeriod(uint8_t _p)
{
    std::unique_lock<std::mutex> lck(m_mutex);
	period = _p;
}


void TASK::start()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	active = true;
	run_flag = true;
}
void TASK::abort()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	active = false;
	run_flag = false;
}

bool TASK::started()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	return active;
}

bool TASK::running()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	return run_flag;
}

void TASK::setStartTime(const size_t &t)
{
    std::unique_lock<std::mutex> lck(m_mutex);
	startTime = t;
}
void TASK::setEndTime(const size_t &t)
{
    std::unique_lock<std::mutex> lck(m_mutex);
	endTime = t;
}
size_t TASK::getStartTime()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	return startTime;
}
size_t TASK::getEndTime()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	return endTime;
}

uint8_t TASK::periodTime()
{
    std::unique_lock<std::mutex> lck(m_mutex);
	return period;
}

//void TASK::task_func()
//{
	
//}



/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */
bool initPublic()
{
	pub_time = new(std::nothrow) CCTIME();
    if(NULL == pub_time)
    {
        return false;
    }
#ifdef __linux



#endif
    return true;
}



void geographicLib_coordinate_transfer::forward(double lat, double lon, double h, double &x, double &y, double &z)
{
    Geocentric earth(Constants::WGS84_a(),Constants::WGS84_f());
    LocalCartesian proj(lat0, lon0, alt0, earth);
    proj.Forward(lat,lon,h,x,y,z);
}

void geographicLib_coordinate_transfer::reverse(double x, double y, double z, double &lat, double &lon, double &h)
{
    Geocentric earth(Constants::WGS84_a(),Constants::WGS84_f());
    LocalCartesian proj(lat0, lon0, alt0, earth);
    proj.Reverse(x,y,z,lat,lon,h);
}

void geographicLib_coordinate_transfer::setGroundOrigin(point_3d_t _groundOrigin)
{
    lon0=_groundOrigin.x;
    lat0=_groundOrigin.y;
    alt0=_groundOrigin.z;
}

point_3d_t geographicLib_coordinate_transfer::Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData)
{
    point_3d_t output_data;
    //
    double x=srcPoint.x;
    double y=srcPoint.y;
    double z=srcPoint.z;

    //绕y轴旋转
    double x2,y2,z2;
    double pitch_com=(insData.pitch)*M_PI/180.0;
    z2=z*cos(pitch_com)-x*sin(pitch_com);
    x2=z*sin(pitch_com)+x*cos(pitch_com);
    y2=y;

    //绕x轴旋转
    double x1,y1,z1;
    double roll_com=(insData.roll)*M_PI/180.0;
    y1 = y2*cos(roll_com)-z2*sin(roll_com);
    z1 = y2*sin(roll_com)+z2*cos(roll_com);
    x1 = x2;

    //绕z轴旋转
    double x3,y3,z3;
    double yaw_com=(-(insData.heading)+90)*M_PI/180.0;
    x3=x1*cos(yaw_com)-y1*sin(yaw_com);
    y3=x1*sin(yaw_com)+y1*cos(yaw_com);
    z3=z1;



    output_data.x =x3 +insData.pos.x ;//x no change
    output_data.y =y3 +insData.pos.y;
    output_data.z =z3 +insData.pos.z;



    return output_data;
}




