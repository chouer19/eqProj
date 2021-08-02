//======================================================================a      
/* ! \file Heading2a.cpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Heading2a 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o

#include <iostream>
#include <string>
#include "ser_outside_xc/eqDriving/sensor/Heading2aPrivate.hpp"
#include "ser_outside_xc/eqDriving/sensor/Heading2a.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;

Heading2a::Heading2a():dataPtr(new Heading2aPrivate){
    this->TimeStamp(0);
    this->Azimuth(0);
    this->Pitch(0);
}

Heading2a::~Heading2a(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

Heading2a::Heading2a(const Heading2a &_value): dataPtr(new Heading2aPrivate) {
    this->TimeStamp(_value.TimeStamp());
    this->Azimuth(_value.Azimuth());
    this->Pitch(_value.Pitch());
}

////////////////////////////////////////////
// get methods
size_t Heading2a::TimeStamp() const{ return this->dataPtr->time_stamp_; }
double Heading2a::Azimuth() const{ return this->dataPtr->azimuth_; }
double Heading2a::Pitch() const{ return this->dataPtr->pitch_; }

////////////////////////////////////////////
// set methods
void Heading2a::TimeStamp(const size_t &_value){ this->dataPtr->time_stamp_ = _value; }
void Heading2a::Azimuth(const double &_value){ this->dataPtr->azimuth_ = _value; }
void Heading2a::Pitch(const double &_value){ this->dataPtr->pitch_ = _value; }

///////////////////////////////
Heading2a &Heading2a::operator=(const Heading2a &_value)
{
    this->dataPtr->time_stamp_ = _value.TimeStamp();
    this->dataPtr->azimuth_  = _value.Azimuth();
    this->dataPtr->pitch_ = _value.Pitch();
    return *this;
}

