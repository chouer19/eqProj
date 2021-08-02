/*
 * =====================================================================
 * Include files
 * =====================================================================
 */
#include "header/drive/dri_can.hpp"
#include <stdlib.h>
#include <stdio.h>
#ifdef __linux
#include "controlcan.h"
#endif
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
/*CAN Receive Buffer Configuration*/
const uint8_t CAN_REV_BUF_NUM[6][8]
= {
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 1, 2, 3, 4, 5, 6, 7}
  }; 

const uint8_t CAN_REV_BUF_IDE[6][8]              //0 - standard frame, 1 - extend frame
= {
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0}
  };            



const uint8_t CAN_REV_BUF_IRQ[6][8]             //0 - disable interrupt, 1 - enable interrupt
= {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1}
  };
const uint32_t CAN_REV_BUF_ID[6][8]
= {
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN0
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN1
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN2
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN3
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN4
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}               //CAN5
  };

const uint32_t CAN_REV_BUF_MASK[6][8]
= {
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN0
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN1
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN2
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN3
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},              //CAN4
		{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}               //CAN5
  };

const uint32_t CAN_GLOBAL_BUF_MASK[6][3]
= {
//		Global Mask  Buf14 Mask  Buf15 Mask
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},   //CAN0
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},   //CAN1
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},   //CAN2
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},   //CAN3
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},   //CAN4
		{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}    //CAN5
};

const uint8_t DEVICE_SN_CHAN[8][20]
={
//    { 49, 49, 54, 48, 53, 51, 48, 49, 48, 49, 52,  0,  0,66,236,64,0,0,0,  0},            //CAN1
    { 49, 49, 54, 48, 53, 51, 48, 48, 57, 53, 56,  0,  0,168,189,2,60,145,127,  0},            //CAN0
     { 49, 49, 56, 49, 48, 51, 49, 48, 49, 52, 50,  0,  0,168,189, 2, 60, 2 ,127,  0}, //CAN1
    { 49, 49, 54, 48, 53, 51, 48, 48, 55, 48, 49,  0,  0,96,94,5,112,20,127,  0},            //CAN2
    { 49, 49, 54, 48, 53, 51, 48, 48, 53, 49, 48,  0,  0,96,94,5,88,117,127,  0},            //CAN3
    { 49, 49, 54, 48, 51, 49, 49, 48, 56, 53, 55,  0,  0,200,150,115,246,255,127,  0},            //CAN4
    { 49, 49, 54, 48, 51, 49, 49, 48, 54, 53, 57,  0,  0,200,150,115,246,255,127,  0},            //CAN5
    { 49, 49, 54, 48, 51, 49, 49, 48, 54, 51, 56,  0,  0,200,150,115,246,255,127,  0},            //CAN6
    { 49, 49, 54, 48, 53, 51, 48, 48, 50, 52, 56,  0,  0,200,150,115,246,255,127,  0}             //CAN7
};



/*
 * =====================================================================
 * Private constant definitions ('static')
 * =====================================================================
 */
#ifdef __linux

static std::mutex s_gate_mutex[8];
static vector<can_frm_t> gate_send_frms[8];
#endif

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
static ICAN_ZLG *s_icanInstance[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

#ifdef __linux
#ifdef ZLG_CAN
static std::mutex s_mutex;

static bool isOnlineChan[8] = {false, false, false, false, false, false, false, false};

static int indOfChan[8] = {0, 0, 0, 0, 0, 0, 0, 0};

static vector<int> devIndOpened;
#endif
#endif



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


ICAN_ZLG::ICAN_ZLG(enum CAN_CHANNEL chan,CCTIME *_time,ITASKLIST *_tlist)
    : ICAN(pub_time, TASK_CAN_PERIOD)
    , channel(chan)
    , time(_time)
    , taskList(_tlist)
    , isOnline(false)
    , m_error_code(0)
    , send_start(true)
    , recv_start(true)
{
    canIntcTaskVector.clear();
#ifdef __linux
    gate_send_frms[channel].clear();
    m_recvt = std::thread(recvThread, this);
    m_sendt = std::thread(sendThread, this);
#endif
    m_can_buff._size = 0;
}

ICAN_ZLG::~ICAN_ZLG()
{
    abort();
    taskList->erase(taskId);
#ifdef __linux
    if(isOnlineChan[channel])
    {
        int cnt = 0;
        int isclosed = 0;
        while((0 == isclosed) && (cnt < 10))
        {
            cnt++;
            isclosed = VCI_CloseDevice(DEVICE_TYPE, indOfChan[channel]);
        }
    }
    recv_start = false;
    m_recvt.join();
    send_start = false;
    m_sendt.join();

#endif

	s_icanInstance[channel] = NULL;
    canIntcTaskVector.clear();
}

void ICAN_ZLG::install()
{
    taskId = taskList->push_back(this);
}

void ICAN_ZLG::task_func()
{
    init();
}

bool ICAN_ZLG::init()
{
#ifdef __linux
#ifdef ZLG_CAN
    {
        std::unique_lock<std::mutex> lck (s_mutex);
        if(isOnlineChan[channel])
        {
            return true;
        }
        int new_dev_ind = 0;
        int _num_limit = 10;
        //find an unopened dev index
        for(int j = 0; j < _num_limit; j++)
        {
            int i = 0;
            for(i = 0; i < devIndOpened.size(); i++)
            {
                if(new_dev_ind == devIndOpened[i])
                {
                    new_dev_ind++;
                    break;
                }
            }

            if(i >= devIndOpened.size())
            {
                if(1 == VCI_OpenDevice(DEVICE_TYPE, new_dev_ind, 0))
                {
                    VCI_BOARD_INFO boardInfo;
                    if(1 == VCI_ReadBoardInfo(DEVICE_TYPE, i, &boardInfo))
                    {
                        for(int k = 0; k < 1; k++)
                        {
//                            if((boardInfo.str_Serial_Num[8] == DEVICE_SN_CHAN[k][8])
//                                    && (boardInfo.str_Serial_Num[9] == DEVICE_SN_CHAN[k][9])
//                                    && (boardInfo.str_Serial_Num[10] == DEVICE_SN_CHAN[k][10]))
                            {
                                VCI_INIT_CONFIG config;
                                config.AccCode = 0;
                                config.AccMask = 0xffffffff;
                                config.Filter = 1;
                                config.Mode = 0;
                                if((k == 1) /*|| (k == 3)*/)
                                {
                                    config.Timing0 = 0x1c00 & 0xff;
                                    config.Timing1 = 0x1c00 >> 8;
                                }
                                else
                                {
                                    config.Timing0 = 0x1c01 & 0xff;
                                    config.Timing1 = 0x1c01 >> 8;
                                }

                                if(1 == VCI_InitCAN(DEVICE_TYPE, new_dev_ind, 0, &config))
                                {
                                    if(1 == VCI_StartCAN(DEVICE_TYPE, new_dev_ind, 0))
                                    {
                                        indOfChan[k] = new_dev_ind;
                                        isOnlineChan[k] = true;
                                        devIndOpened.push_back(new_dev_ind);
                                        return true;
                                    }

                                }

                            }

                        }
                     }

                    int cnt = 0;
                    int isclosed = 0;
                    while((0 == isclosed) && (cnt < 10))
                    {
                        cnt++;
                        isclosed = VCI_CloseDevice(DEVICE_TYPE, new_dev_ind);
                    }
                    new_dev_ind++;
                }
                else
                    new_dev_ind++;
            }
        }
        return false;
    }

#endif
#else
	bool can_set_result = true;
	uint8_t can_not_ready = 1;
	uint32_t reset_flag = 1;
	uint16_t i = 0;
	uint16_t x = 0;
	uint8_t mb_num = 0;		
	switch(channel)
	{
		case CAN_CHANNEL0:
		{
			SIU.PCR[CAN0_TX].R = 0x0601; //Set Pin to be can tx
			SIU.PCR[CAN0_RX].R = 0x0101; //Set Pin to be can rx
			CAN_0.MCR.B.MDIS = 0;
			CAN_0.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_0.MCR.B.SOFTRST;
			}
			CAN_0.MCR.B.MDIS = 1;       //disable flexcan
			CAN_0.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_0.MCR.B.MDIS = 0;       //enable flexcan
			CAN_0.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_0.MCR.B.FRZACK == 0)
			{
				CAN_0.MCR.B.FRZ = 1;    //freezing
			}
			CAN_0.MCR.B.BCC = 1;        //single mask code
			CAN_0.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_0.MCR.B.WRNEN = 1; 
			CAN_0.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_0.MCR.B.FEN = 0;        //close FIFO
			CAN_0.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL0); //set baud rate
			mb_num = USE_NUM_MB_CAN0;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_0.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN0; x++)
			{
				CAN_0.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_0.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_0.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_0.RXGMASK.R = CAN_GLOBAL_BUF_MASK[0][0];       //全局掩码
				CAN_0.RX14MASK.R = CAN_GLOBAL_BUF_MASK[0][1];       //14邮箱掩码
				CAN_0.RX15MASK.R = CAN_GLOBAL_BUF_MASK[0][2];       //15邮箱掩码
			}
			CAN_0.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_0.IMRL.R = 0x00000000; 
			CAN_0.MCR.B.HALT = 0;
			CAN_0.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_0.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL0);
			break;			
		}
		case CAN_CHANNEL1:
		{
			SIU.PCR[CAN1_TX].R = 0x0601; //Set Pin to be can tx
			SIU.PCR[CAN1_RX].R = 0x0101; //Set Pin to be can rx
			SIU.PSMI[0].R = 0x01;        //Set CAN1_RX Pin to be can1 rx
			CAN_1.MCR.B.MDIS = 0;
			CAN_1.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_1.MCR.B.SOFTRST;
			}
			CAN_1.MCR.B.MDIS = 1;       //disable flexcan
			CAN_1.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_1.MCR.B.MDIS = 0;       //enable flexcan
			CAN_1.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_1.MCR.B.FRZACK == 0)
			{
				CAN_1.MCR.B.FRZ = 1;    //freezing
			}
			CAN_1.MCR.B.BCC = 1;        //single mask code
			CAN_1.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_1.MCR.B.WRNEN = 1; 
			CAN_1.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_1.MCR.B.FEN = 0;        //close FIFO
			CAN_1.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL1); //set baud rate
			mb_num = USE_NUM_MB_CAN1;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_1.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN1; x++)
			{
				CAN_1.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_1.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_1.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_1.RXGMASK.R = CAN_GLOBAL_BUF_MASK[1][0];       //全局掩码
				CAN_1.RX14MASK.R = CAN_GLOBAL_BUF_MASK[1][1];       //14邮箱掩码
				CAN_1.RX15MASK.R = CAN_GLOBAL_BUF_MASK[1][2];       //15邮箱掩码
			}
			CAN_1.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_1.IMRL.R = 0x00000000; 
			CAN_1.MCR.B.HALT = 0;
			CAN_1.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_1.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL1);
			break;			
		}
		case CAN_CHANNEL2:
		{
			SIU.PCR[CAN2_TX].R = 0x0600; //Set Pin to be can tx
			SIU.PCR[CAN2_RX].R = 0x0500; //Set Pin to be can rx
			SIU.PSMI[1].R = 0x00;        //Set CAN2_RX Pin to be can2 rx
			CAN_2.MCR.B.MDIS = 0;
			CAN_2.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_2.MCR.B.SOFTRST;
			}
			CAN_2.MCR.B.MDIS = 1;       //disable flexcan
			CAN_2.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_2.MCR.B.MDIS = 0;       //enable flexcan
			CAN_2.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_2.MCR.B.FRZACK == 0)
			{
				CAN_2.MCR.B.FRZ = 1;    //freezing
			}
			CAN_2.MCR.B.BCC = 1;        //single mask code
			CAN_2.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_2.MCR.B.WRNEN = 1; 
			CAN_2.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_2.MCR.B.FEN = 0;        //close FIFO
			CAN_2.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL2); //set baud rate
			mb_num = USE_NUM_MB_CAN0;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_2.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN2; x++)
			{
				CAN_2.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_2.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_2.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_2.RXGMASK.R = CAN_GLOBAL_BUF_MASK[2][0];       //全局掩码
				CAN_2.RX14MASK.R = CAN_GLOBAL_BUF_MASK[2][1];       //14邮箱掩码
				CAN_2.RX15MASK.R = CAN_GLOBAL_BUF_MASK[2][2];       //15邮箱掩码
			}
			CAN_2.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_2.IMRL.R = 0x00000000; 
			CAN_2.MCR.B.HALT = 0;
			CAN_2.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_2.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL2);
			break;			
		}
		case CAN_CHANNEL3:
		{
			SIU.PCR[CAN3_TX].R = 0x0601; //Set Pin to be can tx
			SIU.PCR[CAN3_RX].R = 0x0101; //Set Pin to be can rx
			SIU.PSMI[2].R = 0x02;        //Set CAN3_RX Pin to be can3 rx
			CAN_3.MCR.B.MDIS = 0;
			CAN_3.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_3.MCR.B.SOFTRST;
			}
			CAN_3.MCR.B.MDIS = 1;       //disable flexcan
			CAN_3.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_3.MCR.B.MDIS = 0;       //enable flexcan
			CAN_3.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_3.MCR.B.FRZACK == 0)
			{
				CAN_3.MCR.B.FRZ = 1;    //freezing
			}
			CAN_3.MCR.B.BCC = 1;        //single mask code
			CAN_3.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_3.MCR.B.WRNEN = 1; 
			CAN_3.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_3.MCR.B.FEN = 0;        //close FIFO
			CAN_3.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL3); //set baud rate
			mb_num = USE_NUM_MB_CAN3;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_3.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN3; x++)
			{
				CAN_3.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_3.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_3.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_3.RXGMASK.R = CAN_GLOBAL_BUF_MASK[3][0];       //全局掩码
				CAN_3.RX14MASK.R = CAN_GLOBAL_BUF_MASK[3][1];       //14邮箱掩码
				CAN_3.RX15MASK.R = CAN_GLOBAL_BUF_MASK[3][2];       //15邮箱掩码
			}
			CAN_3.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_3.IMRL.R = 0x00000000; 
			CAN_3.MCR.B.HALT = 0;
			CAN_3.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_3.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL3);
			break;			
		}
		case CAN_CHANNEL4:
		{
			SIU.PCR[CAN4_TX].R = 0x0A01; //Set Pin to be can tx
			SIU.PCR[CAN4_RX].R = 0x0101; //Set Pin to be can rx
			SIU.PSMI[3].R = 0x00;        //Set CAN4_RX Pin to be can4 rx
			CAN_4.MCR.B.MDIS = 0;
			CAN_4.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_4.MCR.B.SOFTRST;
			}
			CAN_4.MCR.B.MDIS = 1;       //disable flexcan
			CAN_4.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_4.MCR.B.MDIS = 0;       //enable flexcan
			CAN_4.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_4.MCR.B.FRZACK == 0)
			{
				CAN_4.MCR.B.FRZ = 1;    //freezing
			}
			CAN_4.MCR.B.BCC = 1;        //single mask code
			CAN_4.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_4.MCR.B.WRNEN = 1; 
			CAN_4.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_4.MCR.B.FEN = 0;        //close FIFO
			CAN_4.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL4); //set baud rate
			mb_num = USE_NUM_MB_CAN4;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_4.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN4; x++)
			{
				CAN_4.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_4.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_4.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_4.RXGMASK.R = CAN_GLOBAL_BUF_MASK[4][0];       //全局掩码
				CAN_4.RX14MASK.R = CAN_GLOBAL_BUF_MASK[4][1];       //14邮箱掩码
				CAN_4.RX15MASK.R = CAN_GLOBAL_BUF_MASK[4][2];       //15邮箱掩码
			}
			CAN_4.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_4.IMRL.R = 0x00000000; 
			CAN_4.MCR.B.HALT = 0;
			CAN_4.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_4.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL4);
			break;			
		}
		case CAN_CHANNEL5:
		{
			SIU.PCR[CAN5_TX].R = 0x0A01; //Set Pin to be can tx
			SIU.PCR[CAN5_RX].R = 0x0101; //Set Pin to be can rx
			SIU.PSMI[4].R = 0x00;        //Set CAN5_RX Pin to be can5 rx
			CAN_5.MCR.B.MDIS = 0;
			CAN_5.MCR.B.SOFTRST = 1;    //soft reset
			while(reset_flag == 1)
			{
				reset_flag = CAN_5.MCR.B.SOFTRST;
			}
			CAN_5.MCR.B.MDIS = 1;       //disable flexcan
			CAN_5.CR.B.CLKSRC = CAN_CLK_SEL;    //Can clock source select
			CAN_5.MCR.B.MDIS = 0;       //enable flexcan
			CAN_5.MCR.B.HALT = 1;       //halt flexcan
			while(CAN_5.MCR.B.FRZACK == 0)
			{
				CAN_5.MCR.B.FRZ = 1;    //freezing
			}
			CAN_5.MCR.B.BCC = 1;        //single mask code
			CAN_5.MCR.B.SRXDIS = 1;     //self recieve mode set
			CAN_5.MCR.B.WRNEN = 1; 
			CAN_5.MCR.B.IDAM = 0;    //FIFO set FORMAT
			CAN_5.MCR.B.FEN = 0;        //close FIFO
			CAN_5.CR.B.RJW = 0;
			can_set_result = setBaudRate(CAN_CHANNEL5); //set baud rate
			mb_num = USE_NUM_MB_CAN5;
			if(mb_num >= CAN_MAX_MB)
			{
				mb_num = CAN_MAX_MB;
			}
			CAN_5.MCR.B.MAXMB = mb_num - 1;      //设置使用报文缓冲区个数
			for(x = 0;x < USE_NUM_MB_CAN5; x++)
			{
				CAN_5.BUF[x].CS.R = 0;                       //清空BUF寄存器第一行
				CAN_5.BUF[x].ID.R = 0;                       //清空BUF寄存器第二行
				for(i = 0; i < 2; i++)
				{
					CAN_5.BUF[x].DATA.W[i] = 0;              //清空BUF寄存器第三四行
				}
			}
			if(can_set_result == true)                       //波特率设置成功
			{
				CAN_5.RXGMASK.R = CAN_GLOBAL_BUF_MASK[5][0];       //全局掩码
				CAN_5.RX14MASK.R = CAN_GLOBAL_BUF_MASK[5][1];       //14邮箱掩码
				CAN_5.RX15MASK.R = CAN_GLOBAL_BUF_MASK[5][2];       //15邮箱掩码
			}
			CAN_5.IMRH.R = 0x00000000;                       //禁止每一个缓冲区中断
			CAN_5.IMRL.R = 0x00000000; 
			CAN_5.MCR.B.HALT = 0;
			CAN_5.MCR.B.FRZ = 0;                             //退出冻结模式
			while(can_not_ready == 1)                       //进入正常模式
			{
				can_not_ready = CAN_5.MCR.B.NOTRDY;
			}
			
			setRecvBuffer(CAN_CHANNEL5);
			break;			
		}
		default:
		{
			break;
		}
	}
#endif
}

enum CAN_CHANNEL ICAN_ZLG::chan(void)
{
	return channel;
}

bool ICAN_ZLG::send(const can_frm_t *frm)
{
#ifdef __linux
#ifdef ZLG_CAN
    if(!isOnlineChan[channel])
    {
        m_error_code |= (1 << ERR_BIT_COM_CAN);
        return false;
    }
    VCI_CAN_OBJ can_obj;
    can_obj.ID = frm->canid;
    can_obj.RemoteFlag = frm->RTR;
    can_obj.ExternFlag = frm->IDE;
    can_obj.DataLen = frm->length;
    can_obj.SendType = 0;            //send once no resend
    for(int i = 0; i < frm->length; i++)
        can_obj.Data[i] = frm->data[i];
    {
        std::unique_lock<std::mutex> lck0 (m_mutex);
        if(m_can_buff._size >= 500)
            return false;
        m_can_buff._obj[m_can_buff._size] = can_obj;
        m_can_buff._size++;
//        if(1 != VCI_Transmit(DEVICE_TYPE, indOfChan[channel], 0, &can_obj, 1))
//        {
//            VCI_ERR_INFO errInfo;
//            VCI_ReadErrInfo(DEVICE_TYPE, indOfChan[channel],0, &errInfo);
//    #if(CAN_AUTO_CONNECT == 1)
//            std::unique_lock<std::mutex> lck (s_mutex);
//            int cnt = 0;
//            int isclosed = 0;
//            while((0 == isclosed) && (cnt < 10))
//            {
//                cnt++;
//                isclosed = VCI_CloseDevice(DEVICE_TYPE, indOfChan[channel]);
//            }
//            isOnlineChan[channel] = false;
//            for(vector<int>::iterator it = devIndOpened.begin(); it != devIndOpened.end();)
//            {
//                if(*it == indOfChan[channel])
//                {
//                    devIndOpened.erase(it);
//                    break;
//                }
//                else
//                    it++;
//            }
//    #endif
//            m_error_code |= (1 << ERR_BIT_COM_CAN);
//            return false;
//        }
//        m_error_code &= ~(1 << ERR_BIT_COM_CAN);
    }
    return true;
#endif
#ifdef ZMQ_CAN
    uint8_t can_buff0[16];
    uint8_t mb_num = frm->MB_NUM;
    uint32_t id = frm->canid;
    memcpy(&can_buff0[0], &id, 4);
    memcpy(&can_buff0[4], &frm->RTR, 1);
    memcpy(&can_buff0[5], &frm->IDE, 1);
    for(uint8_t j = 0; j < 8; j++)
        memcpy(&can_buff0[6+j], &frm->data[j], 1);
    memcpy(&can_buff0[14], &frm->length, 1);
    memcpy(&can_buff0[15], &mb_num, 1);

    if(16 != PubMsg("IDC_CAN5_TX", can_buff0, 16))
    {
        cout << "ERROR: CRO Send Failed !!" << endl;
        return false;
    }
    return true;
#endif
#else
	uint8_t sendindex;
	uint32_t tick_count = 0;
	bool Can_Send_Result = false;	
	switch(channel)
	{
		case CAN_CHANNEL0:
		{
			if(CAN_0.ESR.B.ACKERR != 1)
			{
				while((CAN_0.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_0.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_0.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_0.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}				
				CAN_0.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_0.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_0.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_0.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_0.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true; 		
			}
			else
			{				
				Can_Send_Result = false; 		
			}
			break;
		}
		case CAN_CHANNEL1:
		{
			if(CAN_1.ESR.B.ACKERR != 1)
			{
				while((CAN_1.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_1.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_1.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_1.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}
				CAN_1.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_1.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_1.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_1.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_1.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true; 			
			}
			else
			{
				Can_Send_Result = false; 				
			}
			break;
		}
		case CAN_CHANNEL2:
		{
			if(CAN_2.ESR.B.ACKERR != 1)
			{
				while((CAN_2.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_2.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_2.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_2.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}
				CAN_2.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_2.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_2.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_2.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_2.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true;		
			}
			else
			{
				Can_Send_Result = false;
			}
			break;
		}
		case CAN_CHANNEL3:
		{
			if(CAN_3.ESR.B.ACKERR != 1)
			{
				while((CAN_3.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_3.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_3.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_3.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}
				CAN_3.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_3.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_3.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_3.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_3.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true; 
			}
			else
			{
				Can_Send_Result = false; 	
			}
			break;
		}
		case CAN_CHANNEL4:
		{
			if(CAN_4.ESR.B.ACKERR != 1)
			{
				while((CAN_4.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_4.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_4.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_4.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}
				CAN_4.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_4.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_4.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_4.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_4.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true; 
			}
			else
			{
				Can_Send_Result = false;
			}
			break;
		}
		case CAN_CHANNEL5:
		{
			if(CAN_5.ESR.B.ACKERR != 1)
			{
				while((CAN_5.BUF[frm->MB_NUM].CS.B.CODE  == CAN_TX_MSG)&&(tick_count <= CAN_OVERTIME))
				{
					tick_count++;
				}
			}
			if(tick_count <= CAN_OVERTIME)
			{
				CAN_5.BUF[frm->MB_NUM].CS.B.IDE = frm->IDE;
				if(frm->IDE == 1)
				{
					CAN_5.BUF[frm->MB_NUM].ID.R = frm->canid;
				}
				else
				{
					CAN_5.BUF[frm->MB_NUM].ID.B.STD_ID = frm->canid;
				}
				CAN_5.BUF[frm->MB_NUM].CS.B.RTR = frm->RTR;
				CAN_5.BUF[frm->MB_NUM].CS.B.LENGTH = frm->length;
				
				for(sendindex = 0; sendindex < frm->length; sendindex++)
				{
					CAN_5.BUF[frm->MB_NUM].DATA.B[sendindex] = frm->data[sendindex];
				}
				
				CAN_5.BUF[frm->MB_NUM].CS.B.SRR = 1;
				CAN_5.BUF[frm->MB_NUM].CS.B.CODE = CAN_TX_MSG;
				
				Can_Send_Result = true;
			}
			else
			{
				Can_Send_Result = false;
			}
			break;
		}
		default:
		{
			Can_Send_Result = false; 
		}
	}
	return Can_Send_Result;
#endif
}

bool ICAN_ZLG::zlg_send()
{
    if(m_can_buff._size == 0)
        return true;
    std::unique_lock<std::mutex> lck0 (m_can_mutex);
    bool _ok = (m_can_buff._size == VCI_Transmit(DEVICE_TYPE, indOfChan[channel], 0, m_can_buff._obj, m_can_buff._size));

    m_can_buff._size = 0;
    return _ok;
}

bool ICAN_ZLG::recv(int &buff, can_frm_t *frm)
{
#ifdef __linux
#ifdef ZLG_CAN
    VCI_CAN_OBJ can_obj[1000];
    if(!isOnlineChan[channel])
    {
        buff = 0;
        m_error_code |= (1 << ERR_BIT_COM_CAN);
        return false;
    }
    {
        std::unique_lock<std::mutex> lck (m_can_mutex);
        buff = VCI_Receive(DEVICE_TYPE, indOfChan[channel], 0, can_obj, 1000, 5);
    }

    if(buff > 0)
    {
//        qDebug()<<buff;
        for(int i = 0; i < buff; i++)
        {
            frm[i].canid = can_obj[i].ID;
            frm[i].RTR = can_obj[i].RemoteFlag;
            frm[i].IDE = can_obj[i].ExternFlag;
            frm[i].length = can_obj[i].DataLen;
            frm[i].MB_NUM = 0xFF;
            for(int j = 0; j < frm[i].length; j++)
                frm[i].data[j] = can_obj[i].Data[j];
        }
    }
    else
    {
        VCI_ERR_INFO err_info;
        VCI_ReadErrInfo(DEVICE_TYPE, indOfChan[channel], 0, &err_info);

    }
    m_error_code &= ~(1 << ERR_BIT_COM_CAN);
    return true;
#endif
#ifdef ZMQ_CAN
    uint8_t can_buff0[160];
//    size_t time_stamp = pub_time->getTimeMs();
    int bytes0 = 0;
    if(CAN_CHANNEL0 == channel)
        bytes0 = SubMsg("CAN0", can_buff0, 160);
    else if(CAN_CHANNEL1 == channel)
        bytes0 = SubMsg("CAN1", can_buff0, 160);
    else if(CAN_CHANNEL2 == channel)
        bytes0 = SubMsg("CAN2", can_buff0, 160);
    else if(CAN_CHANNEL3 == channel)
        bytes0 = SubMsg("CAN3", can_buff0, 160);
    else if(CAN_CHANNEL4 == channel)
        bytes0 = SubMsg("CAN4", can_buff0, 160);
    else if(CAN_CHANNEL5 == channel)
    {
        bytes0 = SubMsg("TOOL_CAN5_TX", can_buff0, 160);
    }
    else{}

    if((bytes0 > 0) && (bytes0 % 16 == 0))
    {
        buff = bytes0 / 16;
        for(size_t i = 0; i < buff; i++)
        {
            memcpy(&frm[i].canid, &can_buff0[0+16*i], 4);
            memcpy(&frm[i].RTR, &can_buff0[4+16*i], 1);
            memcpy(&frm[i].IDE, &can_buff0[5+16*i], 1);
            for(uint8_t j = 0; j < 8; j++)
                memcpy(&frm[i].data[j], &can_buff0[6+j+16*i], 1);
            memcpy(&frm[i].length, &can_buff0[14+16*i], 1);
            memcpy(&frm[i].MB_NUM, &can_buff0[15+16*i], 1);
        }
    }
    return true;
#endif
#else
	uint8_t byteindex;
	bool CAN_Read_Result = false;
	uint32_t dumy = 0;
	switch(channel)
	{
		case CAN_CHANNEL0:
		{
			frm->MB_NUM = buff;
			if(CAN_0.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_0.BUF[buff].ID.B.STD_ID;
				if(CAN_0.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_0.BUF[buff].CS.B.IDE == EXTEND_ID)
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_0.BUF[buff].ID.R;
				if(CAN_0.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_0.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_0.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_0.TIMER.R;
			CAN_Read_Result = true; 
			break;
		}		
		case CAN_CHANNEL1:
		{
			frm->MB_NUM = buff;
			if(CAN_1.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_1.BUF[buff].ID.B.STD_ID;
				if(CAN_1.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_1.BUF[buff].CS.B.IDE == EXTEND_ID)
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_1.BUF[buff].ID.R;
				if(CAN_1.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_1.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_1.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_1.TIMER.R;
			CAN_Read_Result = true; 
			break;
		}
		case CAN_CHANNEL2:
		{
			frm->MB_NUM = buff;
			if(CAN_2.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_2.BUF[buff].ID.B.STD_ID;
				if(CAN_2.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_2.BUF[buff].CS.B.IDE == EXTEND_ID) 
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_2.BUF[buff].ID.R;
				if(CAN_2.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_2.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_2.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_2.TIMER.R;
			CAN_Read_Result = true; 
			break;
		}		
		case CAN_CHANNEL3:
		{
			frm->MB_NUM = buff;
			if(CAN_3.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_3.BUF[buff].ID.B.STD_ID;
				if(CAN_3.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_3.BUF[buff].CS.B.IDE == EXTEND_ID) 
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_3.BUF[buff].ID.R;
				if(CAN_3.BUF[buff].CS.B.RTR == REMOTE_FORMAT) 
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_3.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_3.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_3.TIMER.R;
			CAN_Read_Result = true; 
			break;
		}
		case CAN_CHANNEL4:
		{
			frm->MB_NUM = buff;
			if(CAN_4.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_4.BUF[buff].ID.B.STD_ID;
				if(CAN_4.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_4.BUF[buff].CS.B.IDE == EXTEND_ID)
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_4.BUF[buff].ID.R;
				if(CAN_4.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_4.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_4.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_4.TIMER.R; 
			CAN_Read_Result = true; 
			break;
		}

		case CAN_CHANNEL5:
		{
			frm->MB_NUM = buff;
			if(CAN_5.BUF[buff].CS.B.IDE == STANDARD_ID)
			{
				frm->IDE = STANDARD_ID;
				frm->canid = CAN_5.BUF[buff].ID.B.STD_ID;
				if(CAN_5.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;					
				}			
			}
			if(CAN_5.BUF[buff].CS.B.IDE == EXTEND_ID)
			{
				frm->IDE = EXTEND_ID;
				frm->canid = CAN_5.BUF[buff].ID.R;
				if(CAN_5.BUF[buff].CS.B.RTR == REMOTE_FORMAT)
				{
					frm->RTR = REMOTE_FORMAT;
				}
				else
				{
					frm->RTR = DATA_FORMAT;
				}
			}
			frm->length = CAN_5.BUF[buff].CS.B.LENGTH;
			for(byteindex = 0; byteindex < frm->length; byteindex++)
			{
				frm->data[byteindex] = CAN_5.BUF[buff].DATA.B[byteindex];
			}
			dumy = CAN_5.TIMER.R;
			CAN_Read_Result = true; 
			break;
		}		
		default:
		{
			CAN_Read_Result = false;
			break;
		}

	}
	return CAN_Read_Result;
#endif
}

void ICAN_ZLG::installIntcFunc()
{
	switch(channel)
	{
		case CAN_CHANNEL0:
		{
			s_icanInstance[0] = this;
            break;
		}
		case CAN_CHANNEL1:
		{
			s_icanInstance[1] = this;
            break;
		}
		case CAN_CHANNEL2:
		{
			s_icanInstance[2] = this;
            break;
		}
		case CAN_CHANNEL3:
		{
			s_icanInstance[3] = this;
            break;
		}
		case CAN_CHANNEL4:
		{
			s_icanInstance[4] = this;
            break;
		}
		case CAN_CHANNEL5:
		{
			s_icanInstance[5] = this;
            break;
		}
		default: break;
	}
}

void ICAN_ZLG::addIntcTask(can_frm_timed_t frm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    canIntcTaskVector.push_back(frm);
}

vector<can_frm_timed_t> ICAN_ZLG::getIntcTaskList()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return canIntcTaskVector;
}

void ICAN_ZLG::clearIntcTaskList()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    canIntcTaskVector.clear();
}


bool ICAN_ZLG::setBaudRate(enum CAN_CHANNEL channel)
{
#ifdef __linux
    return true;
#else
	bool Error_Code = true;
	switch(channel)
	{
		case CAN_CHANNEL0:
		{
			#if(CAN0_BAUD_RATE == 250)
				CAN_0.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_0.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_0.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_0.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN0_BAUD_RATE == 500)
				CAN_0.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_0.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_0.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_0.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN0_BAUD_RATE == 1000)
				CAN_0.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_0.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_0.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_0.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}		
		case CAN_CHANNEL1:
		{
			#if(CAN1_BAUD_RATE == 250)
				CAN_1.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_1.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_1.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_1.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN1_BAUD_RATE == 500)
				CAN_1.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_1.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_1.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_1.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN1_BAUD_RATE == 1000)
				CAN_1.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_1.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_1.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_1.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}
		case CAN_CHANNEL2:
		{
			#if(CAN2_BAUD_RATE == 250)
				CAN_2.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_2.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_2.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_2.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN2_BAUD_RATE == 500)
				CAN_2.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_2.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_2.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_2.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN2_BAUD_RATE == 1000)
				CAN_2.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_2.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_2.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_2.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}
		case CAN_CHANNEL3:
		{
			#if(CAN3_BAUD_RATE == 250)
				CAN_3.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_3.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_3.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_3.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN3_BAUD_RATE == 500)
				CAN_3.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_3.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_3.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_3.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN3_BAUD_RATE == 1000)
				CAN_3.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_3.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_3.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_3.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}
		case CAN_CHANNEL4:
		{
			#if(CAN4_BAUD_RATE == 250)
				CAN_4.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_4.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_4.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_4.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN4_BAUD_RATE == 500)
				CAN_4.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_4.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_4.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_4.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN4_BAUD_RATE == 1000)
				CAN_4.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_4.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_4.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_4.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}
		case CAN_CHANNEL5:
		{
			#if(CAN5_BAUD_RATE == 250)
				CAN_5.CR.B.PRESDIV = CAN_250kb_PRESDIV;
				CAN_5.CR.B.PSEG1 = CAN_250kb_PSEG1;
				CAN_5.CR.B.PSEG2 = CAN_250kb_PSEG2; 
				CAN_5.CR.B.PROPSEG = CAN_250kb_PROPSEG;
			#endif
			#if(CAN5_BAUD_RATE == 500)
				CAN_5.CR.B.PRESDIV = CAN_500kb_PRESDIV;
				CAN_5.CR.B.PSEG1 = CAN_500kb_PSEG1;
				CAN_5.CR.B.PSEG2 = CAN_500kb_PSEG2; 
				CAN_5.CR.B.PROPSEG = CAN_500kb_PROPSEG;
			#endif
			#if(CAN5_BAUD_RATE == 1000)
				CAN_5.CR.B.PRESDIV = CAN_1Mb_PRESDIV;
				CAN_5.CR.B.PSEG1 = CAN_1Mb_PSEG1;
				CAN_5.CR.B.PSEG2 = CAN_1Mb_PSEG2; 
				CAN_5.CR.B.PROPSEG = CAN_1Mb_PROPSEG;
			#endif
				
			break;
		}		
		default:
		{
			Error_Code = false;
			break;
		}		
	}
	return Error_Code;
#endif
}


err_t ICAN_ZLG::error()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_error_code;
}

void ICAN_ZLG::err_check()
{

}

#ifdef __linux
void ICAN_ZLG::recvThread(void *_p)
{
    ICAN_ZLG *_this = (ICAN_ZLG*)_p;
    _this->recvTask();
}

void ICAN_ZLG::recvTask()
{
    while(recv_start)
    {
        //Send Gate data here
        {
            std::unique_lock<std::mutex> lck(s_gate_mutex[channel]);
            size_t _num = 0;
            while((!gate_send_frms[channel].empty()) && (_num < 50))
            {
                _num++;
                send(&gate_send_frms[channel][0]);
                gate_send_frms[channel].erase(gate_send_frms[channel].begin());
            }
            gate_send_frms[channel].clear();
        }

        can_frm_t canrev[1000];
        can_frm_timed_t canrev_time;
        int len = 0;
        size_t _time_stamp = pub_time->getTimeMs();
        if(recv(len, canrev))
        {
            for(int i = 0; i < len; i++)
            {

                canrev_time.time_stamp = _time_stamp;
                canrev_time.can_frame = canrev[i];
                {                    
                    std::unique_lock<std::mutex> lck1(m_mutex);
                    canIntcTaskVector.push_back(canrev_time);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}


void ICAN_ZLG::sendThread(void *_p)
{
    ICAN_ZLG *_this = (ICAN_ZLG*)_p;
    _this->sendTask();
}

void ICAN_ZLG::sendTask()
{
    while(send_start)
    {
        //Send process
        if(!zlg_send())
            m_error_code |= (1 << ERR_BIT_COM_CAN);
        else
            m_error_code &= ~(1 << ERR_BIT_COM_CAN);
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
#endif
/*
 * =====================================================================
 * Global function definitions
 * =====================================================================
 */




