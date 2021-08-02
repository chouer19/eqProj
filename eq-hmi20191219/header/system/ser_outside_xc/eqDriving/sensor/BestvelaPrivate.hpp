//======================================================================       
/* ! \file BestvelaPrivate.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Bestvela pravite data
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
        /// \Bestvela private data.
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
        class BestvelaPrivate
        {
            public: size_t time_stamp_ = 0;
            public: double speed_ = 0;
            public: double vert_speed_ = 0;
        };
    }
}
