//======================================================================
/* ! \file eqSingletonSingletonClient.hpp

 *
 * \copydoc Copyright
 * \author Xue Chong(xc)
 * \date July 29, 2019
 *
 * a singleton class for SingletonClient
 * read is synchronous, keep listening
 * write is asynchronous
 * Usage:  
 * SingletonClient for connectint cloud, send and receive message
 *///-------------------------------------------------------------------
                                               
#include <stdint.h>                                                
#include <string.h> 
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <memory>
#include <deque>
#include <boost/asio/connect.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/write.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

//#include "ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
//#include "ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"
//#include "pub_types.hpp"

using namespace std;
using namespace boost::asio;
using namespace std::chrono;
using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using namespace std;

class SingletonClient
{
public:
    static SingletonClient *getInstance() {                                    
        if (instance == 0){  
             instance = new SingletonClient;
        }                    
        return instance;     
    } 

  void start_resolve(){
    boost::asio::ip::tcp::resolver::query 
              query("qas.was.eqfleetcmder.com", "28001");
    //boost::system::error_code ec = boost::asio::error::would_block;
    boost::system::error_code ec = boost::asio::error::would_block;
    //boost::asio::async_connect(*m_sock_ptr, iter, var(ec) = _1);
    //deadline_.expires_from_now(boost::posix_time::seconds(6));
    //deadline_.expires_from_now(boost::posix_time::seconds(2));
    //resolver.async_resolve(query, boost::lambda::var(ec) = boost::lambda::_1);
    resolver.async_resolve(query, 
       [&](const boost::system::error_code& inec, 
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
        ec = inec;
        if (inec || endpoint_iterator == 
            boost::asio::ip::tcp::resolver::iterator()) {
            cout << "resolve address failed!\n";
        }else{ 
            cout << "resolve address susscessfully!\n";
            m_ep = endpoint_iterator->endpoint();
        }  
    }); 
    deadline_.expires_from_now(boost::posix_time::seconds(2));
    check_deadline();
    do {m_io.run_one(); cout << ec.message() << endl;} while (ec == boost::asio::error::would_block);
    //m_io.run_one();
  }

  void start_connect(const std::string& host, const std::string& service,
      boost::posix_time::time_duration timeout)
  {
    tcp::resolver::query query(host, service);
    //tcp::resolver::query query("qas.was.eqfleetcmder.com", "28002");
    tcp::resolver::iterator iter = tcp::resolver(m_io).resolve(query);
     boost::system::error_code ecc;
     boost::asio::ip::address ip_address =
       //boost::asio::ip::address::from_string("192.168.1.201", ecc);
       boost::asio::ip::address::from_string("qas.was.eqfleetcmder.com", ecc);
     boost::asio::ip::tcp::endpoint m_ep(ip_address, 28001);
    deadline_.expires_from_now(timeout);
    boost::system::error_code ec = boost::asio::error::would_block;
    //boost::asio::async_connect(*m_sock_ptr, iter, var(ec) = _1);
    boost::asio::async_connect(*m_sock_ptr, iter, boost::lambda::var(ec) = boost::lambda::_1);
    do {m_io.run_one(); cout << ec.message() << endl;} while (ec == boost::asio::error::would_block);
    if(!ec){cout << "=======================\n";}
    else{cout << ec.message() << endl;}
    return ;
    if (ec || !m_sock_ptr->is_open())
      throw boost::system::system_error(
          ec ? ec : boost::asio::error::operation_aborted);
  }


private:
  SingletonClient()
    //: socket_(m_io),
      :deadline_(m_io), resolver(m_io)
  {
    deadline_.expires_at(boost::posix_time::pos_infin);
    check_deadline();
    m_sock_ptr = boost::make_shared<boost::asio::ip::tcp::socket>(m_io);
    //boost::asio::ip::tcp::resolver resolver(this->m_io);
  }
  void check_deadline()
  {
    cout << "ddddddddddddddddddd\n";
    if (deadline_.expires_at() <= deadline_timer::traits_type::now())
    {
      boost::system::error_code ignored_ec;
      //socket_.close();
      m_sock_ptr->close();
      resolver.cancel();
      deadline_.expires_at(boost::posix_time::pos_infin);
    }

    deadline_.async_wait(bind(&SingletonClient::check_deadline, this));
  }

  boost::asio::io_service m_io;
  deadline_timer deadline_;
  boost::asio::ip::tcp::resolver resolver;
  boost::asio::ip::tcp::endpoint m_ep;
  boost::shared_ptr<tcp::socket> m_sock_ptr;
  static SingletonClient *instance;
};

//----------------------------------------------------------------------
SingletonClient *SingletonClient::instance = 0;

int main(int argc, char* argv[])
{
   SingletonClient *s = s->getInstance();
  try
  {
    //if (argc != 4)
    //{
    //  std::cerr << "Usage: blocking_tcp <host> <port> <message>\n";
    //  return 1;
    //}

    //SingletonClient c;
    //c.connect(argv[1], argv[2], boost::posix_time::seconds(10));
    while(true){
        cout << "connecting ....... !\n";
        s->start_connect(argv[1], argv[2], boost::posix_time::seconds(6));
        //s->start_connect("dafd", "dfad", boost::posix_time::seconds(3));
        //s->start_resolve();
        cout << "connected !\n\n";
    }
  }catch(exception eee){}

  return 0;
}

