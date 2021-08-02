/* Desc: Singleton base class
 * Author: XueChong
 * Date: 2 Sept 2007
 */

#pragma once
template <class T>
class SingletonT
{
  /// \brief Get an instance of the singleton
  public: static T *Instance()
          {
            return &GetInstance();
          }

  /// \brief Constructor
  protected: SingletonT() {}

  /// \brief Destructor
  protected: virtual ~SingletonT() {}

  /// \brief Creates and returns a reference to the unique (static) instance
  private: static T &GetInstance()
           {
             static T t;
             return static_cast<T &>(t);
           }
};

#define SINGLETON_DECLARE(n1, n2, singletonType) \
namespace n1 \
{ \
  namespace n2 \
  { \
    class singletonType; \
  } \
} \
template class ::SingletonT<n1::n2::singletonType>;

