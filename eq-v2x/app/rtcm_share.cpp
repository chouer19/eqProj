#include <iostream>
#include <istream>
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/shared_ptr.hpp>
#include "ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp"
#include "ser_outside_xc/eqDriving/transport/TcpClient.hpp"
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
using namespace transport;

boost::mutex mutex_;
SingletonClient *SingletonClient::instance = 0;
boost::asio::io_service io_service_;

std::shared_ptr<SimpleSerial> serial_;
std::shared_ptr<tcpClient> qxClient_;

void login_thread();
void rap_thread();
void respond_thread();
void readSerial();
void readQX();
void writeQX();

string gpgga_ = "$GPGGA,062530.00,3932.1120000,N,11356.5668000,E,1,00,1.0,3.428,M,-3.428,M,0.0,*58";
string GPGGA(){
    boost::lock_guard<boost::mutex> lock{mutex_};
    return gpgga_;
}
void GPGGA(string value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    gpgga_ = value;
}

int main(int argc, char* argv[])
{
    Logger::Instance()->Init();
    //serial_ = std::make_shared<SimpleSerial>(argv[1], atoi(argv[2]));

    qxClient_ = std::make_shared<tcpClient>("rtk.ntrip.qxwz.com","8002");

    SingletonClient *s = s->getInstance();
    boost::thread t1(login_thread);
    boost::thread t2(&SingletonClient::write_thread, s);
    boost::thread t3(&SingletonClient::run_thread, s);
    boost::thread t4(&SingletonClient::read_thread, s);
    boost::thread t5(rap_thread);

    //boost::thread t10(readSerial);
    boost::thread t11(readQX);
    boost::thread t12(writeQX);

    qxClient_->Run();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    //t10.join();
    t11.join();
    t12.join();

    return 1;
}
void login_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
        if(s->jianquan_valid()){continue;}
        s->login();
    }          
}

void rap_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->rap();
        boost::this_thread::sleep_for(boost::chrono::seconds{5});
    }          
}

void respond_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->respond_ontime();
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
    } 
} 

void readSerial(){
        string gpgga_read;
    while(true){
        //gpgga_read = serial_->readLine();
        //cout << "************\n" <<gpgga_read << endl;
        GPGGA(gpgga_read);
    }
}

void writeQX(){
    std::string rtcm_request = "GET /RTCM32_GGB HTTP/1.1\r\nHost: rtk.ntrip.qxwz.com\r\nNtrip-Version: Ntrip/2.0\r\nUser-Agent: NTRIP NtripClientPOSIX/$Revision: 1.51 $\r\nConnection: close\r\nAuthorization: Basic Z2NncnJlZTAwMzpmNDJjNjhh\r\n\r\n";
    while(true){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{1000});
        string request = rtcm_request + GPGGA() + "\r\n";
        qxClient_->Write(request.c_str(), request.size());
        cout << "**********\n" <<request << endl;
    }
}

void readQX(){
    SingletonClient *s = s->getInstance();
    while(true){
        vector<uint8_t> readbytes = qxClient_->ExtractBytes();
        if(readbytes.size() == 0){continue;}
        uint8_t *read_buf = new uint8_t[readbytes.size()];
        memcpy(read_buf, readbytes.data(), readbytes.size());
        cout << "\n*************\nextract bytes : " << readbytes.size() << endl;
        for(int i=0; i<readbytes.size() && i < 50; i++){
            cout << setw(2) << setfill('0') << hex << (0xff & read_buf[i]);
            cout << " ";                
        } 
        cout << endl;
        //serial_->writeBytes(read_buf, readbytes.size());
        s->report_rtcm(read_buf, readbytes.size());
    }
}
