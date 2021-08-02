#pragma once

#include <chrono>
#include <cmath>
#include <algorithm>
#include <limits>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
#include <cstdint>


/// \brief The default tolerance value used by MassMatrix3::IsValid(),
/// MassMatrix3::IsPositive(), and MassMatrix3::ValidMoments()
//template <typename T>
//constexpr T EQ_MASSMATRIX3_DEFAULT_TOLERANCE = T(10);

#define EQ_PI   3.14159265358979323846264338327950288
#define EQ_PI_2 1.57079632679489661923132669163975144
#define EQ_PI_4 0.78539816339744830961566334581987572
#define EQ_SQRT2 1.41421356237309504880

/// \brief Compute sphere volume
/// \param[in] _radius Sphere radius
#define EQ_SPHERE_VOLUME(_radius) (4.0*EQ_PI*std::pow(_radius, 3)/3.0)

/// \brief Compute cylinder volume
/// \param[in] _r Cylinder base radius
/// \param[in] _l Cylinder length
#define EQ_CYLINDER_VOLUME(_r, _l) (_l * EQ_PI * std::pow(_r, 2))

/// \brief Compute box volume
/// \param[in] _x X length
/// \param[in] _y Y length
/// \param[in] _z Z length
#define EQ_BOX_VOLUME(_x, _y, _z) (_x *_y * _z)

/// \brief Compute box volume from a vector
/// \param[in] _v Vector3d that contains the box's dimensions.
#define EQ_BOX_VOLUME_V(_v) (_v.X() *_v.Y() * _v.Z())

namespace eqDriving
{
  /// \brief Math classes and function useful in robot applications.
  namespace math
  {
    //
    /// \brief size_t type with a value of 0
    static const size_t EQ_ZERO_SIZE_T  = 0u;

    /// \brief size_t type with a value of 1
    static const size_t EQ_ONE_SIZE_T   = 1u;

    /// \brief size_t type with a value of 2
    static const size_t EQ_TWO_SIZE_T   = 2u;

    /// \brief size_t type with a value of 3
    static const size_t EQ_THREE_SIZE_T = 3u;

    /// \brief size_t type with a value of 4
    static const size_t EQ_FOUR_SIZE_T  = 4u;

    /// \brief size_t type with a value of 5
    static const size_t EQ_FIVE_SIZE_T  = 5u;

    /// \brief size_t type with a value of 6
    static const size_t EQ_SIX_SIZE_T   = 6u;

    /// \brief size_t type with a value of 7
    static const size_t EQ_SEVEN_SIZE_T = 7u;

    /// \brief size_t type with a value of 8
    static const size_t EQ_EIGHT_SIZE_T = 8u;

    /// \brief size_t type with a value of 9
    static const size_t EQ_NINE_SIZE_T  = 9u;

    /// \brief Double maximum value. This value will be similar to 1.79769e+308
    static const double MAX_D = std::numeric_limits<double>::max();

    /// \brief Double min value. This value will be similar to 2.22507e-308
    static const double MIN_D = std::numeric_limits<double>::min();

    /// \brief Double low value, equivalent to -MAX_D
    static const double LOW_D = std::numeric_limits<double>::lowest();

    /// \brief Double positive infinite value
    static const double INF_D = std::numeric_limits<double>::infinity();

    /// \brief Returns the representation of a quiet not a number (NAN)
    static const double NAN_D = std::numeric_limits<double>::quiet_NaN();

    /// \brief Float maximum value. This value will be similar to 3.40282e+38
    static const float MAX_F = std::numeric_limits<float>::max();

    /// \brief Float minimum value. This value will be similar to 1.17549e-38
    static const float MIN_F = std::numeric_limits<float>::min();

    /// \brief Float low value, equivalent to -MAX_F
    static const float LOW_F = std::numeric_limits<float>::lowest();

    /// \brief float positive infinite value
    static const float INF_F = std::numeric_limits<float>::infinity();

    /// \brief Returns the representation of a quiet not a number (NAN)
    static const float NAN_F = std::numeric_limits<float>::quiet_NaN();

    /// \brief 16bit unsigned integer maximum value
    static const uint16_t MAX_UI16 = std::numeric_limits<uint16_t>::max();

    /// \brief 16bit unsigned integer minimum value
    static const uint16_t MIN_UI16 = std::numeric_limits<uint16_t>::min();

    /// \brief 16bit unsigned integer lowest value. This is equivalent to
    /// EQ_UINT16_MIN, and is defined here for completeness.
    static const uint16_t LOW_UI16 = std::numeric_limits<uint16_t>::lowest();

    /// \brief 16-bit unsigned integer positive infinite value
    static const uint16_t INF_UI16 = std::numeric_limits<uint16_t>::infinity();

    /// \brief 16bit unsigned integer maximum value
    static const int16_t MAX_I16 = std::numeric_limits<int16_t>::max();

    /// \brief 16bit unsigned integer minimum value
    static const int16_t MIN_I16 = std::numeric_limits<int16_t>::min();

    /// \brief 16bit unsigned integer lowest value. This is equivalent to
    /// EQ_INT16_MIN, and is defined here for completeness.
    static const int16_t LOW_I16 = std::numeric_limits<int16_t>::lowest();

    /// \brief 16-bit unsigned integer positive infinite value
    static const int16_t INF_I16 = std::numeric_limits<int16_t>::infinity();

    /// \brief 32bit unsigned integer maximum value
    static const uint32_t MAX_UI32 = std::numeric_limits<uint32_t>::max();

    /// \brief 32bit unsigned integer minimum value
    static const uint32_t MIN_UI32 = std::numeric_limits<uint32_t>::min();

    /// \brief 32bit unsigned integer lowest value. This is equivalent to
    /// EQ_UINT32_MIN, and is defined here for completeness.
    static const uint32_t LOW_UI32 = std::numeric_limits<uint32_t>::lowest();

    /// \brief 32-bit unsigned integer positive infinite value
    static const uint32_t INF_UI32 = std::numeric_limits<uint32_t>::infinity();

    /// \brief 32bit unsigned integer maximum value
    static const int32_t MAX_I32 = std::numeric_limits<int32_t>::max();

    /// \brief 32bit unsigned integer minimum value
    static const int32_t MIN_I32 = std::numeric_limits<int32_t>::min();

    /// \brief 32bit unsigned integer lowest value. This is equivalent to
    /// EQ_INT32_MIN, and is defined here for completeness.
    static const int32_t LOW_I32 = std::numeric_limits<int32_t>::lowest();

    /// \brief 32-bit unsigned integer positive infinite value
    static const int32_t INF_I32 = std::numeric_limits<int32_t>::infinity();

    /// \brief 64bit unsigned integer maximum value
    static const uint64_t MAX_UI64 = std::numeric_limits<uint64_t>::max();

    /// \brief 64bit unsigned integer minimum value
    static const uint64_t MIN_UI64 = std::numeric_limits<uint64_t>::min();

    /// \brief 64bit unsigned integer lowest value. This is equivalent to
    /// EQ_UINT64_MIN, and is defined here for completeness.
    static const uint64_t LOW_UI64 = std::numeric_limits<uint64_t>::lowest();

    /// \brief 64-bit unsigned integer positive infinite value
    static const uint64_t INF_UI64 = std::numeric_limits<uint64_t>::infinity();

    /// \brief 64bit unsigned integer maximum value
    static const int64_t MAX_I64 = std::numeric_limits<int64_t>::max();

    /// \brief 64bit unsigned integer minimum value
    static const int64_t MIN_I64 = std::numeric_limits<int64_t>::min();

    /// \brief 64bit unsigned integer lowest value. This is equivalent to
    /// EQ_INT64_MIN, and is defined here for completeness.
    static const int64_t LOW_I64 = std::numeric_limits<int64_t>::lowest();

    /// \brief 64-bit unsigned integer positive infinite value
    static const int64_t INF_I64 = std::numeric_limits<int64_t>::infinity();

    /// \brief Returns the representation of a quiet not a number (NAN)
    static const int NAN_I = std::numeric_limits<int>::quiet_NaN();

    /// \brief Simple clamping function
    /// \param[in] _v value
    /// \param[in] _min minimum
    /// \param[in] _max maximum
    template<typename T>
    inline T clamp(T _v, T _min, T _max)
    {
      return std::max(std::min(_v, _max), _min);
    }

    /// \brief check if a float is NaN
    /// \param[in] _v the value
    /// \return true if _v is not a number, false otherwise
    inline bool isnan(float _v)
    {
      return (std::isnan)(_v);
    }

    /// \brief check if a double is NaN
    /// \param[in] _v the value
    /// \return true if _v is not a number, false otherwise
    inline bool isnan(double _v)
    {
      return (std::isnan)(_v);
    }

    /// \brief Fix a nan value.
    /// \param[in] _v Value to correct.
    /// \return 0 if _v is NaN, _v otherwise.
    inline float fixnan(float _v)
    {
      return isnan(_v) || std::isinf(_v) ? 0.0f : _v;
    }

    /// \brief Fix a nan value.
    /// \param[in] _v Value to correct.
    /// \return 0 if _v is NaN, _v otherwise.
    inline double fixnan(double _v)
    {
      return isnan(_v) || std::isinf(_v) ? 0.0 : _v;
    }

    /// \brief Check if parameter is even.
    /// \param[in] _v Value to check.
    /// \return True if _v is even.
    inline bool isEven(const int _v)
    {
      return !(_v % 2);
    }

    /// \brief Check if parameter is even.
    /// \param[in] _v Value to check.
    /// \return True if _v is even.
    inline bool isEven(const unsigned int _v)
    {
      return !(_v % 2);
    }

    /// \brief Check if parameter is odd.
    /// \param[in] _v Value to check.
    /// \return True if _v is odd.
    inline bool isOdd(const int _v)
    {
      return (_v % 2) != 0;
    }

    /// \brief Check if parameter is odd.
    /// \param[in] _v Value to check.
    /// \return True if _v is odd.
    inline bool isOdd(const unsigned int _v)
    {
      return (_v % 2) != 0;
    }

    /// \brief The signum function.
    ///
    /// Returns 0 for zero values, -1 for negative values,
    /// +1 for positive values.
    /// \param[in] _value The value.
    /// \return The signum of the value.
    template<typename T>
    inline int sgn(T _value)
    {
      return (T(0) < _value) - (_value < T(0));
    }

    /// \brief The signum function.
    ///
    /// Returns 0 for zero values, -1 for negative values,
    /// +1 for positive values.
    /// \param[in] _value The value.
    /// \return The signum of the value.
    template<typename T>
    inline int signum(T _value)
    {
      return sgn(_value);
    }

    /// \brief get mean of vector of values
    /// \param[in] _values the vector of values
    /// \return the mean
    template<typename T>
    inline T mean(const std::vector<T> &_values)
    {
      T sum = 0;
      for (unsigned int i = 0; i < _values.size(); ++i)
        sum += _values[i];
      return sum / _values.size();
    }

    /// \brief get variance of vector of values
    /// \param[in] _values the vector of values
    /// \return the squared deviation
    template<typename T>
    inline T variance(const std::vector<T> &_values)
    {
      T avg = mean<T>(_values);

      T sum = 0;
      for (unsigned int i = 0; i < _values.size(); ++i)
        sum += (_values[i] - avg) * (_values[i] - avg);
      return sum / _values.size();
    }

    /// \brief get the maximum value of vector of values
    /// \param[in] _values the vector of values
    /// \return maximum
    template<typename T>
    inline T max(const std::vector<T> &_values)
    {
      T max = std::numeric_limits<T>::min();
      for (unsigned int i = 0; i < _values.size(); ++i)
        if (_values[i] > max)
          max = _values[i];
      return max;
    }

    /// \brief get the minimum value of vector of values
    /// \param[in] _values the vector of values
    /// \return minimum
    template<typename T>
    inline T min(const std::vector<T> &_values)
    {
      T min = std::numeric_limits<T>::max();
      for (unsigned int i = 0; i < _values.size(); ++i)
        if (_values[i] < min)
          min = _values[i];
      return min;
    }

    /// \brief check if two values are equal, within a tolerance
    /// \param[in] _a the first value
    /// \param[in] _b the second value
    /// \param[in] _epsilon the tolerance
    template<typename T>
    inline bool equal(const T &_a, const T &_b,
                      const T &_epsilon = T(1e-6))
    {
      T diff = std::abs(_a - _b);
      return diff <= _epsilon;
    }

    /// \brief inequality test, within a tolerance
    /// \param[in] _a the first value
    /// \param[in] _b the second value
    /// \param[in] _epsilon the tolerance
    template<typename T>
    inline bool lessOrNearEqual(const T &_a, const T &_b,
                            const T &_epsilon = 1e-6)
    {
      return _a < _b + _epsilon;
    }

    /// \brief inequality test, within a tolerance
    /// \param[in] _a the first value
    /// \param[in] _b the second value
    /// \param[in] _epsilon the tolerance
    template<typename T>
    inline bool greaterOrNearEqual(const T &_a, const T &_b,
                               const T &_epsilon = 1e-6)
    {
      return _a > _b - _epsilon;
    }

    /// \brief get value at a specified precision
    /// \param[in] _a the number
    /// \param[in] _precision the precision
    /// \return the value for the specified precision
    template<typename T>
    inline T precision(const T &_a, const unsigned int &_precision)
    {
      auto p = std::pow(10, _precision);
      return static_cast<T>(std::round(_a * p) / p);
    }

    /// \brief Sort two numbers, such that _a <= _b
    /// \param[out] _a the first number
    /// \param[out] _b the second number
    template<typename T>
    inline void sort2(T &_a, T &_b)
    {
      using std::swap;
      if (_b < _a)
        swap(_a, _b);
    }

    /// \brief Sort three numbers, such that _a <= _b <= _c
    /// \param[out] _a the first number
    /// \param[out] _b the second number
    /// \param[out] _c the third number
    template<typename T>
    inline void sort3(T &_a, T &_b, T &_c)
    {
      // _a <= _b
      sort2(_a, _b);
      // _a <= _c, _b <= _c
      sort2(_b, _c);
      // _a <= _b <= _c
      sort2(_a, _b);
    }

    /// \brief Is this a power of 2?
    /// \param[in] _x the number
    /// \return true if _x is a power of 2, false otherwise
    inline bool isPowerOfTwo(unsigned int _x)
    {
      return ((_x != 0) && ((_x & (~_x + 1)) == _x));
    }

    /// \brief Get the smallest power of two that is greater or equal to
    /// a given value
    /// \param[in] _x the number
    /// \return the same value if _x is already a power of two. Otherwise,
    /// it returns the smallest power of two that is greater than _x
    inline unsigned int roundUpPowerOfTwo(unsigned int _x)
    {
      if (_x == 0)
        return 1;

      if (isPowerOfTwo(_x))
        return _x;

      while (_x & (_x - 1))
        _x = _x & (_x - 1);

      _x = _x << 1;

      return _x;
    }

    /// \brief parse string into an integer
    /// \param[in] _input the string
    /// \return an integer, 0 or 0 and a message in the error stream
    inline int parseInt(const std::string &_input)
    {
      // Return NAN_I if it is empty
      if (_input.empty())
      {
        return NAN_I;
      }
      // Return 0 if it is all spaces
      else if (_input.find_first_not_of(' ') == std::string::npos)
      {
        return 0;
      }

      // Otherwise try standard library
      try
      {
        return std::stoi(_input);
      }
      // if that fails, return NAN_I
      catch(...)
      {
        return NAN_I;
      }
    }

    /// \brief parse string into float
    /// \param _input the string
    /// \return a floating point number (can be NaN) or 0 with a message in the
    /// error stream
    inline double parseFloat(const std::string &_input)
    {
      // Return NAN_D if it is empty
      if (_input.empty())
      {
        return NAN_D;
      }
      // Return 0 if it is all spaces
      else if (_input.find_first_not_of(' ') == std::string::npos)
      {
        return 0;
      }

      // Otherwise try standard library
      try
      {
        return std::stod(_input);
      }
      // if that fails, return NAN_D
      catch(...)
      {
        return NAN_D;
      }
    }

    /// \brief Convert a std::chrono::steady_clock::duration to a seconds and
    /// nanoseconds pair.
    /// \param[in] _dur The duration to convert.
    /// \return A pair where the first element is the number of seconds and
    /// the second is the number of nanoseconds.
    inline std::pair<int64_t, int64_t> durationToSecNsec(
        const std::chrono::steady_clock::duration &_dur)
    {
      auto s = std::chrono::duration_cast<std::chrono::seconds>(_dur);
      auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(_dur-s);
      return {s.count(), ns.count()};
    }
  }
}
