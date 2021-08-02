//======================================================================a      
/* ! \file Nmea.cpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Nmea 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o

#include <iostream>
#include <string>
#include "ser_outside_xc/eqDriving/sensor/NmeaPrivate.hpp"
#include "ser_outside_xc/eqDriving/sensor/Nmea.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;

Nmea::Nmea():dataPtr(new NmeaPrivate){
    //this->UtcTime();
    this->Latitude(0);
    this->LatitudeDirection("O");
    this->Longitude(0);
    this->LongitudeDirection("O");
    this->FixQuality(0);
    this->PosType("NONE");
    this->SatelliteNumber(0);
    this->Altitude(0);
    this->Azimuth(0);
    this->Pitch(0);
    this->Speed(0);
}

Nmea::~Nmea(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

Nmea::Nmea(const Nmea &_value): dataPtr(new NmeaPrivate) {
    this->UtcTime(_value.UtcTime());
    this->UtcOffset(_value.UtcOffset());
    this->Latitude(_value.Latitude());
    this->LatitudeDirection(_value.LatitudeDirection());
    this->Longitude(_value.Longitude());
    this->LongitudeDirection(_value.LongitudeDirection());
    this->FixQuality(_value.FixQuality());
    this->PosType(_value.PosType());
    this->SatelliteNumber(_value.SatelliteNumber());
    this->Altitude(_value.Altitude());
    this->Azimuth(_value.Azimuth());
    this->Pitch(_value.Pitch());
    this->Speed(_value.Speed());
}

////////////////////////////////////////////
// get methods
utc_time Nmea::UtcTime() const{ return this->dataPtr->utc_time_; }
double Nmea::UtcOffset() const{ return this->dataPtr->utc_offset_; }
double Nmea::Latitude() const{ return this->dataPtr->latitude_; }
string Nmea::LatitudeDirection() const{ return this->dataPtr->latitude_direction_; }
double Nmea::Longitude() const{ return this->dataPtr->longitude_; }
string Nmea::LongitudeDirection() const{ return this->dataPtr->longitude_direction_; }
uint8_t Nmea::FixQuality() const{ return this->dataPtr->fix_quality_; }
string Nmea::PosType() const{ return this->dataPtr->pos_type_; }
uint8_t Nmea::SatelliteNumber() const{ return this->dataPtr->satellite_number_; }
double Nmea::Altitude() const{ return this->dataPtr->altitude_; }
double Nmea::Azimuth() const{ return this->dataPtr->azimuth_; }
double Nmea::Pitch() const{ return this->dataPtr->pitch_; }
double Nmea::Speed() const{ return this->dataPtr->speed_; }

////////////////////////////////////////////
// set methods
void Nmea::UtcTime(const utc_time &_value){ this->dataPtr->utc_time_ = _value; }
void Nmea::UtcOffset(const double &_value){ this->dataPtr->utc_offset_ = _value; }
void Nmea::Latitude(const double &_value){ this->dataPtr->latitude_ = _value; }
void Nmea::LatitudeDirection(const string &_value){ this->dataPtr->latitude_direction_ = _value;}
void Nmea::Longitude(const double &_value){ this->dataPtr->longitude_ = _value; }
void Nmea::LongitudeDirection(const string &_value){ this->dataPtr->longitude_direction_ = _value; }
void Nmea::FixQuality(const uint8_t &_value){ this->dataPtr->fix_quality_ = _value;}
void Nmea::PosType(const string &_value){ this->dataPtr->pos_type_ = _value;}
void Nmea::SatelliteNumber(const uint8_t &_value){ this->dataPtr->satellite_number_ = _value; }
void Nmea::Altitude(const double &_value){ this->dataPtr->altitude_ = _value; }
void Nmea::Azimuth(const double &_value){ this->dataPtr->azimuth_ = _value; }
void Nmea::Pitch(const double &_value){ this->dataPtr->pitch_ = _value; }
void Nmea::Speed(const double &_value){ this->dataPtr->speed_ = _value; }

///////////////////////////////
Nmea &Nmea::operator=(const Nmea &_value)
{
    this->dataPtr->utc_time_  = _value.UtcTime();
    this->dataPtr->utc_offset_  = _value.UtcOffset();
    this->dataPtr->latitude_  = _value.Latitude();
    this->dataPtr->latitude_direction_  = _value.LatitudeDirection();
    this->dataPtr->longitude_  = _value.Longitude();
    this->dataPtr->longitude_direction_  = _value.LongitudeDirection();
    this->dataPtr->azimuth_  = _value.Azimuth();
    this->dataPtr->pitch_ = _value.Pitch();
    this->dataPtr->altitude_ = _value.Altitude();
    this->dataPtr->speed_ = _value.Speed();
    this->dataPtr->fix_quality_  = _value.FixQuality();
    this->dataPtr->pos_type_ = _value.PosType();
    this->dataPtr->satellite_number_  = _value.SatelliteNumber();
    return *this;

}

