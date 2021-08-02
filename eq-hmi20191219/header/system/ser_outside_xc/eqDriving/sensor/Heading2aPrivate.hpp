//======================================================================       
/* ! \file Heading2aPrivate.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Heading2a pravite data
 * Usage:  
 * 
 *///-------------------------------------------------------------------

#pragma once
#include <string>
#include <iostream>
#include "header/system/pub_types.h"
using namespace std;
namespace eqDriving
{
    namespace sensor
    {
        /// \Heading2a private data.
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
        class Heading2aPrivate
        {
            public: size_t time_stamp_;
            public: double azimuth_ = 0;
            public: double pitch_ = 0;
            public: double speed_ = 0;
        };
    }
}
