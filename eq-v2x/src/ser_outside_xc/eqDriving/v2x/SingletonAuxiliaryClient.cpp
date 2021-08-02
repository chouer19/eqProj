//======================================================================
/* ! \file SingletonAuxiliaryClient.cpp
 *
 * \copydoc Copyright
 * \author Xue Chong(xc)
 * \date Sep 12, 2019
 *
 * a singleton class for auxiliary client
 * one client for eatch request
 * using cpp-netlib
 * Usage:  
 *///-------------------------------------------------------------------
 
#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/network/protocol/http/client.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>

#include "ser_outside_xc/thirdParty/sqlite/sqlite3.h"
#include "ser_outside_xc/eqDriving/v2x/SingletonAuxiliaryClient.hpp"
#include "ser_outside_xc/eqDriving/util/Logger.hpp"
#include "ser_outside_xc/eqDriving/util/Strlib.hpp"

using namespace std;
using namespace eqDriving;
using namespace util;

void display(const string &_type, const string &_request, const string &_response){
    cout << "\n================================\n";
    cout << "this is a : " << _type << endl;
    cout << "*** request is :***\n " << _request<< endl;
    cout << "*** response is :***\n " << _response<< endl;
    cout << "================================\n\n";
}

string utcTime(){
    //return boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
    return boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
}

string SingletonAuxiliaryClient::loadoffDispatch(     
          const vector<point_2d_t> &_coordinates,
          const int &_itemType,
          const double &_azimuth
          ){
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    string coordinates = "";
    for(auto vertex : _coordinates){
        coordinates += "" + to_string(vertex.x) + ",";
        coordinates += "" + to_string(vertex.y) + "#";
    }
    body_content += "\"coordinates\":\""+coordinates+ "\",\n";
    body_content += "\"itemType\":\""+to_string(_itemType)+ "\",\n";
    body_content += "\"courseAngle\":\""+to_string(_azimuth)+ "\"";
    body_content += "\n}";
    cout << body_content << endl;
    string response_body = http_post(this->url_ + "/auxiliary/loadoffDispatch", body_content);
    string code = readValueByKey(response_body, "code"); 
    display("loadoffDispatch",body_content, response_body);
    return code;
}
string SingletonAuxiliaryClient::loadoffDispatch(                                                
          const string &_vin,
          const vector<point_2d_t> &_coordinates,
          const int &_itemType,
          const double &_azimuth
          ){
    this->vehicleVin(_vin);
    return this->loadoffDispatch(_coordinates, _itemType, _azimuth);
}

string SingletonAuxiliaryClient::loadoffDispatch(     
          const string &_vin,
          const vector<point_2d_t> &_coordinates,
          const int &_itemType,
          const double &_latitude,
          const double &_longitude,
          const double &_azimuth
          ){
    this->vehicleVin(_vin);

    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    string coordinates = "";
    for(auto vertex : _coordinates){
        coordinates += "" + to_string(vertex.x) + ",";
        coordinates += "" + to_string(vertex.y) + "#";
    }
    body_content += "\"coordinates\":\""+coordinates+ "\",\n";
    body_content += "\"itemType\":\""+to_string(_itemType)+ "\",\n";
    body_content += "\"latitude\":\""+to_string(_latitude)+ "\",\n";
    body_content += "\"longitude\":\""+to_string(_longitude)+ "\",\n";
    body_content += "\"courseAngle\":\""+to_string(_azimuth)+ "\"";
    body_content += "\n}";
    cout << body_content << endl;
    string response_body = http_post(this->url_ + "/auxiliary/loadoffDispatch", body_content);
    string code = readValueByKey(response_body, "code"); 
    display("loadoffDispatch",body_content, response_body);
    return code;
}

getLoadPlace_respond_t SingletonAuxiliaryClient::getLoadPlace(const string &_vin, const string &_load_area_id){
    this->vehicleVin(_vin);
    this->loadAreaId(_load_area_id);
    return this->getLoadPlace();
}
getLoadPlace_respond_t SingletonAuxiliaryClient::getLoadPlace(){
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"loadAreaId\":\""+this->loadAreaId() + "\"";
    body_content += "\n}";
    string response_body = http_post(this->url_ + "/auxiliary/getLoadPlace", body_content);
    getLoadPlace_respond_t tmp_respond;
    tmp_respond.code = readValueByKey(response_body, "code"); 
    tmp_respond.load_space_id = readReturnDataValueByKey(response_body,"loadPositionId");
    SplitString splitline(readReturnDataValueByKey(response_body,"loadPositionIds"));
    tmp_respond.load_space_ids = splitline.split(',',1);
    tmp_respond.latitude = atof(readReturnDataValueByKey(response_body,"latitude").c_str());
    tmp_respond.longitude = atof(readReturnDataValueByKey(response_body,"longitude").c_str());
    tmp_respond.azimuth = atof(readReturnDataValueByKey(response_body,"azimuth").c_str());
    display("getLoadPlace",body_content, response_body);
    return tmp_respond;
}
confirmLoadPlace_respond_t SingletonAuxiliaryClient::confirmLoadPlace(
                              const double &_latitude,
                              const double &_longitude,
                              const double &_azimuth,
                              const vector<point_2d_t> &_coordinates,
                              const bool &_is_new
                              ){
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"loadAreaId\":\""+this->loadAreaId() + "\",\n";
    body_content += "\"loadPositionId\":\""+this->loadSpaceId() + "\",\n";
    body_content += "\"latitude\":\""+to_string(_latitude) + "\",\n";
    body_content += "\"longitude\":\""+to_string(_longitude) + "\",\n";
    body_content += "\"azimuth\":\""+to_string(_azimuth) + "\",\n";
    string coordinates = "";
    for(auto vertex : _coordinates){
        coordinates += "" + to_string(vertex.x) + ",";
        coordinates += "" + to_string(vertex.y) + "#";
    }
    coordinates = string(coordinates, 0, coordinates.size() > 0? coordinates.size() -1 : 0);
    body_content += "\"coordinates\":\""+coordinates+ "\",\n";
    string isNew= _is_new ? "1":"0";
    body_content += "\"isNew\":\""+isNew+ "\"";
    body_content += "\n}";
    string response_body = http_post(this->url_ + "/auxiliary/confirmLoadPlace", body_content);
    confirmLoadPlace_respond_t tmp_respond;
    tmp_respond.code = readValueByKey(response_body, "code"); 
    tmp_respond.load_space_id = readReturnDataValueByKey(response_body,"loadPositionId");
    display("confirmLoadPlace",body_content, response_body);
    return tmp_respond;
}

confirmLoadPlace_respond_t SingletonAuxiliaryClient::confirmLoadPlace(
                              const string &_load_area_id,
                              const string &_load_space_id,
                              const double &_latitude,
                              const double &_longitude,
                              const double &_azimuth,
                              const vector<point_2d_t> &_coordinates,
                              const bool &_is_new
                              ){
    this->loadAreaId(_load_area_id);
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"loadAreaId\":\""+this->loadAreaId() + "\",\n";
    if(!_is_new){
        this->loadSpaceId(_load_space_id);
        body_content += "\"loadPositionId\":\""+this->loadSpaceId() + "\",\n";
    }
    body_content += "\"latitude\":\""+to_string(_latitude) + "\",\n";
    body_content += "\"longitude\":\""+to_string(_longitude) + "\",\n";
    body_content += "\"azimuth\":\""+to_string(_azimuth) + "\",\n";
    string coordinates = "";
    for(auto vertex : _coordinates){
        coordinates += "" + to_string(vertex.x) + ",";
        coordinates += "" + to_string(vertex.y) + "#";
    }
    coordinates = string(coordinates, 0, coordinates.size() > 0? coordinates.size() -1 : 0);
    body_content += "\"coordinates\":\""+coordinates+ "\",\n";
    string isNew= _is_new ? "1":"0";
    body_content += "\"isNew\":\""+isNew+ "\"";
    body_content += "\n}";
    string response_body = http_post(this->url_ + "/auxiliary/confirmLoadPlace", body_content);
    confirmLoadPlace_respond_t tmp_respond;
    tmp_respond.code = readValueByKey(response_body, "code"); 
    tmp_respond.load_space_id = readReturnDataValueByKey(response_body,"loadPositionId");
    display("confirmLoadPlace",body_content, response_body);
    return tmp_respond;
}

string utcTimeToString(const utc_time &tm){
    return to_string(tm.year) + "-" + to_string(tm.month) + "-" + to_string(tm.day) + " " +
           to_string(tm.hour) + ":" + to_string(tm.minute) + ":" + 
           to_string(tm.second) + "." + to_string(tm.millisecond);
}
string SingletonAuxiliaryClient::unloadPlaceScan(const utc_time &_start_time, const utc_time &_end_time){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"startTime\":\""+utcTimeToString(_start_time)+"\",\n";
    body_content += "\"endTime\":\""+utcTimeToString(_end_time)+"\"\n";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/unloadPlaceScan", body_content);
    code = readValueByKey(response_body, "code"); 
    display("unloadPlaceScan",body_content, response_body);
    return code;
}
string SingletonAuxiliaryClient::unloadPlaceScan(const string &_start_time, const string &_end_time){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"startTime\":\""+_start_time+"\",\n";
    body_content += "\"endTime\":\""+_end_time+"\"\n";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/unloadPlaceScan", body_content);
    code = readValueByKey(response_body, "code"); 
    return code;
}
string SingletonAuxiliaryClient::loadFinish(const string &_vin, const string &_driver_id, const bool &_is_hard){
    this->vehicleVin(_vin);
    this->driverId(_driver_id);
    return this->loadFinish(_is_hard);
}
string SingletonAuxiliaryClient::loadFinish(const bool &_is_hard){
    string code = "";
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + "\",\n";
    body_content += "\"driver\":\""+this->driverId() + "\",\n";
    string isHard = _is_hard ? "1":"0";
    body_content += "\"isHard\":\""+isHard + "\"";
    body_content += "\n}";
    string response_body = http_post(this->url_ + "/auxiliary/loadFinish", body_content);
    code = readValueByKey(response_body, "code"); 
    display("loadFinish",body_content, response_body);
    return code;
}
string SingletonAuxiliaryClient::loadFinish(){
    string code = "";
    string body_content = "{\n";
    body_content += "\"vin\":\""+this->vehicleVin() + ",\n";
    body_content += "\"driver\":\""+this->driverId() +",\n";
    body_content += "\"isHard\":\"0\n";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/loadFinish", body_content);
    code = readValueByKey(response_body, "code"); 
    return code;
}
string SingletonAuxiliaryClient::terminalSystemSet(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() + "\"";
    body_content += ",\"terminalNo\":\""+this->terminalNumber() +"\"";
    body_content += ",\"terminalType\":\""+to_string(this->vehicleType())+"\"";
    body_content += ",\"sim\":\""+this->simId() +"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/terminalSystemSet", body_content);
    code = readValueByKey(response_body, "code"); 
    display("terminalSystemSet",body_content, response_body);
    return code;
}

string SingletonAuxiliaryClient::terminalSystemSet(const string &_terminal_number, const int &_vehicle_type, const string &_sim_id){
    this->terminalNumber(_terminal_number);
    this->vehicleType(_vehicle_type);
    this->simId(_sim_id);
    return terminalSystemSet();
}
string SingletonAuxiliaryClient::terminalSystemSetVehicleType(const int &_vehicle_type){
    this->vehicleType(_vehicle_type);
    terminalSystemSet();
}
string SingletonAuxiliaryClient::terminalSystemSetSimId(const string &_sim_id){
    this->simId(_sim_id);
    return terminalSystemSet();
}

utc_time generate_utc_time(string ts){
    utc_time tmp_utc;   
    SplitString raw(ts);
    vector<string> rawcontents = raw.split(' ',1);
    if(rawcontents.size() != 2){return tmp_utc;}
    SplitString day(rawcontents[0]);
    vector<string> daycontents= raw.split('-',1);
    if(daycontents.size() != 3){return tmp_utc;}
    tmp_utc.year = atoi(daycontents[0].c_str());
    tmp_utc.month = atoi(daycontents[1].c_str());
    tmp_utc.day = atoi(daycontents[2].c_str());
    
    SplitString ms(rawcontents[1].c_str());
    vector<string> mscontents= ms.split(':',1);
    if(mscontents.size() != 3){return tmp_utc;}
    tmp_utc.hour = atoi(mscontents[0].c_str());
    tmp_utc.minute = atoi(mscontents[1].c_str());
    tmp_utc.second = (int)atof(mscontents[2].c_str());
    tmp_utc.millisecond = ((int)( 1000 * atof(mscontents[2].c_str())) % 1000);
    
    //boost::posix_time::ptime pt(boost::posix_time::time_from_string(ts));
    //system_clock::duration tp = now.time_since_epoch();
    //tm pt_tm = boost::posix_time::to_tm(pt);
    //tmp_utc.year = pt_tm.tm_year + 1900;
    //tmp_utc.month = pt_tm.tm_mon;
    //tmp_utc.day = pt_tm.tm_mday;
    //tmp_utc.hour = pt_tm.tm_hour;
    //tmp_utc.minute = pt_tm.tm_min;
    //tmp_utc.second = pt_tm.tm_sec;
    //tmp_utc.millisecond = pt_tm.year;
}

getRequestStatus_respond_t SingletonAuxiliaryClient::generate_getRequestStatus_respond(const string &_code, const string &_operate_time, const string &_examine_time, const string &_request_type, const string &_result){
    getRequestStatus_respond_t tmp_respond;
    tmp_respond.code = _code;
    tmp_respond.operate_time = generate_utc_time(_operate_time);
    tmp_respond.examine_time = generate_utc_time(_examine_time);
    tmp_respond.request_type = _request_type;
    tmp_respond.result = _result;
    return tmp_respond;
}
getRequestStatus_respond_t SingletonAuxiliaryClient::getRequestStatus(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() +"\"";
    body_content += ",\"faultType\":\""+to_string(this->requestType())+"\"";
    body_content += ",\"proposer\":\""+this->driverId()+"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/getRequestStatus", body_content);
    code = readValueByKey(response_body, "code"); 
    string serviceTime = readReturnDataValueByKey(response_body,"serviceTime");
    string examineTime = readReturnDataValueByKey(response_body,"examineTime");
    string examineResult = readReturnDataValueByKey(response_body,"examineResult");
    cout << examineResult << endl;
    string type = readReturnDataValueByKey(response_body,"faultType");
    cout << type << endl;
    
    display("getRequestStatus",body_content, response_body);
    return generate_getRequestStatus_respond(code,serviceTime,examineTime,type,examineResult);
}
getRequestStatus_respond_t SingletonAuxiliaryClient::getRequestStatus(const string &_vin, const string &_driver_id, const int &_type){
    this->vehicleVin(_vin);
    this->requestType(_type);
    this->driverId(_driver_id);
    return this->getRequestStatus();
}
getRequestStatus_respond_t SingletonAuxiliaryClient::getRequestStatus(const string &_vin, const string &_driver_id){
    this->vehicleVin(_vin);
    this->driverId(_driver_id);
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() +"\"";
    body_content += ",\"proposer\":\""+this->driverId()+"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/getRequestStatus", body_content);
    code = readValueByKey(response_body, "code");
    string serviceTime = readReturnDataValueByKey(response_body,"serviceTime");
    string examineTime = readReturnDataValueByKey(response_body,"examineTime");
    string examineResult = readReturnDataValueByKey(response_body,"examineResult");
    cout << examineResult << endl;
    string type = readReturnDataValueByKey(response_body,"faultType");
    cout << type << endl;

    display("getRequestStatus",body_content, response_body);
    return generate_getRequestStatus_respond(code,serviceTime,examineTime,type,examineResult);
    //return this->getRequestStatus();
}

//getRequestStatus_respond_t SingletonAuxiliaryClient::getRequestStatus(const int &_type){
//    this->requestType(_type);
//    return this->getRequestStatus();
//}
//getRequestStatus_respond_t SingletonAuxiliaryClient::getRequestStatus(const string &_driver_id, const int &_type){
//    this->requestType(_type);
//    this->driverId(_driver_id);
//    return this->getRequestStatus();
//}
string SingletonAuxiliaryClient::delayedRequest(const int &_minutes){
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->duration(_minutes);
    return delayedRequest();
}

string SingletonAuxiliaryClient::delayedRequest(const string &_vin, const int &_vehicle_type, const int &_delay_type, const int &_minutes, const string &_driver_id){
    this->vehicleVin(_vin);
    this->vehicleType(_vehicle_type);
    this->driverId(_driver_id);
    this->delayType(_delay_type);
    this->duration(_minutes);
    return delayedRequest();
}
string SingletonAuxiliaryClient::delayedRequest(const int &_type, const int &_minutes){
    this->delayType(_type);
    this->duration(_minutes);
    return delayedRequest();
}
string SingletonAuxiliaryClient::delayedRequest(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() +"\"";
    body_content += ",\"vehicleType\":\""+to_string(this->vehicleType())+"\"";
    body_content += ",\"faultType\":\""+to_string(this->delayType())+"\"";
    body_content += ",\"times\":\""+to_string(this->duration())+"\"";
    body_content += ",\"proposer\":\""+this->driverId()+"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/delayedRequest", body_content);
    display("delayedRequest",body_content, response_body);
    code = readValueByKey(response_body, "code"); 
    return code;
}
string SingletonAuxiliaryClient::faultRequest(const string &_vin, const int &_vehicle_type, const int &_fault_type, const string&_message, const string &_driver_id){
    this->vehicleVin(_vin);
    this->vehicleType(_vehicle_type);
    this->faultType(_fault_type);
    this->faultMessage(_message);
    this->driverId(_driver_id);
    return faultRequest();
}
string SingletonAuxiliaryClient::faultRequest(const int &_type, const string&_message){
    this->faultType(_type);
    this->faultMessage(_message);
    return faultRequest();
}
string SingletonAuxiliaryClient::faultRequest(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin()+"\"";
    body_content += ",\"vehicleType\":\""+to_string(this->vehicleType())+"\"";
    body_content += ",\"faultType\":\""+to_string(this->faultType())+"\"";
    body_content += ",\"faultDescribe\":\""+this->faultMessage()+"\"";
    body_content += ",\"proposer\":\""+this->driverId()+"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/faultRequest", body_content);
    code = readValueByKey(response_body, "code"); 
    display("faultRequest",body_content, response_body);
    return code;
}
string SingletonAuxiliaryClient::updateVehicleStatus(const int &_status){
    this->vehicleStatus(_status);
    return updateVehicleStatus();
}
string SingletonAuxiliaryClient::updateVehicleStatus(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin()+"\"";
    body_content += ",\"status\":\""+to_string(this->vehicleStatus())+"\"";
    body_content += "}";
    cout << "request is : \n";
    cout << body_content << endl;
    string response_body = http_post(this->url_ + "/auxiliary/updateVehicleStatus", body_content);
    cout << "response is : \n";
    cout << response_body << endl;
    code = readValueByKey(response_body, "code"); 
    display("updateVehicleStatus",body_content, response_body);
    return code;
}
string SingletonAuxiliaryClient::logout(const string &_vin){
    this->vehicleVin(_vin);
    return this->logout();
}
string SingletonAuxiliaryClient::logout(){
    string code = "";
    string body_content = "{";
    body_content += "\"vin\":\""+this->vehicleVin() +"\"";
    body_content += "}";
    string response_body = http_post(this->url_ + "/auxiliary/loginOut", body_content);
    code = readValueByKey(response_body, "code"); 
    return code;
}
login_respond_t SingletonAuxiliaryClient::generate_login_respond(const string &_code){
    login_respond_t tmp_respond;
    tmp_respond.code = _code;
    tmp_respond.vehicle_number = this->vehicleNumber();
    tmp_respond.vehicle_numbers = this->vehicleNumbers();
    tmp_respond.driver_id =  this->driverId();
    tmp_respond.driver_name = this->driverName();
    tmp_respond.platform_id = this->platformId();
    tmp_respond.load_area_id = this->loadAreaId();
    tmp_respond.item_type = this->itemType();
    return tmp_respond;
}
login_respond_t SingletonAuxiliaryClient::login(const string &_driver_number){
    this->driverNumber(_driver_number);
    return login();
}
login_respond_t SingletonAuxiliaryClient::login(const string &_driver_number,const string &_vehicle_vin, const int &_type){
    this->driverNumber(_driver_number);
    this->vehicleVin(_vehicle_vin);
    this->vehicleType(_type);
    return login();
}
login_respond_t SingletonAuxiliaryClient::login(const string &_vehicle_vin, const int &_type){
    this->vehicleVin(_vehicle_vin);
    this->vehicleType(_type);
    return login();
}
void SingletonAuxiliaryClient::test(){
    string body_content = "{\"vin\":\"VCEC360BL00017544\",\"type\":\"0\",\"securityOfficer\":\"A0125\"}";
    cout << body_content << endl;
    string response_body = http_post("http://125.33.25.66:28003/auxiliary/login", body_content);
    cout << response_body << endl;
}
login_respond_t SingletonAuxiliaryClient::login(){
    string code = "";
    std::string body_content = "{";
    body_content += "\"vin\":\""+ this->vehicleVin() +"\"";
    body_content += ",\"type\":\""+ to_string(this->vehicleType()) +"\"";
    body_content += ",\"securityOfficer\":\""+ this->driverNumber() +"\"";
    body_content += "}";
    string response_body = http_post(this->url() + "/auxiliary/login", body_content);
    //string response_body = http_post("http://125.33.25.66:28003/auxiliary/login", body_content);
    display("login",body_content, response_body);
    if(!ResponseBodyValid(response_body)){return generate_login_respond(code);}
    code = readValueByKey(response_body, "code"); 
    this->driverId(readReturnDataValueByKey(response_body,"driveId"));
    this->driverName(readReturnDataValueByKey(response_body,"driveName"));
    this->vehicleNumber(readReturnDataValueByKey(response_body,"vehicleNo"));
    SplitString splitline(readReturnDataValueByKey(response_body,"vehicles"));
    this->vehicleNumbers(splitline.split(',',1));

    this->platformId(readReturnDataValueByKey(response_body,"platform"));
    this->loadAreaId(readReturnDataValueByKey(response_body,"loadAreaId"));
    this->itemType(readReturnDataValueByKey(response_body,"itemType"));

    return generate_login_respond(code);
}

SingletonAuxiliaryClient::SingletonAuxiliaryClient(){
    this->initialize();
}

string SingletonAuxiliaryClient::http_post(const string &url, const string &request_body){
    string response_body = "";
    try {
        boost::network::http::client::options options;
        options.timeout(10);
        boost::network::http::client client(options);
        boost::network::http::client::request request(url);
        request << boost::network::body(request_body);
        request << boost::network::header( "Content-Type", "application/json;charset=UTF-8");
        request << boost::network::header( "Content-Length", std::to_string(request_body.size()));
        boost::network::http::client::response response = client.post(request);
        return body(response);
    } catch (std::exception &eee) {
        std::cerr << eee.what()<< endl; 
    }
    return response_body;
}
void SingletonAuxiliaryClient::initialize(){
     this->domain("http://125.33.25.66");
     this->port(28003);
     this->url_ = "http://125.33.25.66:28003/";
     vehicle_type_ = 0x01;
     load_params();
}

string SingletonAuxiliaryClient::readCode(const string &_body){
    simdjson::ParsedJson pj = simdjson::build_parsed_json(_body);
    simdjson::ParsedJson::Iterator pjh(pj);
    string code = "";
    if(!pjh.move_to_key("code")){ return code; }
    if(!pjh.is_string()){ pjh.up(); return code; } 
    code = pjh.get_string();
    pjh.up(); return code;
}
vector<string> SingletonAuxiliaryClient::readReturnDataValuesByKey(const string &_body, const string &_key){
    vector<string> value ;
    try{
        simdjson::ParsedJson pj = simdjson::build_parsed_json(_body);
        simdjson::ParsedJson::Iterator pjh(pj);
        if(!pjh.move_to_key("returnData")){ return value; }
        if(!pjh.is_object()){return value;}
        if(!pjh.move_to_key(_key.c_str())){ return value; }
        if(!pjh.is_array()){ pjh.up(); return value; } 
        pjh.down();
        do{
            if(pjh.is_string()){
                value.push_back(pjh.get_string());
            }
        }while(pjh.next());
        pjh.up(); return value;
    } catch (std::exception &eee) {
        Logger::Instance()->Writeln("Error: when parse http response body");
        Logger::Instance()->Writeln(_body);
        Logger::Instance()->Writeln(eee.what());
    }
    return value;
}


string SingletonAuxiliaryClient::readReturnDataValueByKey(const string &_body, const string &_key){
    string value = "";
    try{
        simdjson::ParsedJson pj = simdjson::build_parsed_json(_body);
        simdjson::ParsedJson::Iterator pjh(pj);
        if(!pjh.move_to_key("returnData")){ return value; }
        if(!pjh.is_object()){return value;}
        if(!pjh.move_to_key(_key.c_str())){ return value; }
        if(!pjh.is_string()){ pjh.up(); return value; } 
        value = pjh.get_string();
        pjh.up(); return value;
    } catch (std::exception &eee) {
        Logger::Instance()->Writeln("Error: when parse http response body");
        Logger::Instance()->Writeln(_body);
        Logger::Instance()->Writeln(eee.what());
    }
    return value;
}

string SingletonAuxiliaryClient::readValueByKey(const string &_body, const string &_key){
    string value = "";
    try{
        simdjson::ParsedJson pj = simdjson::build_parsed_json(_body);
        simdjson::ParsedJson::Iterator pjh(pj);
        if(!pjh.is_object()){return value;}
        if(!pjh.move_to_key(_key.c_str())){ return value; }
        if(!pjh.is_string()){ pjh.up(); return value; } 
        value = pjh.get_string();
        pjh.up(); 
    } catch (std::exception &eee) {
        Logger::Instance()->Writeln("Error: when parse http response body");
        Logger::Instance()->Writeln(_body);
        Logger::Instance()->Writeln(eee.what());
    }
    return value;
}

bool SingletonAuxiliaryClient::ResponseBodyValid(const string &_body){
    try{
        simdjson::ParsedJson pj = simdjson::build_parsed_json(_body);
        simdjson::ParsedJson::Iterator pjh(pj);
        if(!pjh.is_object()){return false;} 
    } catch (std::exception &eee) {
        Logger::Instance()->Writeln("Error: when checking http response body");
        Logger::Instance()->Writeln(_body);
        Logger::Instance()->Writeln(eee.what());
        return false;
    }
    return true;
}

SingletonAuxiliaryClient::~SingletonAuxiliaryClient(){
    //delete []jianquan_code;
}

string SingletonAuxiliaryClient::vehicleVin() const{
    return this->vehicle_vin_;
}

void SingletonAuxiliaryClient::vehicleVin(const string &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->vehicle_vin_ = _value;
}

int SingletonAuxiliaryClient::vehicleType() const{
    return this->vehicle_type_;
}
void SingletonAuxiliaryClient::vehicleType(const int &_type){
    this->vehicle_type_ = _type;
}

void SingletonAuxiliaryClient::url(const string &_url){
    this->url_ = _url;
}
string SingletonAuxiliaryClient::url() const{
    return this->url_;
}
void SingletonAuxiliaryClient::driverId(const string &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->driver_id_ = _value;
}
string SingletonAuxiliaryClient::driverId() const{
    return this->driver_id_;
}
                      
string SingletonAuxiliaryClient::driverName() const{
    return this->driver_name_;
}
void SingletonAuxiliaryClient::driverName(const string &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->driver_name_ = _value;
}
string SingletonAuxiliaryClient::driverNumber() const{
    return this->driver_number_;
}
void SingletonAuxiliaryClient::driverNumber(const string &_value){
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->driver_number_ = _value;
}

void SingletonAuxiliaryClient::vehicleNumber(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->vehicle_number_  = _value;
}
string SingletonAuxiliaryClient::vehicleNumber() const{
    return this->vehicle_number_ ;
}
vector<string> SingletonAuxiliaryClient::vehicleNumbers() const{
    return this->vehicle_numbers_ ;
}
void SingletonAuxiliaryClient::vehicleNumbers(const vector<string> &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->vehicle_numbers_  = _value;
}
string SingletonAuxiliaryClient::platformId() const{
    return this->platform_id_ ;
}
void SingletonAuxiliaryClient::platformId(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->platform_id_  = _value;
}
string SingletonAuxiliaryClient::loadAreaId() const{
    return this->load_area_id_ ;
}
void SingletonAuxiliaryClient::loadAreaId(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->load_area_id_ = _value;
}
string SingletonAuxiliaryClient::loadSpaceId() const{
    return this->load_space_id_ ;
}
void SingletonAuxiliaryClient::loadSpaceId(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->load_space_id_ = _value;
}
string SingletonAuxiliaryClient::itemType() const{
    return this->item_type_ ;
}
void SingletonAuxiliaryClient::itemType(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->item_type_  = _value;
}
int SingletonAuxiliaryClient::vehicleStatus() const{
    return this->status_;
}
void SingletonAuxiliaryClient::vehicleStatus(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->status_ = _value;
}
int SingletonAuxiliaryClient::faultType() const{
    return this->fault_type_;
}
void SingletonAuxiliaryClient::faultType(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->fault_type_ = _value;
}
string SingletonAuxiliaryClient::faultMessage() const{
    return this->fault_message_;
}
void SingletonAuxiliaryClient::faultMessage(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->fault_message_ = _value;
}
int SingletonAuxiliaryClient::duration() const{
    return this->duration_;
}
void SingletonAuxiliaryClient::duration(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->duration_ = _value;
}
int SingletonAuxiliaryClient::delayType() const{
    return this->delay_type_;
}
void SingletonAuxiliaryClient::delayType(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->delay_type_ = _value;
}
int SingletonAuxiliaryClient::requestType() const{
    return this->request_type_;
}
void SingletonAuxiliaryClient::requestType(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->request_type_ = _value;
}
string SingletonAuxiliaryClient::simId() const{
    return this->sim_id_;
}
void SingletonAuxiliaryClient::simId(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->sim_id_ = _value;
}
string SingletonAuxiliaryClient::terminalNumber() const{
    return this->terminal_number_;
}
void SingletonAuxiliaryClient::terminalNumber(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->terminal_number_ = _value;
}
void SingletonAuxiliaryClient::readParams() {
}

string SingletonAuxiliaryClient::domain() const{
    return this->domain_;
}
void SingletonAuxiliaryClient::domain(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->domain_ = _value;
}
int SingletonAuxiliaryClient::port() const{
    return this->port_;
}
void SingletonAuxiliaryClient::port(const int &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->port_ = _value;
}
string SingletonAuxiliaryClient::passwd() const{
    return this->passwd_;
}
void SingletonAuxiliaryClient::passwd(const string &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->passwd_ = _value;
}
bool SingletonAuxiliaryClient::readParams() const{
    return this->read_params_;
}
void SingletonAuxiliaryClient::readParams(const bool &_value) {
    boost::lock_guard<boost::mutex> lock{mutex_};
    this->read_params_ = _value;
}
int SingletonAuxiliaryClient::params_callback(void *pVoid, int argc, char **argv, char **azColName){
    SingletonAuxiliaryClient * p = (SingletonAuxiliaryClient *) pVoid;
//"INSERT INTO HTTP_PARAMS(ID ,VIN ,TOKEN ,TYPE ,TERMINAL_NUMBER,VEHICLE_NUMBER,SIM ,PASSWD ,AuxDomain ,AuxPort)"
   for(int i=0; i<argc; i++){
       p->vehicleVin(string(azColName[i]) == "VIN" ? argv[i] : p->vehicleVin());
       p->vehicleType(string(azColName[i]) == "TYPE" ? atoi(argv[i]) : p->vehicleType());
       p->terminalNumber(string(azColName[i]) == "TERMINAL_NUMBER" ? argv[i] : p->terminalNumber());
       p->vehicleNumber(string(azColName[i]) == "VEHICLE_NUMBER" ? argv[i] : p->vehicleNumber());
       p->simId(string(azColName[i]) == "SIM" ? argv[i] : p->simId());
       p->passwd(string(azColName[i]) == "PASSWD" ? argv[i] : p->passwd());
       p->domain(string(azColName[i]) == "AuxDomain" ? argv[i] : p->domain());
       p->port(string(azColName[i]) == "AuxPort" ? atoi(argv[i]) : p->port());
   }                 
   p->url(p->domain() + ":" + to_string(p->port())); 
   p->readParams(true);
   return 0;                                                                   
} 
bool SingletonAuxiliaryClient::load_params(){
    sqlite3 *db; char *zErrMsg = 0; int  rc; char *sql;                   
    /* Open database */          
    rc = sqlite3_open("params.conf", &db);
    if( rc ){                    
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        this->readParams(true);
        return false;
    }
    sql = "SELECT * from HTTP_PARAMS where ID =  1";
    rc = sqlite3_exec(db, sql, params_callback, this, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg); sqlite3_close(db);
        this->readParams(true);
        return false;
    }
    sqlite3_close(db);
    return true;                                                               
}

void SingletonAuxiliaryClient::updateParams(){
    sqlite3 *db; char *zErrMsg = 0; int  rc; char *sql;
    /* Open database */
    rc = sqlite3_open("params.conf", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        this->readParams(true);
        return ;
    }
    string SQL = "UPDATE HTTP_PARAMS SET VIN = '" +this->vehicleVin() +"' ,TOKEN = '"+ this->vehicleVin() + string(this->vehicleVin(),this->vehicleVin().size() > 3 ? this->vehicleVin().size()-3:0,3)  +"' ,TYPE="+to_string(this->vehicleType())+" ,TERMINAL_NUMBER = '"+this->terminalNumber()+"',VEHICLE_NUMBER ='" +this->vehicleNumber()+"',SIM = '"+this->simId()+"',PASSWD = '"+this->passwd()+"' ,AuxDomain = '"+this->domain()+"' ,AuxPort = " +to_string(this->port())+"  WHERE ID = 1;"; cout << SQL << endl;
    //sql = SQL.c_str();
    rc = sqlite3_exec(db, SQL.c_str(), params_callback, this, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg); sqlite3_close(db);
        this->readParams(true);
        return ;
    }
    sqlite3_close(db);
    return ;
}
