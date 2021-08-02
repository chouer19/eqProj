#pragma once

#include <vector>
#include <iostream>
#include "eqDriving/math/Vector3.hpp"
#include "eqDriving/math/Quaternion.hpp"

using namespace std;
using namespace eqDriving;
using namespace math;
namespace eqDriving
{
  namespace vehicle
  {
    class StatePrivate;
    //
    /// \class State State.hh eqDriving/gis/State.hh
    /// \brief A three dimensional line segment. The line is defined by a
    /// start and end point.
    class State
    {
      /// \brief State Constructor
      public: State(const Vector3d &_xyzp, const Vector3d &_origin);
      public: State() ;
      public: State(const Vector3d &_llhp);
      public: State(const double &_latitude, const double &_longitude, const double &_height);
      public: State(const double &_latitude, const double &_longitude, const double &_height, const double &yaw);
      public: State(const double &_latitude, const double &_longitude, const double &_height, const double &_yaw, const double &_roll, const double &_pitch);
      public: State(const Vector3d &_llhp, const Quaterniond &_rot);
      public: State(const Vector3d &_xyzp, const int &_zone, const bool &_north);
      public: State(const Vector3d &_xyzp, const int &_zone, const bool &_north, const Quaterniond &_rot);
      public: ~State();
      /// \brief Copy constructor
      /// \param[in] _line a line object
      public: State(const State &_point);

      public: void Update(const Vector3d &_cmder);

      public: double Latitude() const;
      public: double Longitude() const;
      public: double Height() const;
      public: Vector3d LLHPos() const;
      public: double X() const;
      public: void X(const double &_value) ;
      public: double Y() const;
      public: void Y(const double &_value) ;
      public: double Z() const;
      public: void Z(const double &_value) ;
      public: Vector3d XYZPos() const;
      public: int Zone() const;
      public: bool IsNorth() const;
      public: double Yaw() const;
      public: void Yaw(const double &_value) ;
      public: double Roll() const;
      public: double Pitch() const;
      public: Quaterniond Rot() const;
      public: Vector3d Velocity() const;
      public: Vector3d Acceleration() const;
      public: double Wheelbase() const;
      public: double Track() const;

      public: void Pos(const Vector3d &_llhp);
      public: void Pos(const Vector3d &_xyzp, const int &_zone, const bool &_north);
      public: void LLHPos(const double &_lat, const double &_lon, const double &_height);
      public: void XYZPos(const double &_x, const double &_y, const double &_z);
      public: void Velocity(const double &_x, const double &_y, const double &_z);
      public: void Acceleration(const double &_x, const double &_y, const double &_z);
      public: void Rot(const Quaterniond &_rot);
      public: void Rot(const double &_roll, const double &_pitch, const double &_yaw);

      public: void Latitude(const double &_lat);
      public: void Longitude(const double &_lon);
      public: void Height(const double &_height) ;

      /// \brief Copy operator.
      /// \param[in] _material Material to copy.
      /// \return Reference to this material.
      public: State&operator=(const State &_point); 

      /// \brief Vector for storing the start and end points of the line
      private: StatePrivate *dataPtr = nullptr;
    };
  }
}
