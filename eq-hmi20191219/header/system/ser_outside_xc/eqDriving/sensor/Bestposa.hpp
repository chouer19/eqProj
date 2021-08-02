//======================================================================a
/* ! \file Bestposa.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Bestposa 
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
        class BestposaPrivate;
      
        //
        /// \class Bestposa Bestposa.hh eqDriving/gis/Bestposa.hh
        /// \brief A three dimensional line segment. The line is defined by a
        /// start and end point.
        class Bestposa
        {
            /// \brief Bestposa Constructor
            public: Bestposa() ;
            public: ~Bestposa();

            /// \brief Copy constructor
            /// \param[in] _line a line object
            public: Bestposa(const Bestposa &_Bestposa);

            public: void TimeStamp(const size_t&_value);
            public: void Latitude(const double &_value);
            public: void Longitude(const double &_value);
            public: void PosType(const string &_value);
            public: void SatelliteNumber(const uint8_t &_value);
            public: void Altitude(const double &_value);

            public: size_t TimeStamp() const;
            public: double Latitude() const;
            public: double Longitude() const;
            public: string PosType() const;
            public: uint8_t SatelliteNumber() const;
            public: double Altitude() const;

            /// \brief Copy operator.
            /// \param[in] _material Material to copy.
            /// \return Reference to this material.
            public: Bestposa&operator=(const Bestposa &_Bestposa); 

            /// \brief Vector for storing the start and end points of the line
            private: BestposaPrivate *dataPtr = nullptr;
        };
    }
}
