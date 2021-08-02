#pragma once

#include "eqDriving/math/Helpers.hpp"

namespace eqDriving
{
  namespace math
  {
    //
    /// \class Angle Angle.hh eqDriving/math/Angle.hh
    /// \brief An angle and related functions.
    class Angle
    {
      /// \brief math::Angle(0)
      public: static const Angle Zero;

      /// \brief math::Angle(EQ_PI)
      public: static const Angle Pi;

      /// \brief math::Angle(EQ_PI * 0.5)
      public: static const Angle HalfPi;

      /// \brief math::Angle(EQ_PI * 2)
      public: static const Angle TwoPi;

      /// \brief Constructor
      public: Angle(){
        this->value = 0;
      }

      /// \brief Conversion Constructor
      /// \param[in] _radian Radians
      // cppcheck-suppress noExplicitConstructor
      public: Angle(const double _radian){
        this->value = _radian;
      }

      /// \brief Copy constructor
      /// \param[in] _angle Angle to copy
      public: Angle(const Angle &_angle){
        this->value = _angle.value;
      };

      /// \brief Destructor
      public: virtual ~Angle(){};

      /// \brief Set the value from an angle in radians
      /// \param[in] _radian Radian value
      public: void Radian(double _radian){
        this->value = _radian;
      };

      /// \brief Set the value from an angle in degrees
      /// \param[in] _degree Degree value
      public: void Degree(double _degree){
        this->value = _degree * EQ_PI / 180;
      };

      /// \brief Get the angle in radians
      /// \return double containing the angle's radian value
      public: double Radian() const{
        return this->value;
      }

      /// \brief Get the angle in degrees
      /// \return double containing the angle's degree value
      public: double Degree() const{
        return this->value * 180 / EQ_PI;
      }

      /// \brief Normalize the angle in the range -Pi to Pi
      public: void Normalize(){
        this->value = atan2(sin(this->value), cos(this->value));
      }

      /// \brief Return the angle's radian value
      /// \return double containing the angle's radian value
      public: double operator()() const;

      /// \brief Dereference operator
      /// \return Double containing the angle's radian value
      public: inline double operator*() const
              {
                return value;
              }

      /// \brief Substraction, result = this - _angle
      /// \param[in] _angle Angle for substraction
      /// \return the new angle
      public: Angle operator-(const Angle &_angle) const{
        return Angle(this->value - _angle.value);
      }

      /// \brief Addition operator, result = this + _angle
      /// \param[in] _angle Angle for addition
      /// \return the new angle
      public: Angle operator+(const Angle &_angle) const{
        return Angle(this->value + _angle.value);
      }

      /// \brief Multiplication operator, result = this * _angle
      /// \param[in] _angle Angle for multiplication
      /// \return the new angle
      public: Angle operator*(const Angle &_angle) const{
        return Angle(this->value * _angle.value);
      }

      /// \brief Division, result = this / _angle
      /// \param[in] _angle Angle for division
      /// \return the new angle
      public: Angle operator/(const Angle &_angle) const{
        return Angle(this->value / _angle.value);
      }

      /// \brief Subtraction set, this = this - _angle
      /// \param[in] _angle Angle for subtraction
      /// \return angle
      public: Angle operator-=(const Angle &_angle){
        this->value -= _angle.value;
        return *this;
      }

      /// \brief Addition set, this = this + _angle
      /// \param[in] _angle Angle for addition
      /// \return angle
      public: Angle operator+=(const Angle &_angle){
        this->value += _angle.value;
        return *this;
      }

      /// \brief Multiplication set, this = this * _angle
      /// \param[in] _angle Angle for multiplication
      /// \return angle
      public: Angle operator*=(const Angle &_angle){
        this->value *= _angle.value;
        return *this;
      }

      /// \brief Division set, this = this / _angle
      /// \param[in] _angle Angle for division
      /// \return angle
      public: Angle operator/=(const Angle &_angle){
        this->value /= _angle.value;
        return *this;
      }

      /// \brief Equality operator, result = this == _angle
      /// \param[in] _angle Angle to check for equality
      /// \return true if this == _angle
      public: bool operator==(const Angle &_angle) const{
        return equal(this->value, _angle.value, 0.001);
      }


      /// \brief Inequality
      /// \param[in] _angle Angle to check for inequality
      /// \return true if this != _angle
      public: bool operator!=(const Angle &_angle) const{
        return !(*this == _angle);
      }


      /// \brief Less than operator
      /// \param[in] _angle Angle to check
      /// \return true if this < _angle
      public: bool operator<(const Angle &_angle) const{
        return this->value < _angle.value;
      }


      /// \brief Less or equal operator
      /// \param[in] _angle Angle to check
      /// \return true if this <= _angle
      public: bool operator<=(const Angle &_angle) const{
        return this->value < _angle.value || equal(this->value, _angle.value);
      }


      /// \brief Greater than operator
      /// \param[in] _angle Angle to check
      /// \return true if this > _angle
      public: bool operator>(const Angle &_angle) const{
        return this->value > _angle.value;
      }

      /// \brief Greater or equal operator
      /// \param[in] _angle Angle to check
      /// \return true if this >= _angle
      public: bool operator>=(const Angle &_angle) const{
        return this->value > _angle.value || equal(this->value, _angle.value);
      }

      /// \brief Stream insertion operator. Outputs in degrees
      /// \param[in] _out output stream
      /// \param[in] _a angle to output
      /// \return The output stream
      public: friend std::ostream &operator<<(std::ostream &_out,
                                              const eqDriving::math::Angle &_a)
      {
        _out << _a.Radian();
        return _out;
      }

      /// \brief Stream extraction operator. Assumes input is in radians
      /// \param in input stream
      /// \param pt angle to read value into
      /// \return The input stream
      public: friend std::istream &operator>>(std::istream &_in,
                                              eqDriving::math::Angle &_a)
      {
        // Skip white spaces
        _in.setf(std::ios_base::skipws);
        _in >> _a.value;
        return _in;
      }

      /// The angle in radians
      private: double value;
    };
    //const Angle Angle::Zero(0);
    //const Angle Angle::Pi(EQ_PI);
    //const Angle Angle::HalfPi(EQ_PI_2);
    //const Angle Angle::TwoPi(EQ_PI * 2.0);
  }
}
