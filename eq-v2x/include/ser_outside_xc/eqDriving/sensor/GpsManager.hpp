

#pragma once
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "ser_outside_xc/eqDriving/common/SingletonT.hpp"
#include "ser_outside_xc/eqDriving/sensor/Nmea.hpp"
#include "ser_outside_xc/eqDriving/sensor/Bestposa.hpp"
#include "ser_outside_xc/eqDriving/sensor/Bestvela.hpp"
#include "ser_outside_xc/eqDriving/sensor/Heading2a.hpp"
#include "ser_outside_xc/eqDriving/sensor/Time.hpp"
#include "ser_outside_xc/eqDriving/serial/SimpleSerial.hpp"

SINGLETON_DECLARE(eqDriving , sensor, GpsManager)

namespace eqDriving{
    namespace sensor{
        class GpsManager : public SingletonT<GpsManager>{
            // \brief This is a singletone class. Use GpsManager::Instance()
            /// to get a pointer to this class.
            private: GpsManager();
         
            /// \brief Destructor
            private: ~GpsManager();
         
            /// \brief Init all the sensors
            public: void Init(std::string port, unsigned int baud_rate);
         
            /// \brief Run the sensor updates in a separate thread.
            public: void Run();  
        
            /// \brief Run sensor updates in separate threads.
            /// This will only run non-image based sensor updates.
            private: void RunLoop();
         
            /// \brief Stop the run thread
            public: void Stop();
    
            public: Nmea Data() const;
            public: Bestposa Posa() const;
            public: Bestvela Vela() const;
            public: Heading2a Heading() const;
            public: Time Utc() const;
         
            /// \brief Get whether there's at least one sensor container running.
            /// \return True if running.
            public: bool Running() const;
        
            /// \brief This is a singleton class.
            private: friend class SingletonT<GpsManager>; 
    
            /// \brief Flag to inidicate when to stop the runThread.
            private: bool stop;     
                                    
            /// \brief Flag to indicate that the sensors have been
            /// initialized.        
            private: bool initialized;
    
            /// \brief A thread to update the sensors.
            private: boost::thread *runThread;
                                    
            /// \brief A mutex to manage access to the sensors vector.
            private: mutable boost::recursive_mutex mutex;
            //private: boost::mutex n_mutex; 
                                               
            private: Nmea nmea;
            private: Bestposa bestposa;
            private: Bestvela bestvela;
            private: Heading2a heading2a;
            private: Time time;
            private: io::SimpleSerial sp;
        
        };
    }
}
