/* 
 * File:   SimpleSerial.h
 * Author: xuechong
 *
 * Created on September 28, 2019
 */


#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "header/system/ser_outside_xc/eqDriving/util/Logger.hpp"

using namespace std;
using namespace eqDriving;
using namespace util;

namespace eqDriving{
    namespace io{
        class SimpleSerial
        {
        public:
            /**
             * Constructor.
             * \param port device name, example "/dev/ttyUSB0" or "COM4"
             * \param baud_rate communication speed, example 9600 or 115200
             * \throws boost::system::system_error if cannot open the
             * serial device
             */
            SimpleSerial():io(), serial(io){ }
            //SimpleSerial(std::string port, unsigned int baud_rate)
            SimpleSerial(std::string port, int baud_rate)
            //: io(), serial(io,port)
            : io(), serial(io)
            {
                Logger::Instance()->Init();
                boost::system::error_code ec;
                try{
                    serial.open(port,ec);
                    if(ec){
                        Logger::Instance()->Writeln("ERROR: when open serial port!");
                        Logger::Instance()->Writeln("\t" + port + to_string(baud_rate));
                        Logger::Instance()->Writeln(ec.message());
                    }else{
                        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
                        Logger::Instance()->Writeln("Opened serial port " + port + to_string(baud_rate) + "successfully!");
                    }
                }catch(std::exception eee){
                    Logger::Instance()->Writeln("ERROR: when open serial port!");
                    Logger::Instance()->Writeln("\t" + port + to_string(baud_rate));
                    Logger::Instance()->Writeln(eee.what());
                }
            }

            bool open(std::string port, unsigned int baud_rate){
                boost::system::error_code ec;
                try{
                    serial.open(port,ec);
                    if(ec){
                        Logger::Instance()->Writeln("ERROR: when open serial port!");
                        Logger::Instance()->Writeln("\t" + port + to_string(baud_rate));
                        Logger::Instance()->Writeln(ec.message());
                    return false;} 
                    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));return true;
                }catch(std::exception eee){
                    Logger::Instance()->Writeln("ERROR: when open serial port!");
                    Logger::Instance()->Writeln("\t" + port + to_string(baud_rate));
                    Logger::Instance()->Writeln(eee.what());
                return false;}
            }
        
            /**
             * Write a string to the serial device.
             * \param s string to write
             * \throws boost::system::system_error on failure
             */
            void writeString(std::string s)
            {
                boost::system::error_code ec;
                boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()), ec);
                if(ec){
                    Logger::Instance()->Writeln("ERROR: when writeString(<vector>) from serial port");
                    Logger::Instance()->Writeln(ec.message());
                }
            }
        
            void writeBytes(const uint8_t *data, int length){
                boost::system::error_code ec;
                boost::asio::write(serial,boost::asio::buffer(data, length), ec);
                if(ec){
                    Logger::Instance()->Writeln("ERROR: when writeBytes(const uint8_t *, int) from serial port");
                    Logger::Instance()->Writeln(ec.message());
                }
            }
            void writeBytes(vector<uint8_t> data){
                boost::system::error_code ec;
                boost::asio::write(serial,boost::asio::buffer(data.data(), data.size()), ec);
                if(ec){
                    Logger::Instance()->Writeln("ERROR: when writeBytes(<vector>) from serial port");
                    Logger::Instance()->Writeln(ec.message());
                }
            }
        
            /**
             * Blocks until a line is received from the serial device.
             * Eventual '\n' or '\r\n' characters at the end of the string are removed.
             * \return a string containing the received line
             * \throws boost::system::system_error on failure
             */
            std::string readLine()
            {
                //Reading data char by char, code is optimized for simplicity, not speed
                boost::system::error_code ec;
                using namespace boost;
                char c;
                std::string result;
                for(;;)
                {
                    asio::read(serial,asio::buffer(&c,1), ec);
                    if(ec){
                        Logger::Instance()->Writeln("ERROR: when readLine() from serial port");
                        //Logger::Instance()->Writeln(ec.message());
                        return result;
                    }
                    switch(c)
                    {
                        case '\r':
                            break;
                        case '\n':
                            return result;
                        default:
                            result+=c;
                    }
                }
            }
        
        private:
            boost::asio::io_service io;
            boost::asio::serial_port serial;
        };
    }
}
