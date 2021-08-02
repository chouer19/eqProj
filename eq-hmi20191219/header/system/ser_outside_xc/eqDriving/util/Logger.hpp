#pragma once
#include <fstream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include "header/system/ser_outside_xc/eqDriving/common/SingletonT.hpp"

SINGLETON_DECLARE(eqDriving , util, Logger)

namespace eqDriving{
    namespace util{
        class Logger : public SingletonT<Logger>{
            // \brief This is a singletone class. Use Logger::Instance()
            /// to get a pointer to this class.
            private: Logger();
         
            /// \brief Destructor
            private: ~Logger();
         
            /// \brief Init all the utils
            public: void Init();
         
            /// \brief Run the util updates in a separate thread.
            public: void Write(std::string log);  
            public: void Writeln(std::string log);  
        
            /// \brief Get whether there's at least one util container running.
            /// \return True if running.
            public: bool Running() const;
            public: void Stop();
        
            /// \brief This is a singleton class.
            private: friend class SingletonT<Logger>; 
    
            /// \brief Flag to inidicate when to stop the runThread.
            private: bool stop;     
                                    
            /// \brief Flag to indicate that the utils have been
            /// initialized.        
            private: bool initialized;
    
            /// \brief A mutex to manage access to the utils vector.
            private: boost::mutex write_mutex; 

            //private: std::ofstream ofs;
            private: std::ofstream ofs;
            private: std::string filename = "NULL";
        };
    }
}
