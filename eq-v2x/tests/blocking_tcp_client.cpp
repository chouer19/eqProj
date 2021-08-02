//
// blocking_tcp_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;
using namespace std;

class client
{
public:
    static client *getInstance() {                                    
        if (instance == 0){  
             instance = new client;
        }                    
        return instance;     
    } 

  void connect(const std::string& host, const std::string& service,
      boost::posix_time::time_duration timeout)
  {
    tcp::resolver::query query(host, service);
    tcp::resolver::iterator iter = tcp::resolver(io_service_).resolve(query);
     boost::system::error_code ecc;
     boost::asio::ip::address ip_address =
       //boost::asio::ip::address::from_string("192.168.1.201", ecc);
       boost::asio::ip::address::from_string("qas.was.eqfleetcmder.com", ecc);
     boost::asio::ip::tcp::endpoint m_ep(ip_address, 8001);
    deadline_.expires_from_now(timeout);

    boost::system::error_code ec = boost::asio::error::would_block;

    //boost::asio::async_connect(socket_, iter, var(ec) = _1);
    //boost::asio::async_connect(*m_sock_ptr, iter, var(ec) = _1);
    boost::asio::async_connect(*m_sock_ptr, iter, boost::lambda::var(ec) = boost::lambda::_1);
    //boost::asio::async_connect(socket_, m_ep, var(ec) = _1);

    do {io_service_.run_one(); cout << ec.message() << endl;} while (ec == boost::asio::error::would_block);

    return ;
    if (ec || !socket_.is_open())
      throw boost::system::system_error(
          ec ? ec : boost::asio::error::operation_aborted);
  }

  std::string read_line(boost::posix_time::time_duration timeout)
  {
    deadline_.expires_from_now(timeout);

    boost::system::error_code ec = boost::asio::error::would_block;

    boost::asio::async_read_until(socket_, input_buffer_, '\n', var(ec) = _1);

    do io_service_.run_one(); while (ec == boost::asio::error::would_block);

    if (ec)
      throw boost::system::system_error(ec);

    std::string line;
    std::istream is(&input_buffer_);
    std::getline(is, line);
    return line;
  }

  void write_line(const std::string& line,
      boost::posix_time::time_duration timeout)
  {
    std::string data = line + "\n";

    deadline_.expires_from_now(timeout);

    boost::system::error_code ec = boost::asio::error::would_block;

    boost::asio::async_write(socket_, boost::asio::buffer(data), var(ec) = _1);

    do io_service_.run_one(); while (ec == boost::asio::error::would_block);

    if (ec)
      throw boost::system::system_error(ec);
  }

private:
  client()
    : socket_(io_service_),
      deadline_(io_service_)
  {
    deadline_.expires_at(boost::posix_time::pos_infin);
    check_deadline();
    m_sock_ptr = boost::make_shared<boost::asio::ip::tcp::socket>(io_service_);
  }
  void check_deadline()
  {
    if (deadline_.expires_at() <= deadline_timer::traits_type::now())
    {
      boost::system::error_code ignored_ec;
      //socket_.close();
      m_sock_ptr->close();

      deadline_.expires_at(boost::posix_time::pos_infin);
    }

    deadline_.async_wait(bind(&client::check_deadline, this));
  }

  boost::asio::io_service io_service_;
  tcp::socket socket_;
  deadline_timer deadline_;
  boost::asio::streambuf input_buffer_;
  boost::shared_ptr<tcp::socket> m_sock_ptr;
  static client *instance;
};

//----------------------------------------------------------------------
client *client::instance = 0;

int main(int argc, char* argv[])
{
   client *s = s->getInstance();
  try
  {
    if (argc != 4)
    {
      std::cerr << "Usage: blocking_tcp <host> <port> <message>\n";
      return 1;
    }

    //client c;
    //c.connect(argv[1], argv[2], boost::posix_time::seconds(10));
    while(true){
    cout << "connecting ....... !\n";
    s->connect(argv[1], argv[2], boost::posix_time::seconds(6));
    cout << "connected !\n\n";
    }

    boost::posix_time::ptime time_sent =
      boost::posix_time::microsec_clock::universal_time();

    s->write_line(argv[3], boost::posix_time::seconds(10));

    for (;;)
    {
      std::string line = s->read_line(boost::posix_time::seconds(10));

      // Keep going until we get back the line that was sent.
      if (line == argv[3])
        break;
    }

    boost::posix_time::ptime time_received =
      boost::posix_time::microsec_clock::universal_time();

    std::cout << "Round trip time: ";
    std::cout << (time_received - time_sent).total_microseconds();
    std::cout << " microseconds\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
