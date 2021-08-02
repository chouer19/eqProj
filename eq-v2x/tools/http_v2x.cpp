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
    GpsManager::Instance()->Init("/dev/ttyUSB0",115200);
    GpsManager::Instance()->Run();

    //boost::thread tLogger(testLogger);
    //boost::thread tLogger2(testLogger2);

    while(true){
        cout << "calling......\n";
        cout << GpsManager::Instance()->Data().Latitude() << endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds{1000});
    }
    return 1;
}
