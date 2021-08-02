//======================================================================a
/* ! \file Time.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Time 
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
        class TimePrivate;
      
        //
        /// \class Time Time.hh eqDriving/gis/Time.hh
        /// \brief A three dimensional line segment. The line is defined by a
        /// start and end point.
        class Time
        {
            /// \brief Time Constructor
            public: Time() ;
            public: ~Time();

            /// \brief Copy constructor
            /// \param[in] _line a line object
            public: Time(const Time &_Time);

            public: void TimeStamp(const size_t &_value);
            public: void UtcTime(const utc_time &_value);

            public: size_t TimeStamp() const;
            public: utc_time UtcTime() const;

            /// \brief Copy operator.
            /// \param[in] _material Material to copy.
            /// \return Reference to this material.
            public: Time&operator=(const Time &_Time); 

            /// \brief Vector for storing the start and end points of the line
            private: TimePrivate *dataPtr = nullptr;
        };
    }
}
