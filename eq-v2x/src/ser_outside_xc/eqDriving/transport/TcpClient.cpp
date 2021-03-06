#include "ser_outside_xc/eqDriving/transport/TcpClient.hpp"
#include "ser_outside_xc/eqDriving/util/Logger.hpp"
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>

#include <deque>
#include <string>
#include <iostream>
using namespace std;

using namespace eqDriving;
using namespace transport;
using namespace util;

tcpClient::tcpClient(string url, string port)
    :socket_(io_service_)
    ,resolver_(io_service_)
    ,url_(url)
    ,port_(port)
{
}

tcpClient::~tcpClient(){
}

void tcpClient::check_counter(){
    while(true){
        counter_ = 0;
        wait_s(10);
        if(counter_ == 0){
            Connected(false);
            Logger::Instance()->Writeln("ERROR: received no any bytes in 10s, bad connecting status!");      
        }
    }
}

boost::system::error_code tcpClient::Write(const void *data, size_t size_in_bytes){
    boost::system::error_code ec;
    socket_.write_some(boost::asio::buffer(data, size_in_bytes),ec);
    if(ec){
        Logger::Instance()->Writeln("ERROR: write to "+url_+":"+port_+"failed!");      
        Logger::Instance()->Writeln(ec.message());
        cout <<"&&&&&&&&&&&\n" <<ec.message() << endl;}
    return ec;
}

vector<uint8_t> tcpClient::ExtractBytes(size_t size_in_bytes){
    boost::lock_guard<boost::mutex> lock{mutex_};
    size_in_bytes = size_in_bytes<0? bytes_pointer_ : size_in_bytes;
    size_in_bytes = size_in_bytes>bytes_pointer_? bytes_pointer_ : size_in_bytes;
    vector<uint8_t> bytes(read_bytes_, read_bytes_+size_in_bytes);
    memmove(&read_bytes_[0], &read_bytes_[size_in_bytes], size_in_bytes);
    bytes_pointer_ -= size_in_bytes;
    return bytes;
}

void tcpClient::Run(){
    if(Running()){return ;}
    running_ = true;
    auto thread = new boost::thread(
        boost::bind(&tcpClient::auto_connect_loop, this));
    auto thread_2 = new boost::thread(
        boost::bind(&tcpClient::check_counter, this));
    wait_ms(500);
// ***// assert thread is true
}

void tcpClient::Stop(){
    running_ = false;
}

bool tcpClient::Running(){
    return running_;
}

void tcpClient::read_loop(){
    while(running_){
        boost::system::error_code ec;
        int bytes_read = socket_.read_some(boost::asio::buffer(tmp_bytes_),ec);
        if(ec){
            Logger::Instance()->Writeln("ERROR: read from "+url_+":"+port_+"failed!");      
            Logger::Instance()->Writeln(ec.message());
            cout <<"&&&&&&&&&&&\n" <<ec.message() << endl;
            Connected(false);break;}
        cout << "*******\nread bytes num is : " << dec <<bytes_read << endl;
        add_read_bytes(tmp_bytes_, bytes_read);
        counter_ ++;
    }
}

void tcpClient::auto_connect_loop(){
    while(running_){
        async_resolve(); 
        async_connect();
        wait_s(1);
    }
    socket_.close();
    io_service_.stop();
}

void tcpClient::add_read_bytes(const uint8_t *data, size_t size_in_bytes){
    boost::lock_guard<boost::mutex> lock{mutex_};
    for(int outside = size_in_bytes + bytes_pointer_- 1024000;outside>0;){
        memmove(&read_bytes_[0], &read_bytes_[outside], outside);
        bytes_pointer_ -= outside;
        break;
    }
    memcpy(&read_bytes_[bytes_pointer_], data, size_in_bytes);
    bytes_pointer_ += size_in_bytes;
}

void tcpClient::async_resolve(){
    if(Resolved()){return ;}
    if(Resolving()){return ;} Resolving(true);
    boost::asio::ip::tcp::resolver::query query(url_,port_);
    resolver_.async_resolve(query, 
    [&](const boost::system::error_code& ec, 
       boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
        if (ec || endpoint_iterator == 
            boost::asio::ip::tcp::resolver::iterator()) {
            Resolved(false);
            Logger::Instance()->Writeln("ERROR: resolve address" +url_ + ":" + port_ +"failed!");      
            Logger::Instance()->Writeln(ec.message());
        }else{         
            endpoint_ = endpoint_iterator->endpoint();
            Resolved(true);
            Logger::Instance()->Writeln("SUCCESS: resolve address" +url_ + ":" + port_ +"successfully!");      
        }
        Resolving(false);
    }); io_service_.reset(); io_service_.run();
}

void tcpClient::async_connect(){
    if(Connected()){return ;}
    if(Connecting()){return ;} Connecting(true);
    socket_.close();
    socket_.async_connect(endpoint_,
       [&](const boost::system::error_code& ec){
        if (ec ){
            Connected(false);
            Logger::Instance()->Writeln("ERROR: connect address" +url_ + ":" + port_ +"failed!");      
            Logger::Instance()->Writeln(ec.message());
        }else{ 
            Connected(true);
            auto thread = new boost::thread(
                boost::bind(&tcpClient::read_loop, this));
            Logger::Instance()->Writeln("SUCCESS: connect address" +url_ + ":" + port_ +"successfully!");      
        }   
        Connecting(false);
    }); io_service_.reset(); io_service_.run();
}

bool tcpClient::Resolved(){
    boost::lock_guard<boost::mutex> lock{mutex_};
    return resolved_;
}
bool tcpClient::Resolved(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    resolved_ = _value;
    return resolved_;
}
bool tcpClient::Resolving(){
    boost::lock_guard<boost::mutex> lock{mutex_};
    return resolving_;
}
bool tcpClient::Resolving(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    resolving_ = _value;
    return resolving_;
}
bool tcpClient::Connected(){
    boost::lock_guard<boost::mutex> lock{mutex_};
    return connected_;
}
bool tcpClient::Connected(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    connected_ = _value;
    return connected_;
}
bool tcpClient::Connecting(){
    boost::lock_guard<boost::mutex> lock{mutex_};
    return connecting_;
}
bool tcpClient::Connecting(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    connecting_ = _value;
    return connecting_;
}

void tcpClient::wait_ms(unsigned int ms_count){
    boost::this_thread::sleep_for(boost::chrono::milliseconds{ms_count});

}
void tcpClient::wait_s(unsigned int s_count){
    boost::this_thread::sleep_for(boost::chrono::seconds{s_count});
}
