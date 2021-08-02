#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"

#include "ser_outside_xc/eqDriving/sensor/Nmea.hpp"
#include "ser_outside_xc/eqDriving/sensor/Bestposa.hpp"
#include "ser_outside_xc/eqDriving/sensor/Bestvela.hpp"
#include "ser_outside_xc/eqDriving/sensor/Heading2a.hpp"
#include "ser_outside_xc/eqDriving/sensor/Time.hpp"
#include "ser_outside_xc/eqDriving/sensor/GpsManager.hpp"
#include "ser_outside_xc/eqDriving/util/Strlib.hpp"
#include "ser_outside_xc/eqDriving/util/Logger.hpp"
#include <boost/assert.hpp>

using namespace eqDriving;
using namespace sensor;
using namespace util;
using namespace std;

GpsManager::GpsManager(): initialized(false), stop(true) {}
GpsManager::~GpsManager(){}


size_t timeStamp(){
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_microseconds();
}

void GpsManager::Init(std::string port, unsigned int baud_rate){
    Logger::Instance()->Init();
    //boost::recursive_mutex::scoped_lock lock(this->mutex);
    if(this->initialized){return ;}
    try{
        if(!sp.open(port, baud_rate)){return ;};
        this->initialized = true;
        cout << "opened " << port << endl;
    }catch(exception &eee){
        Logger::Instance()->Writeln("ERROR: open serial port" + port);
        Logger::Instance()->Writeln(eee.what());
    }
}

void GpsManager::Stop() { 
    this->stop = true;         
    if (this->runThread)       
    {                          
        this->runThread->join(); 
        delete this->runThread;  
        this->runThread = nullptr;
    }
}

bool GpsManager::Running() const {
    return !this->stop;
}
Nmea GpsManager::Data() const{
    boost::recursive_mutex::scoped_lock lock(this->mutex);
    return this->nmea;
}
Bestposa GpsManager::Posa() const{
    boost::recursive_mutex::scoped_lock lock(this->mutex);
    return this->bestposa;
}
Bestvela GpsManager::Vela() const{
    boost::recursive_mutex::scoped_lock lock(this->mutex);
    return this->bestvela;
}
Heading2a GpsManager::Heading() const{
    boost::recursive_mutex::scoped_lock lock(this->mutex);
    return this->heading2a;
}
Time GpsManager::Utc() const{
    boost::recursive_mutex::scoped_lock lock(this->mutex);
    return this->time;
}

void GpsManager::Run() {                                     
    this->runThread = new boost::thread(
        boost::bind(&GpsManager::RunLoop, this));            
    BOOST_ASSERT_MSG(this->runThread, "Unable to create boost::thread.");
}  

void GpsManager::RunLoop() {
    this->stop = false;
    size_t count = 0;
    //cout << "loop start! \n";
    //cout << sp.readLine() << endl;
    while(!this->stop){
        //cout << "loop .........! \n";
        boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
        //boost::recursive_mutex::scoped_lock lock(this->mutex);
        SplitString splitline(sp.readLine());
        //cout << "read line is : ";
        //cout << splitline << endl;
        //continue;
        vector<string> linecontents = splitline.split(';',1);
        if(linecontents.size() < 2){continue;}
        if(linecontents[0].find("BESTPOSA") != std::string::npos &&
           linecontents[1].find("SOL_COMPUTED") != std::string::npos){
            cout << splitline << endl;
            SplitString sol_computed(linecontents[1]);
            vector<string> sol_contents = sol_computed.split(',',1);
            if(sol_contents.size() < 14){continue;}
            try{
                this->nmea.PosType(sol_contents[1]);
                this->nmea.Latitude(atof(sol_contents[2].c_str()));
                this->nmea.Longitude(atof(sol_contents[3].c_str()));
                this->nmea.Altitude(atof(sol_contents[4].c_str()));
                this->nmea.SatelliteNumber(atoi(sol_contents[13].c_str()));
                this->bestposa.PosType(sol_contents[1]);
                this->bestposa.Latitude(atof(sol_contents[2].c_str()));
                this->bestposa.Longitude(atof(sol_contents[3].c_str()));
                this->bestposa.Altitude(atof(sol_contents[4].c_str()));
                this->bestposa.SatelliteNumber(atoi(sol_contents[13].c_str()));
                this->bestposa.TimeStamp(timeStamp());
            }catch(exception ec){ continue; }
        } else if(linecontents[0].find("BESTVELA") != std::string::npos &&
           linecontents[1].find("SOL_COMPUTED") != std::string::npos){
            SplitString sol_computed(linecontents[1]);
            vector<string> sol_contents = sol_computed.split(',',1);
            if(sol_contents.size() < 7){continue;}
            try{
                this->nmea.Speed(atof(sol_contents[4].c_str()));
                this->bestvela.Speed(atof(sol_contents[4].c_str()));
                this->bestvela.VertSpeed(atof(sol_contents[6].c_str()));
                this->bestvela.TimeStamp(timeStamp());
            }catch(exception ec){ continue; }
        } else if(linecontents[0].find("HEADING2A") != std::string::npos &&
           linecontents[1].find("SOL_COMPUTED") != std::string::npos){
            SplitString sol_computed(linecontents[1]);
            vector<string> sol_contents = sol_computed.split(',',1);
            if(sol_contents.size() < 5){continue;}
            //this->nmea.PosType(sol_contents[1]);
            try{
                this->nmea.Azimuth(atof(sol_contents[3].c_str()));
                this->nmea.Pitch(atof(sol_contents[4].c_str()));
                this->heading2a.Azimuth(atof(sol_contents[3].c_str()));
                this->heading2a.Pitch(atof(sol_contents[4].c_str()));
                this->heading2a.TimeStamp(timeStamp());
            }catch(exception ec){ continue; }
        } else if(linecontents[0].find("TIME") != std::string::npos &&
            linecontents[1].find("VALID") != std::string::npos){
            SplitString sol_computed(linecontents[1]);
            vector<string> sol_contents = sol_computed.split(',',1);
            if(sol_contents.size() < 11){continue;}
            try{
                this->nmea.UtcOffset(atof(sol_contents[3].c_str()));
                this->nmea.UtcTime(utc_time(
                    atoi(sol_contents[4].c_str()),
                    atoi(sol_contents[5].c_str()),
                    atoi(sol_contents[6].c_str()),
                    atoi(sol_contents[7].c_str()),
                    atoi(sol_contents[8].c_str()),
                    atoi(sol_contents[9].c_str())/1000,
                    atoi(sol_contents[9].c_str())%1000)
                );
                this->time.UtcTime(utc_time(
                    atoi(sol_contents[4].c_str()),
                    atoi(sol_contents[5].c_str()),
                    atoi(sol_contents[6].c_str()),
                    atoi(sol_contents[7].c_str()),
                    atoi(sol_contents[8].c_str()),
                    atoi(sol_contents[9].c_str())/1000,
                    atoi(sol_contents[9].c_str())%1000)
                );
                this->time.TimeStamp(timeStamp());
            }catch(exception ec){ continue; }
        }
    }
    cout << "loop done!\n";
}
