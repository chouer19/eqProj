#include <iomanip>
#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "ser_outside_xc/eqDriving/sensor/GpsManager.hpp"
#include "ser_outside_xc/eqDriving/util/Logger.hpp"

using namespace std;
using namespace eqDriving;
using namespace sensor;
using namespace util;

void testLogger(){
    while(true){
        Logger::Instance()->Writeln("logger 1");
        boost::this_thread::sleep_for(boost::chrono::milliseconds{200});
    }
}

void testLogger2(){
    while(true){
        Logger::Instance()->Write("logger 2\n==\n");
        boost::this_thread::sleep_for(boost::chrono::milliseconds{100});
    }
}

int main(int argc, char ** argv){
    //GpsManager::Instance()->Init("/dev/ttyUSB1",115200);
    //GpsManager::Instance()->Init("/dev/ttyUSB0",115200);
    if(argc < 2){ cout << argv[0] << " portname baundary" << endl; return 0;}
    GpsManager::Instance()->Init(argv[1],atoi(argv[2]));
    GpsManager::Instance()->Run();

    cout << std::setprecision(15);
    boost::this_thread::sleep_for(boost::chrono::milliseconds{1500});

    while(true){
        cout << "calling..........................................\n";
        cout << GpsManager::Instance()->Posa().TimeStamp() << endl;
        cout << "lat : " ;
        //cout << GpsManager::Instance()->Data().Latitude() << endl;
        cout << GpsManager::Instance()->Posa().Latitude() << endl;
        cout << "lon : " ;
        //cout << GpsManager::Instance()->Data().Longitude() << endl;
        cout << GpsManager::Instance()->Posa().Longitude() << endl;
        cout << "altitude : " ;
        //cout << GpsManager::Instance()->Data().Altitude() << endl;
        cout << GpsManager::Instance()->Posa().Altitude() << endl;
        cout << "pos type: " ;
        //cout << GpsManager::Instance()->Data().PosType() << endl;
        cout << GpsManager::Instance()->Posa().PosType() << endl;
        cout << "number: " ;
        //cout << (int)GpsManager::Instance()->Data().SatelliteNumber() << endl;
        cout << (int)GpsManager::Instance()->Posa().SatelliteNumber() << endl;

        cout << GpsManager::Instance()->Vela().TimeStamp() << endl;
        cout << "speed: " ;
        //cout << GpsManager::Instance()->Data().Speed() << endl;
        cout << GpsManager::Instance()->Vela().Speed() << endl;
        cout << GpsManager::Instance()->Vela().VertSpeed() << endl;

        cout << GpsManager::Instance()->Heading().TimeStamp() << endl;
        cout << "azimuth: " ;
        //cout << GpsManager::Instance()->Data().Azimuth() << endl;
        cout << GpsManager::Instance()->Heading().Azimuth() << endl;
        cout << "pitch: " ;
        //cout << GpsManager::Instance()->Data().Pitch() << endl;
        cout << GpsManager::Instance()->Heading().Pitch() << endl;

        cout << GpsManager::Instance()->Utc().TimeStamp() << endl;
        cout << "utc offset: " ;
        //cout << GpsManager::Instance()->Data().UtcOffset() << endl;
        cout << "utc time: " ;
        cout << (int) GpsManager::Instance()->Data().UtcTime().year << "-";
        cout << (int)GpsManager::Instance()->Utc().UtcTime().year << endl;
        cout << (int) GpsManager::Instance()->Data().UtcTime().month << "-";
        cout << (int) GpsManager::Instance()->Data().UtcTime().day << " ";
        cout << (int) GpsManager::Instance()->Data().UtcTime().hour << ":";
        cout << (int) GpsManager::Instance()->Data().UtcTime().minute << ":";
        cout << (int) GpsManager::Instance()->Data().UtcTime().second << ".";
        cout << (int) GpsManager::Instance()->Data().UtcTime().millisecond << endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds{800});
        cout << "done..........................................\n\n";
    }
    return 1;
}
