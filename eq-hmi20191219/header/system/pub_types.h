#ifndef PUB_TYPES_H
#define PUB_TYPES_H

#include<QDebug>
#include<QDateTime>
#include<stdlib.h>
#include <new>
#include<mutex>
#include <pthread.h>
#include<iostream>
#include<thread>
#include<math.h>
#include <boost/endian/arithmetic.hpp>
#include"rtwtypes.h"
#include"typedefs.h"
#include"header/system/ser_outside_xc/thirdParty/geographicLib/Geocentric.hpp"
#include"header/system/ser_outside_xc/thirdParty/geographicLib/LocalCartesian.hpp"

using namespace std;
using namespace boost::endian;
using namespace GeographicLib;

/* Interface related */
#define Interface class

#define DeclareInterface(name) Interface name { \
          public: \
          virtual ~name() {}

#define DeclareBasedInterface(name, base) Interface name :\
        public base { \
           public: \
           virtual ~name() {}

#define EndInterface };

#define implements public

#define MULTI_THREAD_MODE
#ifdef __linux
#ifdef MULTI_THREAD_MODE
#define TaskFuncDeclare(ClassType, Name, maxPeriod)                 \
    public:\
    virtual void task_start()\
    {\
        m_ticked = true;\
    }\
    private:\
    virtual void task_func();\
    static void task_thread(void *_p)\
    {\
        ClassType *_this = (ClassType*)_p;\
        _this->task_ ## Name();\
    }\
    void task_ ## Name()                         \
    { \
        while(m_isAlive)\
        {\
            if(m_ticked)\
            {\
                size_t period = pub_time->getDiffTimeMs(getStartTime());\
                setRealPeriod(period);\
                size_t start = pub_time->getTimeMs();\
                setEndTime(start);\
                setStartTime(start);\
                bool _over_time = (period > maxPeriod);\
                if(m_over_time_record[0] >= 0x80) m_task_over_time_cnt--;\
                for(uint8_t i = 0; i < 24; i++)\
                {\
                    m_over_time_record[i] <<= 1;\
                    if(m_over_time_record[i+1] >= 0x80)\
                        m_over_time_record[i] |= 0x01;\
                }\
                m_over_time_record[24] <<= 1;\
                if(_over_time){\
                    m_over_time_record[24] |= 0x01;\
                    m_task_over_time_cnt++;}\
                m_over_time_duty = (float)m_task_over_time_cnt / 2.0;\
                task_func();\
                size_t end = pub_time->getTimeMs();\
                /*std::string str_print = ""#Name" running! ";*/\
                /*str_print += "exec time:";*/\
                /*str_print += ((end - start > maxPeriod)? (", WARN: "#Name" exec over time !!") : "");*/\
                /*str_print += (_over_time? (", ERROR: "#Name" period over time !!") : "");*/\
                /*str_print +=  ", Over time duty:";*/\
                /*str_print +=  to_string(m_over_time_duty);*/\
                /*str_print += "%";*/\
                /*str_print += ", Error Code:";*/\
                /*str_print += to_string(m_error_code);*/\
                /*pub_printer->print(str_print, true);*/\
                m_ticked = false;\
            }else\
            {\
                this_thread::sleep_for(std::chrono::milliseconds(1));\
            }\
        }\
    }\
    void err_check();\
    private:\
    PropertyDeclare(bool, isAlive, setAlive)\
    PropertyDeclare(bool, ticked, setTicked)\
    PropertyDeclare(size_t, real_period, setRealPeriod)\
    std::thread m_thread;\
    uint8_t m_task_over_time_cnt;\
    uint8_t m_over_time_record[25];\
    PropertyDeclare(float, over_time_duty, setOverDuty)\
    PropertyDeclare(bool, task_alive, setTaskAlive)\
    err_t m_error_code;
#else
#define TaskFuncDeclare(ClassType, Name)                            \
    public:                                                         \
    virtual void task_func();\
    private:\
    static void* task_ ## Name(void *a)                         \
    {                                                               \
        ClassType *_this = (ClassType *)a;                          \
        size_t period = pub_time->getDiffTimeMs(_this->startTime());\
        size_t start = pub_time->getTimeMs();                    \
        _this->setStartTime(start);\
        _this->task_func();                                     \
        size_t end = pub_time->getTimeMs();\
        cout << "Task "#Name" is running! " << "ID:"\
        << "not thread!" << ", Start time is:" << start << ", ";\
        cout << "End time is:" << end << ", "#Name" exec time is:" \
        << end - start << ", Period is:" << period;\
        if(end - start > 50)\
        {\
            cout << ", WARN: "#Name" exec over time !!";\
        }\
        if(period > maxPeriod)\
        {\
            cout << " ERROR: "#Name" period over time !!";\
        }\
        cout << endl;\
        return NULL;\
    }\
    virtual void task_start()\
    {\
        task_ ## Name(this);\
    }\
    PropertyDeclare(size_t, startTime, setStartTime)
#endif
#else
#define TaskFuncDeclare(ClassType, Name, maxPeriod)                            \
    public:                                                         \
    virtual void task_func();\
    private:\
    static void* task_ ## Name(void *a)                         \
    {                                                               \
        ClassType *_this = (ClassType *)a;                          \
        size_t start = pub_time->getTimeMs();                    \
        _this->task_func();                                     \
        size_t end = pub_time->getTimeMs();\
        if(end - start > 50)\
        {\
        }\
    }\
    virtual void task_start()\
    {\
        task_ ## Name(this);\
    }
#endif


#define PropertyDeclare(Type, Name, setName, ...)                   \
    private:                                                        \
    Type m_ ## Name __VA_ARGS__;                                    \
    public:                                                         \
    inline const Type &Name(void) const\
    {\
        return m_ ## Name;\
    }      \
    inline void setName(const Type &Name)\
    { \
        m_ ## Name = Name;\
    }    \
    private:

#define PI       3.1415926
#define _g_      9.8

#define MAX(x,y)   ((x) > (y) ? (x):(y))
#define MIN(x,y)   ((x) < (y) ? (x):(y))



typedef struct{
    string dbfile;
    vector<string> ids;
    vector<uint8_t> types;
    vector<uint8_t> gears;
    vector<uint8_t> formats;
}task_db_t;


/**************************Time**************************************/
struct TIME
{
    int16_t year;
    int8_t month;
    int8_t day;
    int8_t hour;
    int8_t minute;
    int8_t second;
    int16_t minisecond;
};


class CCTIME
{
public:
    CCTIME();
    ~CCTIME();
    size_t getTimeMs(void);
    TIME getTime(void);
    size_t getDiffTimeMs(const size_t &_t);

#ifndef __linux
    TIME getDiffTime(const TIME &t0, const TIME &t1);
#endif
    void delayuS(size_t us);
private:
#ifdef __linux
    QTime startTime;
#endif
    TIME time;
    size_t time_ms;

    std::mutex m_mutex;
};

/***************************List***********************************/
struct LIST_NODE_TYPE
{
    char* data;    //datas
    LIST_NODE_TYPE *prev;
    LIST_NODE_TYPE *next;
};       /* node struct */


class CLIST
{
public:
    CLIST(size_t iSize, size_t mSize);
    ~CLIST(void);
    LIST_NODE_TYPE *insertNode(LIST_NODE_TYPE *node, char* data);
    void removeNode(LIST_NODE_TYPE *node);
    LIST_NODE_TYPE *headNode(void);
    LIST_NODE_TYPE *tailNode(void);
    LIST_NODE_TYPE *prevNode(LIST_NODE_TYPE *node);
    LIST_NODE_TYPE *nextNode(LIST_NODE_TYPE *node);
    bool isListEmpty(void);
private:
    size_t item_size;
    size_t list_size;
    size_t max_size;
    LIST_NODE_TYPE head;
};

template<typename T>
struct Node
{
    Node(const T& data)
        : _data(data)
        , _pNext(NULL)
        , _pPre(NULL)
    {}

    T _data;            //�����е�����
    Node<T>* _pNext;    //��һ���ڵ�
    Node<T>* _pPre;    //ǰһ���ڵ�
};
template<typename T>
class List
{
public:
    List()
    :_pHead(NULL)
    ,_pTail(NULL)
    ,_size(0)
    ,_max_size(0)
    {}

    List(size_t _mSize)
    :_pHead(NULL)
    ,_pTail(NULL)
    ,_size(0)
    ,_max_size(_mSize)
    {}

    List(const T arr[], size_t size)
    :_pHead(NULL)
    ,_pTail(NULL)
    ,_size(0)
    ,_max_size(0)
    {
        for(size_t i = 0; i < size; i++)
        {
            push_back(arr[i]);
        }
    }

    List(const List<T>& l)
    :_pHead(NULL)
    ,_pTail(NULL)
    ,_size(0)
    ,_max_size(0)
    {
        Node<T>* pRet = l._pHead;
        while(NULL != pRet)
        {
            push_back(pRet->_data);
            pRet = pRet->_pNext;
        }
    }
    List<T>& operator=(const List<T>& l)
    {
        if(empty())
        {
            Node<T>* pCur = l._pHead;
            for(size_t i = 0; i<l._size; i++)
            {
                push_back(pCur->_data);
                if(_max_size > 0)
                {
                    while(_size >= _max_size)
                        pop_front();
                }
                pCur = pCur->_pNext;
            }
        }
        //��ǰ�������������ȴ���Ҫ����������������
        else if(_size>l._size)
        {
            Node<T>* pCur = _pHead;
            Node<T>* pRet = l._pHead;
            //��l._size���ȵ�ֵ��������
            for(size_t i = 0; i<l._size; i++)
            {
                pCur->_data = pRet->_data;
                pCur = pCur->_pNext;
                pRet = pRet->_pNext;
            }

            //��l._size��_size֮����ʣ���ĳ���ɾ��
            for(size_t i = l._size; i<_size; i++)
            {
                pop_back();
            }
            if(_max_size > 0)
            {
                while(_size >= _max_size)
                    pop_front();
            }
        }
        else
        {
            //��ǰ�����ĳ���С�ڻ����ڱ�����������������
            Node<T>* pCur = _pHead;
            Node<T>* pRet = l._pHead;
            //������Ϊ_size �ĸ�ֵ����������
            for(size_t i = 0; i<_size; i++)
            {
                pCur->_data = pRet->_data;
                pCur = pCur->_pNext;
                pRet = pRet->_pNext;
            }
            if(_max_size > 0)
            {
                while(_size >= _max_size)
                    pop_front();
            }
        }
        return *this;
    }

    virtual ~List()
    {
        clear();
    }
public:
    bool empty()const { return (_size == 0); }
    Node<T>* push_back(const T& data)
    {
        Node<T>* pCur = NULL;
        if(empty())
        {
            pCur = BuyNewNode(data);
            _pHead = _pTail = pCur;
        }
        else
        {
            pCur = BuyNewNode(data);
            pCur->_pPre = _pTail;
            _pTail->_pNext = pCur;
            _pTail = pCur;
        }
        ++_size;
        if(_max_size > 0)
        {
            while(_size >= _max_size)
                pop_front();
        }
        return pCur;
    }
    void push_back(Node<T>* node)
    {
        if(node == NULL)
            return;
        if(empty())
        {
            _pHead = _pTail = node;
        }
        else
        {
            node->_pPre = _pTail;
            _pTail->_pNext = node;
            _pTail = node;
        }
        ++_size;
        if(_max_size > 0)
        {
            while(_size >= _max_size)
                pop_front();
        }
    }
    void pop_back()
    {
        if(empty())
        {
            return;
        }
        else if(_pHead->_pNext == NULL)
        {
            Node<T>* pDel(_pHead);
            _pHead = _pTail = NULL;
            delete pDel;
        }
        else
        {
            Node<T>* pDel(_pTail);
            _pTail = _pTail->_pPre;
            _pTail->_pNext = NULL;
            delete pDel;
        }
        --_size;
    }
    Node<T>* push_front(const T& data)
    {
        Node<T>* pRet = NULL;
        if(empty())
        {
            pRet = BuyNewNode(data);
            push_back(pRet->_data);
        }
        else
        {
            pRet = BuyNewNode(data);
            _pHead->_pPre = pRet;
            pRet->_pNext = _pHead;
            _pHead = pRet;

        }
        ++_size;
        if(_max_size > 0)
        {
            while(_size >= _max_size)
                pop_back();
        }
        return pRet;
    }
    void pop_front()
    {
        if(empty())
        {
            return;
        }
        else if(_pHead->_pNext == NULL)
        {
            Node<T>* pDel = _pHead;
            delete pDel;
            _pHead = _pTail = NULL;
        }
        else if(_pHead->_pNext == NULL)
        {
            delete _pHead;
            _pHead = NULL;
        }
        else
        {
            Node<T>* pDel = _pHead;
            _pHead = _pHead->_pNext;
            delete pDel;
            _pHead->_pPre = NULL;
        }
        --_size;
    }
    Node<T>* find(const T& data)
        {
            if(empty())
            {
                return NULL;
            }
            else
            {
                Node<T>* pCur = _pHead;
                while(pCur)
                {
                    if(pCur->_data == data)
                    {
                        return pCur;
                    }
                    pCur = pCur->_pNext;
                }
            }
            return NULL;
        }
    Node<T>* insert(Node<T>* pos, const T& data)
    {
        Node<T>* pCur = NULL;
        if(pos == NULL)
        {
            //assert(false);
            return NULL;
        }
        else if(pos->_pNext == NULL)
        {
            pCur = push_back(data);
        }
        else
        {
            pCur = BuyNewNode(data);
            pCur->_pPre = pos;
            pCur->_pNext = pos->_pNext;
            pos->_pNext->_pPre = pCur;


            pos->_pNext = pCur;
        }
        ++_size;
        if(_max_size > 0)
        {
            while(_size >= _max_size)
                pop_front();
        }
        return pCur;
    }
    void erase(Node<T>* pos)
    {
        if((pos == NULL) || ((pos->_pNext == NULL) && (pos->_pPre == NULL)))
            return;
        if(empty())
        {
            return;
        }
        else if(pos->_pNext == NULL)
        {
            pop_back();
        }
        else
        {
            pos->_pPre->_pNext = pos->_pNext;
            pos->_pNext->_pPre = pos->_pPre->_pNext;
            delete pos;
        }
        --_size;
    }
    Node<T>* front(){ return _pHead; }
    const Node<T>* front()const{ return _pHead; }
    T front_data(){ return _pHead->_data; }
    Node<T>* back(){ return _pTail; }
    const Node<T>* back()const{ return _pTail; }
    T back_data(){ return _pTail->_data; }
    T data(Node<T>* pos){ return pos->_data; }
    void clear()
    {
        //cout<<"clear()"<<endl;
        if(empty())
        {
            return;
        }
        else
        {
            while(_pHead != NULL)
            {
                pop_back();
            }
        }
    }
    size_t size()const{ return _size; }
    size_t maxSize()const{ return _max_size; }
private:
    Node<T>* BuyNewNode(const T& data)
    {
        //TODO: if new error,so what? LiFei, 2017/10/17
        Node<T>* pNew = new Node<T>(data);
        return pNew;
    }

private:
    Node<T>* _pHead;
    Node<T>* _pTail;
    size_t _size;
    size_t _max_size;
};

/* Task Class */
class TASK
        #ifdef __linux
        //: public thread
        #endif
{
public:
    TASK()
    :time(NULL), period(100)
    {}
    TASK(CCTIME *_time, uint8_t _period);
    virtual ~TASK();
    void setTimeSrc(CCTIME *_time);
    void setPeriod(uint8_t _p);
    void start();
    void abort();
    bool started();
    bool running();
    void setStartTime(const size_t &t);
    void setEndTime(const size_t &t);
    size_t getStartTime();
    size_t getEndTime();
    uint8_t periodTime();
public:
    virtual void task_start() = 0;
    virtual void task_func() = 0;
public:
    CCTIME *time;
    Node<TASK*> *taskId;
private:
    size_t startTime;
    size_t endTime;
    uint8_t period;
    bool active;
    bool run_flag;
    std::mutex m_mutex;
};

/***************************Polynomial**************************************/
class CPOLYN
{
private:
    vector<double> coe;
    float ep0;
    float ep1;
public:
    CPOLYN()
        :ep0(0)
        ,ep1(0)
        ,coe(vector<double>(4))
        {}
    CPOLYN(const vector<double> &_c, float endPoint0, float endPoint1)
        :ep0(endPoint0)
        ,ep1(endPoint1)
        ,coe(_c)
        {}
    virtual ~CPOLYN(){}
    void setPolyn(const vector<double> &_c, float endPoint0, float endPoint1);
    float getValue(float x);
    float getEndPoint0();
    float getEndPoint1();
    float getDerivValue(float x, size_t k);
    float getCurvature(float x);
};



/***************************PID**************************************/
class CPID_INC
{
public:
    CPID_INC()
        :m_kp(0)
        ,m_ki(0)
        ,m_kd(0)
        ,m_sep_err(0)
        ,m_integMax(0)
        ,m_integMin(0)
        ,m_integ(0)
        ,m_last_err(0)
        ,m_prev_err(0)
    {}
    virtual ~CPID_INC()
    {}
    void init();
    float proc(float aim_value, float real_value, float ts);

    PropertyDeclare(float, kp, setKp)
    PropertyDeclare(float, ki, setKi)
    PropertyDeclare(float, kd, setKd)
    PropertyDeclare(float, sep_err, setSepErr)
    PropertyDeclare(float, integMax, setIntegMax)
    PropertyDeclare(float, integMin, setIntegMin)
    PropertyDeclare(float, integ, setInteg)
    PropertyDeclare(float, last_err, setLastErr)
    PropertyDeclare(float, prev_err, setPrevErr)
} ;

class CPID
{
public:
    CPID()
        :m_kp(0)
        ,m_ki(0)
        ,m_kd(0)
        ,m_sep_err(0)
        ,m_integMax(0)
        ,m_integMin(0)
        ,m_integ(0)
        ,m_last_err(0)
        ,m_prev_err(0)
    {}
    virtual ~CPID()
    {}
    void init();
    float proc(float aim_value, float real_value, float ts);

    PropertyDeclare(float, kp, setKp)
    PropertyDeclare(float, ki, setKi)
    PropertyDeclare(float, kd, setKd)
    PropertyDeclare(float, sep_err, setSepErr)
    PropertyDeclare(float, integMax, setIntegMax)
    PropertyDeclare(float, integMin, setIntegMin)
    PropertyDeclare(float, integ, setInteg)
    PropertyDeclare(float, last_err, setLastErr)
    PropertyDeclare(float, prev_err, setPrevErr)
} ;


extern CCTIME *pub_time;



extern bool initPublic();


enum RUN_CORE
{
    RUN_CORE_P0 = 0,
    RUN_CORE_P1,
    RUN_CORE_P2
};

enum ERROR_ENUM
{
    ERR_BIT_COM_CAN = 0,
    ERR_BIT_COM_RS232,
    ERR_BIT_MALLOC,
    ERR_BIT_PARAM,
    ERR_BIT_TASK_PERIOD,
    ERR_BIT_DATA_UPDATE,
    ERR_BIT_INTERFACE
};

struct point_2d_t
{
    point_2d_t()
    :x(0), y(0)
    {}
    point_2d_t(const double &_x, const double &_y)
    :x(_x), y(_y)
    {}
    double x;
    double y;

};

struct point_3d_t
{
    point_3d_t()
    :x(0), y(0), z(0)
    {}
    point_3d_t(const double &_x, const double &_y, const double &_z)
    :x(_x), y(_y), z(_z)
    {}
    double x;
    double y;
    double z;
};

enum TERMINAL_TYPE
{
    Excavator=0,
    Forklift
};

struct TERMINAL_CONFIG
{
    TERMINAL_CONFIG()
        :terminal_type(Excavator)
        ,terminal_code("")
        ,terminal_num("")
        ,sim_card_num("")
        ,binding_device_num("")
        ,reserve(0)
    {
        terminal_code.resize(50);
        terminal_num.resize(50);
        sim_card_num.resize(50);
        binding_device_num.resize(50);
    }

    TERMINAL_TYPE terminal_type;
    string terminal_code;
    string terminal_num;
    string sim_card_num;
    string binding_device_num;
    uint8_t reserve;
};

struct EXCAVATOR_STATUS
{
     EXCAVATOR_STATUS()
         :work_status("")
         ,work_platform("")
         ,material_type("")
         ,output(0)
     {
         group_truck.clear();
     }

     string work_status;
     vector<string> group_truck;
     string work_platform;
     string load_area_id;
     string material_type;
     uint16_t output;
};

struct FORKLIFT_STATUS
{
     FORKLIFT_STATUS()
         :work_status("")
         ,dispatch_order("")
         ,mission("")
     {}

     string work_status;
     string dispatch_order;
     string mission;
};



struct DELAY_REQUEST
{
    DELAY_REQUEST()
        :isDelayRequestOk(true)
        ,delayType(0)
        ,delayMinutes(0)
    {}

    bool isDelayRequestOk;
    int delayType;
    int delayMinutes;
};

struct TERMINAL_REQUEST
{
    TERMINAL_REQUEST()
        :isTcpTerminalRequestOk(true)
        ,isHttpTerminalRequestOk(true)
        ,request_type(0)
        ,isHard(true)
    {}

    bool isTcpTerminalRequestOk;
    bool isHttpTerminalRequestOk;
    uint8_t request_type;
    bool isHard;
};

struct LOAD_POS_UPDATE
{
    LOAD_POS_UPDATE()
        :isNewLoadPos(false)
        ,updateType(0)
    {}

    bool isNewLoadPos;
    uint8_t updateType;
};

struct CMD_INFO
{
    CMD_INFO()
        :isNewCmd(false)
        ,cmd_type(0)
    {}


    bool isNewCmd;
    uint8_t cmd_type;
    point_3d_t cmd_destination;
};

struct SURRD_TRUCK_INFO_REQ
{
    SURRD_TRUCK_INFO_REQ()
        :isNew(false)
        ,vehicleNum("NULL")
    {}

    bool isNew;
    string vehicleNum;

};

struct SURRD_TRUCK_CTRL_REQ
{
    SURRD_TRUCK_CTRL_REQ()
        :isNew(false)
        ,vehicleNum("NULL")
        ,ctrlType(0)
    {}

    bool isNew;
    string vehicleNum;
    uint8_t ctrlType;
};


struct TERMINAL_INFO
{
    TERMINAL_INFO()
        :device_type(Excavator)
        ,driver_log_status(0)
        ,device_num("")
        ,driver_name("")
        ,driver_id("")
        ,device_status(0)
        ,heading_offset(0)
        ,parasGet(false)
    {}

    TERMINAL_TYPE device_type;
    TERMINAL_CONFIG config;
    int driver_log_status;
    string device_num;
    string driver_name;
    string driver_id;
    uint8_t device_status;
    point_3d_t base_point_offset;
    point_3d_t ref_point_offset;
    point_3d_t ground_origin;
    double heading_offset;
    bool parasGet;
    EXCAVATOR_STATUS es;
    FORKLIFT_STATUS fs;

};

struct DIRC_POINT
{
    DIRC_POINT()
        :heading(0)
    {}


    point_2d_t pos_xy;
    point_2d_t pos_lonlat;
    double heading;
};

struct LOAD_POS
{
    LOAD_POS()
        :isValid(false)
        ,load_pos_id("NULL")
        ,load_mode(0)
        ,calibrate_mode(0)
    {}

    bool isValid;
    DIRC_POINT basePoint;
    point_3d_t hmi_ref_point;
    string load_pos_id;
    uint8_t load_mode;
    uint8_t calibrate_mode;
    point_3d_t coordinate;
    vector<point_2d_t> endpoint_xy;
    vector<point_2d_t> endpoint_lonlat;
};

struct utc_time
{
    utc_time()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}

    utc_time(uint16_t _year,uint8_t _month,uint8_t _day,uint8_t _hour,
             uint8_t _minute,uint8_t _second,uint16_t _millisecond)
        :year(_year)
        ,month(_month)
        ,day(_day)
        ,hour(_hour)
        ,minute(_minute)
        ,second(_second)
        ,millisecond(_millisecond)
    {}

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millisecond;

};

struct point_lidarmap_t
{
    point_3d_t p;
    int index;
    point_lidarmap_t()
    {
        index=0;
    }
};

struct nav_t
{
    size_t time_stamp;

    utc_time utcTime;

    point_3d_t pos;

    float heading;		//??????
    float pitch;			//??????
    float roll;			//??????/??????

    float headingSTD;		//??????
    float pitchSTD;			//??????
    float rollSTD;			//??????/??????

    float gyroX;			//????????X
    float gyroY;			//????????Y
    float gyroZ;			//????????Z

    float accX;			//?????X
    float accY;			//?????Y
    float accZ;			//?????Z

    float magX;			//???X
    float magY;			//???Y
    float magZ;			//???Z

    uint16_t usw;			//uint??

    float vInp;			//???????
    float temper;			//?�?

    double latitude;			//???
    double longitude;			//????
    double altitude;			//????

    double latitudeSTD;			//???
    double longitudeSTD;			//????
    double altitudeSTD;			//????

    float eastSpeed;			//???????
    float northSpeed;		//???????
    float verticalSpeed;		//??????

    float eastSpeedSTD;			//???????
    float northSpeedSTD;		//???????
    float verticalSpeedSTD;		//??????

    double latitudeGNSS;		//
    double longitudeGNSS;		//
    double altitudeGNSS;		//

    float horizontalSpeed;	// lon speed
    float trackOverGround;  //
    float verticalSpeed2;	//

    uint32_t msGps;			//????

    uint8_t GNSSInfo1;		//
    uint8_t GNSSInfo2;		//

    uint8_t solnSVs;			//??????
    float vLatency;		//???????????????
    uint8_t anglesPositionType;//

    string RTK_status;
    string align_status;


    float headingGNSS;	//
    int16_t latencyMsHead;		//
    int16_t latencyMsPos;	//
    int16_t latencyMsVel;	//
    float pBar;			//
    float hBar;				//
    uint8_t newGPS;			//
    uint8_t imu_work_status;

    nav_t()
    {
        time_stamp=0;
        pos.x=0;
        pos.y=0;
        pos.z=0;
        heading=0;		//??????
        pitch=0;			//??????
        roll=0;			//??????/??????
        headingSTD=0;		//??????
        pitchSTD=0;			//??????
        rollSTD=0;			//??????/??????
        gyroX=0;			//????????X
        gyroY=0;			//????????Y
        gyroZ=0;			//????????Z
        accX=0;			//?????X
        accY=0;			//?????Y
        accZ=0;			//?????Z

        magX=0;			//???X
        magY=0;			//???Y
        magZ=0;			//???Z

        usw=0;			//uint??

        vInp=0;			//???????
        temper=0;			//?�?

        latitude=0;			//???
        longitude=0;			//????
        altitude=0;			//????

        latitudeSTD=0;			//???
        longitudeSTD=0;			//????
        altitudeSTD=0;			//????

        eastSpeed=0;			//???????
        northSpeed=0;		//???????
        verticalSpeed=0;		//??????

        eastSpeedSTD=0;			//???????
        northSpeedSTD=0;		//???????
        verticalSpeedSTD=0;		//??????

        latitudeGNSS=0;		//
        longitudeGNSS=0;		//
        altitudeGNSS=0;		//

        horizontalSpeed=0;	// lon speed
        trackOverGround=0;  //
        verticalSpeed2=0;	//

        msGps=0;			//????

        GNSSInfo1=0;		//
        GNSSInfo2=0;		//

        solnSVs=0;			//??????
        vLatency=0;		//???????????????
        anglesPositionType=0;//

        headingGNSS=0;	//
        latencyMsHead=0;		//
        latencyMsPos=0;	//
        latencyMsVel=0;	//
        pBar=0;			//
        hBar=0;				//
        newGPS=0;			//
        RTK_status="NONE";
        align_status="NONE";
        imu_work_status=0;
    }
};


struct sample_gis_packet_t{
    uint8_t time[8];
    uint8_t type = 0;
    big_uint16_t duration = 0;
    uint8_t area = 0;
    uint8_t unload_position[32];
    bool is_new = false;
};


struct GIS_VECTOR_POINT{
    double x = 0;
    double y = 0;
    double z = 0;
};

struct GIS_VECTOR_PACKET{
    string uuid;
    uint8_t updata_area;
    size_t point_number;
    vector<point_3d_t> points;
};


struct GIS_UPDATE_PACKET
{
    //byte timestamp[8];
    utc_time timestamp;
    byte state;//1 working  2 end
    string uuid;
    byte updata_area;//1 load area,2 unload area,3road
    byte sensor_pos;//1 left ,2 right, 3 center
    byte sensor_type;//1 16lidar ,2 24lidar,3 32lidar,4 rtk
    uint8_t unload_position[32];
    vector<uint8_t> lidar_data;
};

struct FORKLIFT_SAMPLE_GIS_PACKET
{
    FORKLIFT_SAMPLE_GIS_PACKET()
        :isNewMessage(false)

    {}

    bool isNewMessage;
//    string vin;
    utc_time start_time;
    utc_time end_time;
};

typedef struct{
    uint8_t id[10];
    double x = 0;
    double y = 0;
    double z = 0;
    uint8_t speed = 0;
    double heading = 0;
    uint8_t type = 0;
    uint8_t length = 0;
    uint8_t width = 0;
    uint8_t height = 0;
}one_vehicle;


typedef struct{
    bool is_new = true;
    uint8_t time[8];
    uint8_t number = 0;
    vector<one_vehicle> vehicles;
}surrd_vehicle_packet_t;

class geographicLib_coordinate_transfer
{
public:
    geographicLib_coordinate_transfer()
        :lat0(39.8825)
        ,lon0(110.257)
        ,alt0(1376.49)
        ,lonF(6.275)
        ,lonR(2.575)
        ,lat(3.45)
        ,alt(0.5)
        ,ref_lat_offset(-1)
    {}

    geographicLib_coordinate_transfer(double _lat0,double _lon0,double _alt0,
                                      double _lonF,double _lonR,double _lat,double _alt)
        :lat0(_lat0)
        ,lon0(_lon0)
        ,alt0(_alt0)
        ,lonF(_lonF)
        ,lonR(_lonR)
        ,lat(_lat)
        ,alt(_alt)
    {}

    void forward(double lat, double lon, double h, double& x, double& y, double& z);
    void reverse(double x, double y, double z, double& lat, double& lon, double& h);
    void setGroundOrigin(point_3d_t _groundOrigin);
    point_3d_t Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData);

    double lonF;
    double lonR;
    double lat;
    double alt;

private:
    double lat0;
    double lon0;
    double alt0;
    double ref_lat_offset;

};



#endif // PUB_TYPES_H
