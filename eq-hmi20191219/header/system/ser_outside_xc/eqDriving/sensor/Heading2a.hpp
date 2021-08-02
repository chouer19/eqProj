//======================================================================a
/* ! \file Heading2a.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Heading2a 
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
        class Heading2aPrivate;
      
        //
        /// \class Heading2a Heading2a.hh eqDriving/gis/Heading2a.hh
        /// \brief A three dimensional line segment. The line is defined by a
        /// start and end point.
        class Heading2a
        {
            /// \brief Heading2a Constructor
            public: Heading2a() ;
            public: ~Heading2a();

            /// \brief Copy constructor
            /// \param[in] _line a line object
            public: Heading2a(const Heading2a &_Heading2a);

            public: void TimeStamp(const size_t &_value);
            public: void Azimuth(const double &_value);
            public: void Pitch(const double &_value);

            public: size_t TimeStamp() const;
            public: double Azimuth() const;
            public: double Pitch() const;

            /// \brief Copy operator.
            /// \param[in] _material Material to copy.
            /// \return Reference to this material.
            public: Heading2a&operator=(const Heading2a &_Heading2a); 

            /// \brief Vector for storing the start and end points of the line
            private: Heading2aPrivate *dataPtr = nullptr;
        };
    }
}
