#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ser_outside_xc/eqDriving/util/Logger.hpp"

using namespace std;
using namespace eqDriving;
using namespace util;

Logger::Logger(): initialized(false), stop(true) {}
Logger::~Logger(){
    //this->ofs.close();
}

void Logger::Init(){
    {
        boost::lock_guard<boost::mutex> lock(write_mutex);
        if(this->initialized){return ;}
        this->initialized = true; this->stop = false;
    }
    this->filename = boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time()) + ".log";
}

void Logger::Write(std::string log) { 
    boost::lock_guard<boost::mutex> lock(write_mutex);
    if(!this->initialized){
        this->filename = boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time()) + ".log";
        this->initialized = true; this->stop = false;
    }
    this->ofs.open(this->filename, std::fstream::out | std::fstream::app);
    if(!this->ofs.is_open()){return ;}
    this->ofs << "[" << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) << "]";
    this->ofs << log ;
    this->ofs.close();
}
void Logger::Writeln(std::string log) { 
    boost::lock_guard<boost::mutex> lock(write_mutex);
    if(!this->initialized){
        this->filename = boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time()) + ".log";
        this->initialized = true; this->stop = false;
    }
    this->ofs.open(this->filename, std::fstream::out | std::fstream::app);
    if(!this->ofs.is_open()){return ;}
    this->ofs << "[" << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) << "]";
    this->ofs << log << "\n";
    this->ofs.close();
}
void Logger::Stop() { 
    this->stop = true;         
}

bool Logger::Running() const {
    return !this->stop;
}
