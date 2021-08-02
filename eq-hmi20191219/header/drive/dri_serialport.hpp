#ifndef DRI_SERIALPORT_HPP
#define DRI_SERIALPORT_HPP

#include<thread>
#include"header/system/pub_types.h"
#include "dri_interface.hpp"
#include "pub_serialport.hpp"
extern std::mutex rev_com_mutex;

class ISERIALPORT_INS_D_COM1 : public ISERIALPORT
{
public:
    ISERIALPORT_INS_D_COM1(COM_CHAN _chan, sp_dev_e _dev)
        :m_error_code(0)
        ,channel(_chan)
        ,m_dev(_dev)
        ,m_sp(Serial())
        ,m_frms(vector<serial_frm_t>())
    {

        send_start = false;
        m_sendt = std::thread(sendThread, this);
        recv_start = false;
        m_recvt = std::thread(recvThread, this);
    }
    virtual ~ISERIALPORT_INS_D_COM1()
    {
        send_start = false;
        m_sendt.join();
        recv_start = false;
        m_recvt.join();
        close();
    }

    virtual bool init();//not implemented
    virtual bool open();
    virtual void close();
    virtual bool send(char *_buff, int _len);
    virtual vector<serial_frm_t> getFrms();
    virtual vector<vector<string>> getSerialStrings();
    virtual void clearSerialStrings();
    virtual void clearFrms();
    virtual err_t error();

    static void recvThread(void *_p);
    void recvTask();
    static void sendThread(void *_p);
    void sendTask();

    std::thread m_recvt;
    std::thread m_sendt;
    bool recv_start;
    bool send_start;

private:
    COM_CHAN channel;
    Serial m_sp;             //Serial Port
    QString m_sn;
    sp_dev_e m_dev;
    int m_serial_fd;
    vector<uint8_t> rev_buffer;
    vector<uint8_t> rev_buffer1;
    vector<serial_frm_t> m_frms;
    std::mutex m_mutex;
    err_t m_error_code;

    vector<vector<string>> serialStrings;
    void setSerialStrings(vector<string> _serialStrings);
};




#endif // DRI_SERIALPORT_HPP
