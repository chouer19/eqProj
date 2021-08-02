//======================================================================a      
/* ! \file Time.cpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Time 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o

#include <iostream>
#include <string>
#include "header/system/ser_outside_xc/eqDriving/sensor/TimePrivate.hpp"
#include "header/system/ser_outside_xc/eqDriving/sensor/Time.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;

Time::Time():dataPtr(new TimePrivate){
    this->TimeStamp(0);
}

Time::~Time(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

Time::Time(const Time &_value): dataPtr(new TimePrivate) {
    this->UtcTime(_value.UtcTime());
    this->TimeStamp(_value.TimeStamp());
}

////////////////////////////////////////////
// get methods
utc_time Time::UtcTime() const{ return this->dataPtr->utc_time_; }
size_t Time::TimeStamp() const{ return this->dataPtr->time_stamp_; }

////////////////////////////////////////////
// set methods
void Time::UtcTime(const utc_time &_value){ this->dataPtr->utc_time_ = _value; }
void Time::TimeStamp(const size_t &_value){ this->dataPtr->time_stamp_ = _value; }

///////////////////////////////
Time &Time::operator=(const Time &_value)
{
    this->dataPtr->utc_time_  = _value.UtcTime();
    this->dataPtr->time_stamp_ = _value.TimeStamp();
    return *this;

}

