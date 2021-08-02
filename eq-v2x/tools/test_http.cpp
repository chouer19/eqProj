#include <iostream>
#include "ser_outside_xc/eqDriving/v2x/SingletonAuxiliaryClient.hpp"

using namespace std;
using namespace eqDriving;

SingletonAuxiliaryClient *SingletonAuxiliaryClient::instance = 0;

int main(int argc, char ** argv){
    SingletonAuxiliaryClient *s = s->getInstance();
    s->url("dafdfdw2efl");
    s->login("A0125");
    login_respond_t login_respond = s->login("S1003");
    cout << login_respond.code << endl;
    cout << "vehicle number" << login_respond.vehicle_number  << endl;
    for(int i=0;i<login_respond.vehicle_numbers.size() ; cout << login_respond.vehicle_numbers[i++] << endl ){;}
    cout << login_respond.driver_id << endl;
    cout << login_respond.driver_name  << endl;
    cout << login_respond.platform_id  << endl;
    cout << login_respond.load_area_id << endl;
    cout << login_respond.item_type  << endl;

    string code = s->updateVehicleStatus(1);
    cout << code << endl;

    cout << s->driverId() << endl;

    code = s->faultRequest(0,"I do not know what happened!");
    cout << code << endl;

    code = s->delayedRequest(0,5);
    cout << code << endl;

    //getRequestStatus_respond_t getRequestStatus_respond = s->getRequestStatus(1);
    getRequestStatus_respond_t getRequestStatus_respond = s->getRequestStatus(s->vehicleVin(), s->driverId(), 1);
    cout << getRequestStatus_respond.code << endl;
    cout << getRequestStatus_respond.operate_time.year << endl;

    code = s->terminalSystemSet("98020",1,"123098778324213");
    cout << code << endl;

    code = s->loadFinish(1);
    cout << code << endl;

    vector<point_2d_t> coordinates;
    coordinates.push_back(point_2d_t(123,98));
    coordinates.push_back(point_2d_t(123,98));
    coordinates.push_back(point_2d_t(123,98));
    coordinates.push_back(point_2d_t(123,98));
    confirmLoadPlace_respond_t confirmLoadPlace_respond = s->confirmLoadPlace(39.923, 110.234, 250.5, coordinates, true);

    getLoadPlace_respond_t getLoadPlace_respond = s->getLoadPlace();

    code = s->unloadPlaceScan(
        utc_time(2019,10,17,16,20,30,255), 
        utc_time(2019,10,17,16,20,36,255)
        );
    cout << code << endl;

    vector<point_2d_t> coordinates2;
    coordinates2.push_back(point_2d_t(123,98));
    coordinates2.push_back(point_2d_t(123,98));
    coordinates2.push_back(point_2d_t(123,98));
    coordinates2.push_back(point_2d_t(123,98));
    code = s->loadoffDispatch(coordinates2,
                             1,
                             224.5);
    cout << code << endl;
    s->simId("18813128144");
    s->updateParams();

    return 1;
}
