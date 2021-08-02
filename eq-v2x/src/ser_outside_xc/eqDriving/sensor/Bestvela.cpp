//======================================================================a      
/* ! \file Bestvela.cpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Bestvela 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o

#include <iostream>
#include <string>
#include "ser_outside_xc/eqDriving/sensor/BestvelaPrivate.hpp"
#include "ser_outside_xc/eqDriving/sensor/Bestvela.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;

Bestvela::Bestvela():dataPtr(new BestvelaPrivate){
    this->TimeStamp(0);
    this->VertSpeed(0);
    this->Speed(0);
}

Bestvela::~Bestvela(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

Bestvela::Bestvela(const Bestvela &_value): dataPtr(new BestvelaPrivate) {
    this->TimeStamp(_value.TimeStamp());
    this->VertSpeed(_value.VertSpeed());
    this->Speed(_value.Speed());
}

////////////////////////////////////////////
// get methods
size_t Bestvela::TimeStamp() const{ return this->dataPtr->time_stamp_; }
double Bestvela::VertSpeed() const{ return this->dataPtr->vert_speed_; }
double Bestvela::Speed() const{ return this->dataPtr->speed_; }

////////////////////////////////////////////
// set methods
void Bestvela::TimeStamp(const size_t &_value){ this->dataPtr->time_stamp_ = _value; }
void Bestvela::VertSpeed(const double &_value){ this->dataPtr->vert_speed_ = _value; }
void Bestvela::Speed(const double &_value){ this->dataPtr->speed_ = _value; }

///////////////////////////////
Bestvela &Bestvela::operator=(const Bestvela &_value)
{
    this->dataPtr->time_stamp_ = _value.TimeStamp();
    this->dataPtr->vert_speed_ = _value.VertSpeed();
    this->dataPtr->speed_ = _value.Speed();
    return *this;

}

