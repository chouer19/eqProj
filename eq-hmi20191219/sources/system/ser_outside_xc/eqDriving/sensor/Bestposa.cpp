//======================================================================a      
/* ! \file Bestposa.cpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Bestposa 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o

#include <iostream>
#include <string>
#include "header/system/ser_outside_xc/eqDriving/sensor/BestposaPrivate.hpp"
#include "header/system/ser_outside_xc/eqDriving/sensor/Bestposa.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;

Bestposa::Bestposa():dataPtr(new BestposaPrivate){
    this->TimeStamp(0);
    this->Latitude(0);
    this->Longitude(0);
    this->PosType("NONE");
    this->SatelliteNumber(0);
    this->Altitude(0);
}

Bestposa::~Bestposa(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

Bestposa::Bestposa(const Bestposa &_value): dataPtr(new BestposaPrivate) {
    this->TimeStamp(_value.TimeStamp());
    this->Latitude(_value.Latitude());
    this->Longitude(_value.Longitude());
    this->PosType(_value.PosType());
    this->SatelliteNumber(_value.SatelliteNumber());
    this->Altitude(_value.Altitude());
}

////////////////////////////////////////////
// get methods
size_t Bestposa::TimeStamp() const{ return this->dataPtr->time_stamp_; }
double Bestposa::Latitude() const{ return this->dataPtr->latitude_; }
double Bestposa::Longitude() const{ return this->dataPtr->longitude_; }
string Bestposa::PosType() const{ return this->dataPtr->pos_type_; }
uint8_t Bestposa::SatelliteNumber() const{ return this->dataPtr->satellite_number_; }
double Bestposa::Altitude() const{ return this->dataPtr->altitude_; }

////////////////////////////////////////////
// set methods
void Bestposa::TimeStamp(const size_t &_value){ this->dataPtr->time_stamp_= _value; }
void Bestposa::Latitude(const double &_value){ this->dataPtr->latitude_ = _value; }
void Bestposa::Longitude(const double &_value){ this->dataPtr->longitude_ = _value; }
void Bestposa::PosType(const string &_value){ this->dataPtr->pos_type_ = _value;}
void Bestposa::SatelliteNumber(const uint8_t &_value){ this->dataPtr->satellite_number_ = _value; }
void Bestposa::Altitude(const double &_value){ this->dataPtr->altitude_ = _value; }

///////////////////////////////
Bestposa &Bestposa::operator=(const Bestposa &_value)
{
    this->dataPtr->time_stamp_ = _value.TimeStamp();
    this->dataPtr->latitude_  = _value.Latitude();
    this->dataPtr->longitude_  = _value.Longitude();
    this->dataPtr->altitude_ = _value.Altitude();
    this->dataPtr->pos_type_ = _value.PosType();
    this->dataPtr->satellite_number_  = _value.SatelliteNumber();
    return *this;

}

