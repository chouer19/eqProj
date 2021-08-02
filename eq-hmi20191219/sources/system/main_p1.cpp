#include "header/system/pub_globals.h"
#include "header/system/sys_abstract.hpp"
#include "header/drive/dri_abstract.hpp"
#include "header/system/main_linux.h"
#include "controlcan.h"
#include <unistd.h>
#include <thread>




static void parseRS232(ISERIALPORT_INS_D_COM1 *_rs232)
{
    vector<serial_frm_t> data;
    data.clear();
    data = _rs232->getFrms();
    _rs232->clearFrms();

    for(size_t i = 0; i < data.size(); i++)
    {
        switch(data[i].frm_src)
        {
            case Nav_Frm://Nav
            {
                protocol->dgps->parse(&data[i]);
                break;
            }
        }
    }

}

static bool s_can_rev_start = false;


static bool s_com_rev_start = false;
static void com_rev_work()
{
    while(s_com_rev_start)
    {
        parseRS232((ISERIALPORT_INS_D_COM1*)drive->rs232_ins_d_com1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}



bool main_p1_start=false;
int main_p1(void)
{
    //ThreadPool m_thread_pool_p1(17);
//    s_can_rev_start = true;
//    s_com_rev_start = true;
    std::thread thread_recv_rs232(com_rev_work);

//    protocol->dgps->install();
//    protocol->dgps->start();
//    protocol->vehicle->install();
//    protocol->vehicle->start();
    protocol->gnss_receiver->install();
    protocol->gnss_receiver->start();

    while (main_p1_start)
    {
        sys->taskList_p[1]->exec();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    s_can_rev_start = false;

    s_com_rev_start = false;
    thread_recv_rs232.join();

}
