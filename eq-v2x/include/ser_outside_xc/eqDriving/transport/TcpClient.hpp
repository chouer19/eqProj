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

namespace eqDriving{
namespace transport{
class tcpClient{
private:
    boost::mutex mutex_;
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::endpoint endpoint_;
    string url_ = "127.0.0.1";
    string port_ = "8080";
    bool running_ = false;
    bool resolved_ = false;
    bool resolving_ = false;
    bool connected_ = false;
    bool connecting_ = false;
    unsigned int auto_connect_cycle_ = 2000;//ms
    uint8_t read_bytes_[1024000];
    uint8_t tmp_bytes_[1024];
    size_t bytes_pointer_ = 0;
    size_t counter_ = 0;

public: 
    tcpClient(string url, string port);
    ~tcpClient();
    boost::system::error_code Write(const void *data, size_t size_in_bytes);
    vector<uint8_t> ExtractBytes(size_t size_in_bytes = -1);
    void Run();
    void Stop();
    bool Running();

    void wait_ms(unsigned int ms_count);
    void wait_s(unsigned int s_count);

private:
    void read_loop();
    void auto_connect_loop();
    void async_resolve();
    void async_connect();
    bool Resolved();
    bool Resolved(const bool &_value);
    bool Resolving();
    bool Resolving(const bool &_value);
    bool Connected();
    bool Connected(const bool &_value);
    bool Connecting();
    bool Connecting(const bool &_value);
    void add_read_bytes(const uint8_t *data, size_t size_in_bytes);
    void check_counter();

};
}
}
