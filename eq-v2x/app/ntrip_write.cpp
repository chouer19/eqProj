#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/shared_ptr.hpp>
#include "ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp"
#include "ser_outside_xc/eqDriving/serial/SimpleSerial.hpp"
#include "ser_outside_xc/eqDriving/util/Logger.hpp"
 
static volatile int running = 1;
    
void sigterm(int signo)
{   
    running = 0;
}  

using namespace std;
using namespace boost::asio;
using namespace eqDriving;
using namespace util;
using namespace io;

boost::mutex mut;
SingletonClient *SingletonClient::instance = 0;
boost::asio::io_service io_service_;
boost::asio::ip::tcp::resolver::iterator endpoint_iterator ;

void login_thread();
void rap_thread();
void write_rtcm(shared_ptr<SimpleSerial> serial);
void read_serial(shared_ptr<SimpleSerial> serial);

int main(int argc, char* argv[])
{
    Logger::Instance()->Init();

    std::string gpgga = "dfdfdsa";
//rtcm

    SingletonClient *s = s->getInstance();
    boost::thread t1(login_thread);
    boost::thread t2(&SingletonClient::write_thread, s);
    boost::thread t3(&SingletonClient::run_thread, s);
    boost::thread t4(&SingletonClient::read_thread, s);
    boost::thread t5(rap_thread);
    shared_ptr<SimpleSerial> serial(new SimpleSerial(argv[1], atoi(argv[2])));
    boost::thread t6(write_rtcm, serial);
    boost::thread t7(read_serial, serial);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();

    return 1;
}
void login_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
        if(s->jianquan_valid()){continue;}
        s->test_login();
    }          
}

void rap_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->rap();
        boost::this_thread::sleep_for(boost::chrono::seconds{5});
    }          
}

void read_serial(shared_ptr<SimpleSerial> serial){
    while(true){
        cout << serial->readLine() << endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
    } 
}

void write_rtcm(){
    SimpleSerial serial("/dev/ttyS0", 115200);
    SingletonClient *s = s->getInstance();
    while(true){
        rtcm_packet_t rtcm = s->rtcm();
        if(rtcm.is_new){
            cout << "writing rtcm to serial port ......\n";
            serial.writeBytes(rtcm.data);
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
    } 
}

void write_rtcm(shared_ptr<SimpleSerial> serial){
    //shared_ptr<SimpleSerial> serial(new SimpleSerial(argv[1], atoi(argv[2])));
    SingletonClient *s = s->getInstance();
    while(true){
        rtcm_packet_t rtcm = s->rtcm();
        if(rtcm.is_new){
            cout << "writing rtcm to serial port ......\n";
            serial->writeBytes(rtcm.data);
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
    } 
}
