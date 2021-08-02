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
boost::asio::ip::tcp::endpoint endpoint_;
std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
std::shared_ptr<boost::asio::ip::tcp::resolver> resolver_;
std::shared_ptr<SimpleSerial> serial_;

bool resolved_ = false;
bool connected_ = false;
bool resolving_ = false;
bool connecting_ = false;

bool Resolved();
void Resolved(const bool &_value);
bool Connected();
void Connected(const bool &_value);
bool Resolving();
void Resolving(const bool &_value);
bool Connecting();
void Connecting(const bool &_value);
void start_resolve();
void start_connect();

void login_thread();
void rap_thread();
void respond_thread();
void readSerial();
void readQX();
void run_thread();

int main(int argc, char* argv[])
{
    Logger::Instance()->Init();
    socket_ = std::make_shared<boost::asio::ip::tcp::socket>(io_service_);
    resolver_ = std::make_shared<boost::asio::ip::tcp::resolver>(io_service_);
    serial_ = std::make_shared<SimpleSerial>(argv[1], atoi(argv[2]));

    start_resolve();
    start_connect();

    SingletonClient *s = s->getInstance();
    boost::thread t1(login_thread);
    boost::thread t2(&SingletonClient::write_thread, s);
    boost::thread t3(&SingletonClient::respond_thread, s);
    boost::thread t4(&SingletonClient::read_thread, s);
    boost::thread t5(rap_thread);

    boost::thread t8(run_thread);

    boost::thread t10(readSerial);
    boost::thread t11(readQX);
    t10.join();
    t11.join();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t8.join();

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
        //std::string rtcm_request = "GET /RTCM32_GGB HTTP/1.1\r\nHost: rtk.ntrip.qxwz.com\r\nNtrip-Version: Ntrip/2.0\r\nUser-Agent: NTRIP NtripClientPOSIX/$Revision: 1.51 $\r\nConnection: close\r\nAuthorization: Basic Z2NncnJlZTAwMzpmNDJjNjhh\r\n\r\n";
        //std::string rtcm_request = "GET /RTCM32_GGB HTTP/1.1\r\nHost: rtk.ntrip.qxwz.com\r\nNtrip-Version: Ntrip/2.0\r\nUser-Agent: NTRIP NtripClientPOSIX/$Revision: 1.51 $\r\nConnection: close\r\nAuthorization: Basic cXhlaGlyMDAyOjI3OTgxZTM=\r\n\r\n";
        std::string rtcm_request = "GET /RTCM32_GGB HTTP/1.1\r\nHost: rtk.ntrip.qxwz.com\r\nNtrip-Version: Ntrip/2.0\r\nUser-Agent: NTRIP NtripClientPOSIX/$Revision: 1.51 $\r\nConnection: close\r\nAuthorization: Basic Z2NncnJlZTAwMzpmNDJjNjhh\r\n\r\n";
    while(true){
        gpgga_read = serial_->readLine();
        cout << gpgga_read << endl;
        std::string str = rtcm_request + gpgga_read + "\r\n";
        try{
            // write when socket is open
            boost::system::error_code write_ec;
            boost::asio::write(*socket_, boost::asio::buffer(str), write_ec);
            if(write_ec){
                start_resolve();
                start_connect();
                boost::this_thread::sleep_for(boost::chrono::milliseconds{500});
                Logger::Instance()->Writeln("ERROR: write from rtk.ntrip.qxwz.com:8002 failed");
                Logger::Instance()->Writeln(write_ec.message());
                continue;
            }
        }catch(exception error){
            boost::this_thread::sleep_for(boost::chrono::milliseconds{500});
            Logger::Instance()->Writeln("ERROR: write from rtk.ntrip.qxwz.com:8002 failed");
            Logger::Instance()->Writeln(error.what());
            continue;
        }
        cout << str << endl;
    }
}

void readQX(){
    uint8_t read_buf[10240];
    boost::system::error_code read_ec;
    SingletonClient *s = s->getInstance();
    
    while(true){
        int bytes_read = 0;
        {
            bytes_read = socket_->read_some(boost::asio::buffer(read_buf), read_ec);
        }
        if(read_ec){
            start_resolve();
            start_connect();
            boost::this_thread::sleep_for(boost::chrono::milliseconds{500});
            Logger::Instance()->Writeln("ERROR: read from rtk.ntrip.qxwz.com:8002 failed");
            Logger::Instance()->Writeln(read_ec.message());
            cout << "ERROR: read from rtk.ntrip.qxwz.com:8002 failed" << endl;
            cout << "connection error\n";
            continue;
        }
        cout << "bytes_read is : " << dec << bytes_read << endl;
        Logger::Instance()->Writeln("read from QX, bytes: " + to_string(bytes_read));
        cout << "read content is :\n";  
        for(int i=0; i<bytes_read; i++){                                                          
            cout << setw(2) << setfill('0') << hex << (0xff & read_buf[i]);
            cout << " ";                
        } 
        serial_->writeBytes(read_buf, bytes_read);
        s->report_rtcm(read_buf, bytes_read);
    }
}

void Connecting(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mut}; 
    connecting_ = _value;
}
bool Connecting(){
    boost::lock_guard<boost::mutex> lock{mut}; 
    return connecting_;
}
void Resolving(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mut}; 
    resolving_ = _value;
}
bool Resolving(){
    boost::lock_guard<boost::mutex> lock{mut}; 
    return resolving_;
}
void Connected(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mut}; 
    connected_ = _value;
}
bool Connected(){
    boost::lock_guard<boost::mutex> lock{mut}; 
    return connected_;
}
void Resolved(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mut}; 
    resolved_ = _value;
}
bool Resolved(){
    boost::lock_guard<boost::mutex> lock{mut}; 
    return resolved_;
}

void start_resolve(){
    if(Resolved()){return ;}
    if(Resolving()){return ;}
    Resolving(true);
    boost::asio::ip::tcp::resolver::query query("rtk.ntrip.qxwz.com","8002");
    resolver_->async_resolve(query, 
       [&](const boost::system::error_code& inec, 
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
        if (inec || endpoint_iterator == 
            boost::asio::ip::tcp::resolver::iterator()) {
            cout << "resolve address failed!\n";
            Resolving(false);
            Logger::Instance()->Writeln("ERROR: resolve address failed!");
            Logger::Instance()->Writeln(inec.message());
        }else{         
            cout << "resolve address susscessfully!\n";
            endpoint_ = endpoint_iterator->endpoint();
            Resolved(true);
            Logger::Instance()->Writeln("SUCCESS: resolve address successfully!");
            Logger::Instance()->Writeln(inec.message());                                  }              
        Resolving(false);
    });                
}

void start_connect(){
    if(Connected()){
        //cout << "no need to connect!\n";
        return ;}
    if(Connecting()){
        //cout << "connecting......\n";
        return ;}
    Connecting(true);
    socket_->close();
    socket_->async_connect(endpoint_,
       [&](const boost::system::error_code& inec){
        if (inec ){
            cout << "connect qxwz failed!\n";
            cout << inec.message() << endl;
            Logger::Instance()->Writeln("ERROR: connect rtk.ntrip.qxwz.com:8002 failed");
            Logger::Instance()->Writeln(inec.message());
            Connected(false);
        }else{ 
            cout << "connect qxwz susscessfully!\n";
            Logger::Instance()->Writeln("SUCCESS: connect rtk.ntrip.qxwz.com:8002 succesfully");
            Logger::Instance()->Writeln(inec.message());
            Connected(true);
        }   
        Connecting(false);
    });
}

void run_thread(){
    while(true){
        io_service_.reset();
        io_service_.run();
    }
}

