#include "header/drive/dri_serialport.hpp"


#ifdef __linux

std::mutex rev_com_mutex;

bool ISERIALPORT_INS_D_COM1::init()
{

    return true;
}

bool ISERIALPORT_INS_D_COM1::open()
{
    m_serial_fd =  m_sp.OpenPort(m_dev);
    if (m_serial_fd < 0)
    {
         cout << "Open serial port " << m_dev << " failed!" << endl;
         return false;
     }
    else
    {
        cout << "Open serial port " << m_dev << " success!" << endl;
    }

    m_sp.SetPara(m_serial_fd, 6);//change baudrate to 115200
    send_start = true;
    recv_start = true;

    return true;
}


void ISERIALPORT_INS_D_COM1::close()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_sp.ClosePort(m_serial_fd);
}

bool ISERIALPORT_INS_D_COM1::send(char *_buff, int _len)
{
    bool _ok = false;
    if(_len > 0)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        _ok = (m_sp.WriteData(m_serial_fd,_buff,_len) >= 0);
        if(!_ok)
            m_error_code |= (1 << ERR_BIT_COM_RS232);
        else
            m_error_code &= ~(1 << ERR_BIT_COM_RS232);

    }
    else
        return true;

    this_thread::sleep_for(chrono::milliseconds(1));

    return _ok;
}

vector<serial_frm_t> ISERIALPORT_INS_D_COM1::getFrms()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_frms;
}

vector<vector<string>> ISERIALPORT_INS_D_COM1::getSerialStrings()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return serialStrings;
}

void ISERIALPORT_INS_D_COM1::clearSerialStrings()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vector<vector<string>> _serialStrings;
    serialStrings=_serialStrings;
//    serialStrings.clear();
}

void ISERIALPORT_INS_D_COM1::clearFrms()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_frms.clear();
}

err_t ISERIALPORT_INS_D_COM1::error()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_error_code;
}

void ISERIALPORT_INS_D_COM1::recvThread(void *_p)
{
    ISERIALPORT_INS_D_COM1 *_this = (ISERIALPORT_INS_D_COM1*)_p;
    _this->recvTask();
}

void ISERIALPORT_INS_D_COM1::recvTask()
{
    uint8_t _rec_data[10000];
    serial_frm_t _frm_tmp;


    while(true)
    {
        if(recv_start)
        {
            int _len = 0;
            {
                std::unique_lock<std::mutex> lck(m_mutex);
                _len = m_sp.ReadData(m_serial_fd,_rec_data,10000);//get data from serialport
            }

            if( _len > 0)
            {
                if(m_dev==ttyUSB0)
                {

                    for(int i = 0; i < _len; i++)
                        rev_buffer.push_back(_rec_data[i]);//put data into a buffer

                }


            }


    //        if(m_dev==ttyUSB0)
    //        {
    //            bool dataEnd=false;
    //            while(!dataEnd&&rev_buffer.size()>=137)
    //            {
    //                if((rev_buffer[0] == 0xAA) && (rev_buffer[1] == 0x55))
    //                {
    //                    vector<uint8_t> datas;
    //                    datas.reserve(137);
    //                    for(int i=0;i<137;i++)
    //                    {
    //                        datas.push_back(rev_buffer[0]);
    //                        rev_buffer.erase(rev_buffer.begin());
    //                    }
    //                    _frm_tmp.frm_src = Nav_Frm;
    //                    _frm_tmp.frm.time_stamp=QDateTime::currentMSecsSinceEpoch();
    //                    _frm_tmp.frm.datas=datas;
    //                    {
    //                        std::unique_lock<std::mutex> lck(m_mutex);
    //                        m_frms.push_back(_frm_tmp);
    //                    }

    //                }
    //                else
    //                {
    //                    rev_buffer.erase(rev_buffer.begin());
    //                }


    //            }
    //        }

            if(m_dev==ttyUSB0)
            {
                bool dataEnd=false;
                while(!dataEnd&&rev_buffer.size()>=5)
                {
                    if(rev_buffer[0] == '#')
                    {
                        vector<char> strings;
                        bool findEnd=false;
                        for(int i=0;i<rev_buffer.size();i++)
                        {
                            strings.push_back(rev_buffer[i]);
                            if(rev_buffer[i]=='*')
                            {
                                findEnd=true;
                                break;
                            }
                        }

                        if(findEnd)
                        {
                            rev_buffer.erase(rev_buffer.begin(),rev_buffer.begin()+strings.size());
                            vector<string> _serialStrings;
                            string _string;
                            for(int i=0;i<strings.size();i++)
                            {

                                if(strings[i]!=','&&strings[i]!=';'&&strings[i]!='*')
                                    _string.push_back(strings[i]);
                                else
                                {
                                    _serialStrings.push_back(_string);
                                    _string.clear();
                                }
                            }

                           setSerialStrings(_serialStrings);
                        }
                        else
                            break;



                    }
                    else
                    {
                        rev_buffer.erase(rev_buffer.begin());
                    }
                }
            }



            this_thread::sleep_for(chrono::milliseconds(1));
        }

    }



}




void ISERIALPORT_INS_D_COM1::sendThread(void *_p)
{
    ISERIALPORT_INS_D_COM1 *_this = (ISERIALPORT_INS_D_COM1*)_p;
    _this->sendTask();
}

void ISERIALPORT_INS_D_COM1::sendTask()
{
    while(send_start)
    {

        this_thread::sleep_for(chrono::milliseconds(5));
    }
}

void ISERIALPORT_INS_D_COM1::setSerialStrings(vector<string> _serialStrings)
{
    serialStrings.push_back(_serialStrings);
}


#endif
