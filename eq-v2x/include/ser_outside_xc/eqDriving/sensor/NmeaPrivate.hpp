//======================================================================       
/* ! \file NmeaPrivate.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Nmea pravite data
 * Usage:  
 * 
 *///-------------------------------------------------------------------

#pragma once
#include <string>
#include <iostream>
#include "pub_types.hpp"
using namespace std;
namespace eqDriving
{
    namespace sensor
    {
        /// \Nmea private data.
        /// ============== GPZDA, UTC time ============
        // * Date & Time
        // * 
        // * UTC, day, month, year, and local time zone.
        // * 
        // * $--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx
        // * hhmmss.ss = UTC 
        // * xx = Day, 01 to 31 
        // * xx = Month, 01 to 12 
        // * xxxx = Year 
        // * xx = Local zone description, 00 to +/- 13 hours 
        // * xx = Local zone minutes description (same sign as hours)
        /// GPZDA, UTC time 
        /// GPVTG, speed
        /// GPGSV, Azimuth
        /// GPGGA, lat,lon,fix,sattelites,altitude,
        class NmeaPrivate
        {
            public: utc_time utc_time_;
            public: double utc_offset_;
            public: double latitude_ = 0;
            public: string latitude_direction_ = "0";
            public: double longitude_ = 0;
            public: string longitude_direction_ = "0";
            public: uint8_t fix_quality_ = 0;
            public: string pos_type_ = "NONE";
            public: uint8_t satellite_number_ = 0;
            public: double altitude_ = 0;
            public: double azimuth_ = 0;
            public: double pitch_ = 0;
            public: double speed_ = 0;
        };
    }
}
