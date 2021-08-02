#ifndef DRI_INTERFACE_HPP
#define DRI_INTERFACE_HPP

#include"header/system/pub_types.h"

/*****************Can Interface***************************/
enum CAN_CHANNEL            //CAN channels
{
    CAN_CHANNEL0 = 0,
    CAN_CHANNEL1,
    CAN_CHANNEL2,
    CAN_CHANNEL3,
    CAN_CHANNEL4,
    CAN_CHANNEL5,
    CAN_CHANNEL6,
    CAN_CHANNEL7
};
/* Can Frame Type Define */
class can_frm_t
{
public:

    can_frm_t()
    :canid(0), RTR(0), IDE(0), length(0), MB_NUM(0)
    {
        for(uint8_t i = 0; i < 8; i++)
            data[i] = 0;
    }
    can_frm_t(const uint32_t &_id,
                 const uint8_t &_rtr,
                 const uint8_t &_ide,
                 const uint8_t *_data,
                 const uint8_t &_len,
                 const uint8_t _num)
    :canid(_id), RTR(_rtr), IDE(_ide), length(_len), MB_NUM(_num)
    {
        for(uint8_t i = 0; i < 8; i++)
            data[i] = _data[i];
    }

    void calcChecksum()
    {
        uint canDataSum=0;
        for(int i=0;i<7;i++)
        {
            canDataSum+=data[i];
        }
        canDataSum+=(data[7]>>4)&0x0f;
        canDataSum=((canDataSum >> 4)+canDataSum)&0x0f;
        data[7]+=canDataSum;
    }

    bool checksum()
    {
        uint canDataSum=0;
        for(int i=0;i<7;i++)
        {
            canDataSum+=data[i];
        }
        canDataSum+=(data[7]>>4)&0x0f;
        canDataSum=((canDataSum >> 4)+canDataSum)&0x0f;

        uint checksum=data[7]&0x0f;

        if(canDataSum==checksum)
            return true;
        else
            return false;
    }

    uint32_t canid;
    uint8_t RTR;
    uint8_t IDE;
    uint8_t data[8];
    uint8_t length;
    uint8_t MB_NUM;
};

struct can_frm_timed_t
{
    can_frm_timed_t()
    :time_stamp(0)
    {}
    can_frm_timed_t(const size_t &t, const can_frm_t &frm)
    :time_stamp(t), can_frame(frm)
    {}
    size_t time_stamp;
    can_frm_t can_frame;
};

/* Can Interface Declaration */


DeclareBasedInterface(ICAN, TASK)
    ICAN()
    :TASK(pub_time, 50)
    {}
    ICAN(CCTIME *_time, uint8_t _period)
    :TASK(_time, _period)
    {}
    virtual void install() = 0;
    //basic interfaces
    virtual bool init(void) = 0;                                    //interface to init can module
    virtual enum CAN_CHANNEL chan(void) = 0;

    virtual bool send(const can_frm_t *frm) = 0;                 //interface to send a can frame
    virtual bool recv(int &buff, can_frm_t *frm) = 0;         //interface to recieve a can frame

    //can interrupt interfaces
    virtual void installIntcFunc() = 0;
    virtual void addIntcTask(can_frm_timed_t frm) = 0;
    virtual vector<can_frm_timed_t> getIntcTaskList() = 0;
    virtual void clearIntcTaskList() = 0;

    virtual err_t error() = 0;
EndInterface

/*****************Serial Port Interface**************************/
enum serial_src_e
{
    NO_Frm = 0,
    Nav_Frm,
    Eps_Frm,
    Ebs_Frm,
    Imu_Frm
};


struct serial_frm_t
{
    serial_src_e frm_src;
    struct{
        size_t time_stamp;
        vector<uint8_t> datas;
    } frm;
};

enum COM_CHAN
{
    COM_CHAN0 = 0,
    COM_CHAN1
};

DeclareInterface(ISERIALPORT)
    virtual bool init() = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool send(char *_buff, int _len) = 0;
    virtual vector<serial_frm_t> getFrms() = 0;
    virtual void clearFrms() = 0;
    virtual vector<vector<string>> getSerialStrings()=0;
    virtual void clearSerialStrings()=0;
    virtual err_t error() = 0;
EndInterface

/*****************File**************************/
DeclareInterface(IFILE)
    virtual QByteArray read(string _filename)=0;
    virtual bool write(string _filename,QByteArray _data)=0;

EndInterface

class DRIVE
{
public:
    DRIVE();
    ~DRIVE();

    /* CAN channels */
    ICAN *can[1];

    /* RS232 */
    ISERIALPORT *rs232_ins_d_com1;

    /* File Txt */
    IFILE *txt;

};


#endif // DRI_INTERFACE_HPP
