#ifndef DRI_CAN_H_
#define DRI_CAN_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "header/system/pub_types.h"
#include "dri_interface.hpp"
#include "header/system/sys_interface.hpp"
#include "controlcan.h"



/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */
/* Can device */
enum{ DEVICE_TYPE = 4 };

/* Can Configurations */
#define ZLG_CAN
//#define ZMQ_CAN

/* Baud Rate Configurations */
#define CAN0_BAUD_RATE     250             //kb, when set 1mb, use 1000
#define CAN1_BAUD_RATE     250             //kb, when set 1mb, use 1000
#define CAN2_BAUD_RATE     250             //kb, when set 1mb, use 1000
#define CAN3_BAUD_RATE     250             //kb, when set 1mb, use 1000
#define CAN4_BAUD_RATE     500             //kb, when set 1mb, use 1000
#define CAN5_BAUD_RATE     250             //kb, when set 1mb, use 1000

/* CAN Module Configurations */
enum
{
	USE_NUM_MB_CAN0 = 32,                 //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
	USE_NUM_MB_CAN1 = 32,                 //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
	USE_NUM_MB_CAN2 = 32,                 //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
	USE_NUM_MB_CAN3 = 32,                 //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
	USE_NUM_MB_CAN4 = 32,                 //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
	USE_NUM_MB_CAN5 = 32                  //最大可用邮箱个数 = MAX_NUM_MB+1,最大可用个数为64
};

/* CAN Send Buffer num Configuration */
enum
{
	SEND_BUFF_NUM0 = 8,
	SEND_BUFF_NUM1 = 9,
	SEND_BUFF_NUM2 = 10,
	SEND_BUFF_NUM3 = 11,
	SEND_BUFF_NUM4 = 12,
	SEND_BUFF_NUM5 = 13,
	SEND_BUFF_NUM6 = 14,
	SEND_BUFF_NUM7 = 15
};



/* Definitions */

enum
{
	CAN_OVERTIME = 20000,
	CAN_INITDELAYTIME = 2
};

enum
{
	STANDARD_ID = 0,
	EXTEND_ID = 1
};

enum
{
	DATA_FORMAT = 0,
	REMOTE_FORMAT = 1
};

enum{ 
	CAN_MAX_MB = 64,           //报文最大缓冲区个数
	CAN_TX_MSG = 0x0C,         //传输报文
	CAN_RX_EMPTY = 0x04,       //设置RX接收为空
	TX_CODE_INACTIVE = 0x08,   //TX缓冲区未被激活
	RX_CODE_INACTIVE = 0x00,   //RX缓冲区未被激活
	TX_CODE_SUCCESS_RTR1 = 0x04,//RTR为1时发送成功后CODE的返回值
	TX_CODE_SUCCESS_RTR0 = 0x08 //RTR为0时发送成功后CODE的返回值
};

enum{ TASK_CAN_PERIOD = 1000 };

/*
 * =====================================================================
 * Global type definitions ('typedef')
 * =====================================================================
 */

class ICAN_ZLG : implements ICAN
{
public:
	//(de)constructor
    ICAN_ZLG(enum CAN_CHANNEL chan,  CCTIME *_time, ITASKLIST *_tlist);
    virtual ~ICAN_ZLG();
    virtual void install();
    virtual bool init(void);                                    //interface to init can module
	virtual enum CAN_CHANNEL chan(void);

    virtual bool send(const can_frm_t *frm);                 //interface to send a can frame
    bool zlg_send();
    virtual bool recv(int &buff, can_frm_t *frm);         //interface to recieve a can frame

	virtual void installIntcFunc();
    virtual void addIntcTask(can_frm_timed_t frm);
    virtual vector<can_frm_timed_t> getIntcTaskList();
    virtual void clearIntcTaskList();

    virtual err_t error();

    TaskFuncDeclare(ICAN_ZLG, can_connect, (TASK_CAN_PERIOD+100))

#ifdef __linux
    static void recvThread(void *_p);
    void recvTask();
    static void sendThread(void *_p);
    void sendTask();

    std::thread m_recvt;
    std::thread m_sendt;
    bool recv_start;
    bool send_start;
#endif
	
public:
    //interfaces used

    CCTIME *time;

    ITASKLIST *taskList;
	
private:
	bool setBaudRate(CAN_CHANNEL channel);
    void setRecvBuffer(CAN_CHANNEL channel);
	
private:
	enum CAN_CHANNEL channel;	
    vector<can_frm_timed_t> canIntcTaskVector;
    struct{
        int _size;
        VCI_CAN_OBJ _obj[500];
    } m_can_buff;

#ifdef __linux
    std::mutex m_mutex;
    std::mutex m_can_mutex;

#ifdef ZLG_CAN
    bool isDevOpend;
    bool isOnline;
#endif
#endif
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

//extern void initCAN(enum CAN_CHANNEL channel);


extern void CAN0_BUF00_03_ExternalIRQHandler();

extern void CAN0_BUF04_07_ExternalIRQHandler();

extern void CAN1_BUF00_03_ExternalIRQHandler();

extern void CAN1_BUF04_07_ExternalIRQHandler();

extern void CAN2_BUF00_03_ExternalIRQHandler();

extern void CAN2_BUF04_07_ExternalIRQHandler();

extern void CAN3_BUF00_03_ExternalIRQHandler();

extern void CAN3_BUF04_07_ExternalIRQHandler();

extern void CAN4_BUF00_03_ExternalIRQHandler();

extern void CAN4_BUF04_07_ExternalIRQHandler();

extern void CAN5_BUF00_03_ExternalIRQHandler();

extern void CAN5_BUF04_07_ExternalIRQHandler();


#endif /* DRI_CAN_H_ */
