//======================================================================a
/* ! \file Bestvela.hpp
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date September 28, 2019
 *                    
 * Bestvela 
 * Usage:  
 * 
 *///-------------------------------------------------------------------o
 
#pragma once
#include <string>
#include <iostream>
#include "pub_types.hpp"

using namespace std;

namespace eqDriving
{
    namespace sensor
    {
        class BestvelaPrivate;
      
        //
        /// \class Bestvela Bestvela.hh eqDriving/gis/Bestvela.hh
        /// \brief A three dimensional line segment. The line is defined by a
        /// start and end point.
        class Bestvela
        {
            /// \brief Bestvela Constructor
            public: Bestvela() ;
            public: ~Bestvela();

            /// \brief Copy constructor
            /// \param[in] _line a line object
            public: Bestvela(const Bestvela &_Bestvela);

            public: void TimeStamp(const size_t &_value);
            public: void VertSpeed(const double &_value);
            public: void Speed(const double &_value);

            public: size_t TimeStamp() const;
            public: double VertSpeed() const;
            public: double Speed() const;

            /// \brief Copy operator.
            /// \param[in] _material Material to copy.
            /// \return Reference to this material.
            public: Bestvela&operator=(const Bestvela &_Bestvela); 

            /// \brief Vector for storing the start and end points of the line
            private: BestvelaPrivate *dataPtr = nullptr;
        };
    }
}
