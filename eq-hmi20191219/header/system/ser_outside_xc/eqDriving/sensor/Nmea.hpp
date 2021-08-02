//======================================================================a
/* ! \file Nmea.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Nmea 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o
 
#pragma once
#include <string>
#include <iostream>
#include "header/system/pub_types.h"

using namespace std;

namespace eqDriving
{
    namespace sensor
    {
        class NmeaPrivate;
      
        //
        /// \class Nmea Nmea.hh eqDriving/gis/Nmea.hh
        /// \brief A three dimensional line segment. The line is defined by a
        /// start and end point.
        class Nmea
        {
            /// \brief Nmea Constructor
            public: Nmea() ;
            public: ~Nmea();

            /// \brief Copy constructor
            /// \param[in] _line a line object
            public: Nmea(const Nmea &_Nmea);

            public: void UtcTime(const utc_time &_value);
            public: void UtcOffset(const double &_value);
            public: void Latitude(const double &_value);
            public: void LatitudeDirection(const string &_value);
            public: void Longitude(const double &_value);
            public: void LongitudeDirection(const string &_value);
            public: void FixQuality(const uint8_t &_value);
            public: void PosType(const string &_value);
            public: void SatelliteNumber(const uint8_t &_value);
            public: void Altitude(const double &_value);
            public: void Azimuth(const double &_value);
            public: void Pitch(const double &_value);
            public: void Speed(const double &_value);

            public: utc_time UtcTime() const;
            public: double UtcOffset() const;
            public: double Latitude() const;
            public: string LatitudeDirection() const;
            public: double Longitude() const;
            public: string LongitudeDirection() const;
            public: uint8_t FixQuality() const;
            public: string PosType() const;
            public: uint8_t SatelliteNumber() const;
            public: double Altitude() const;
            public: double Azimuth() const;
            public: double Pitch() const;
            public: double Speed() const;

            /// \brief Copy operator.
            /// \param[in] _material Material to copy.
            /// \return Reference to this material.
            public: Nmea&operator=(const Nmea &_Nmea); 

            /// \brief Vector for storing the start and end points of the line
            private: NmeaPrivate *dataPtr = nullptr;
        };
    }
}
