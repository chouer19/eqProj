//======================================================================
/* ! \file SingletonAuxiliaryClient.hpp
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

#pragma once
#include <string> 
#include <vector>
#include <iostream>
#include <boost/thread.hpp>
#include "header/system/ser_outside_xc/thirdParty/simdjson/simdjson.hpp"
#include "header/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"
#include "header/system/pub_types.h"

using namespace std;

namespace eqDriving{
    class SingletonAuxiliaryClient {
    public:
        /// an instance
        static SingletonAuxiliaryClient *getInstance() {
            if (instance == 0){
                 instance = new SingletonAuxiliaryClient;
            }   
            return instance;
        }
    
        void test();
        login_respond_t login();
        login_respond_t login(const string &_vehicle_vin, const int &_type);
        login_respond_t login(const string &_driver_number, const string &_vehicle_vin, const int &_type);
        login_respond_t login(const string &_driver_number);
        string logout();
        string logout(const string &_vin);
        string updateVehicleStatus();
        string updateVehicleStatus(const int &_status);
        string updateVehicleStatus(const int &_status,const string &_id);
        string faultRequest();
        string faultRequest(const int &_type, const string &_message);
        string faultRequest(const string &_vin, const int &_vehicle_type, const int &_fault_type, const string&_message, const string &_driver_id);
        string delayedRequest();
        string delayedRequest(const int &_minutes);
        string delayedRequest(const int &_type, const int &_minutes);
        string delayedRequest(const string &_vin, const int &_vehicle_type, const int &_delay_type, const int &_minutes, const string &_driver_id);
        getRequestStatus_respond_t getRequestStatus();
//        getRequestStatus_respond_t getRequestStatus(const int &_type);
        getRequestStatus_respond_t getRequestStatus(const string &_vin, const string &_driver_id);
        getRequestStatus_respond_t getRequestStatus(const string &_vin, const string &_driver_id, const int &_type);
        string terminalSystemSet();
        string terminalSystemSet(const string &_vehicle_number, const int &_vehicle_type, const string &_sim_id);
        string terminalSystemSetVehicleType(const int &vehicle_type);
        string terminalSystemSetSimId(const string &_sim_id);
        loadFinish_respond_t loadFinish();
        loadFinish_respond_t loadFinish(const bool &_is_hard);
        loadFinish_respond_t loadFinish(const string &_vin, const string &_driver_id, const bool &_is_hard);
        loadFinish_respond_t loadFinish(const string &_vin, const string &_driver_id, const bool &_is_hard, const point_3d_t &_coordinate);
        //string loadFinish();
        //string confirmLoadPlace(const string &_load_number, );
        getLoadPlace_respond_t getLoadPlace(const string &_vin, const string &_load_area_id);
        getLoadPlace_respond_t getLoadPlace();
        confirmLoadPlace_respond_t confirmLoadPlace(
                                  const string &_load_area_id,
                                  const string &_load_space_id,
                                  const double &_latitude,
                                  const double &_longitude,
                                  const double &_azimuth,
                                  const vector<point_2d_t> &_coordinates,
                                  const bool &_is_new
                                  );
        confirmLoadPlace_respond_t confirmLoadPlace(
                                  const string &_load_area_id,
                                  const string &_load_space_id,
                                  const double &_latitude,
                                  const double &_longitude,
                                  const double &_azimuth,
                                  const vector<point_2d_t> &_coordinates,
                                  const bool &_is_new,
                                  const int &_autoConfirm,
                                  const int &_loadMode,
                                  const point_3d_t &_coordinate
                                  );
        confirmLoadPlace_respond_t confirmLoadPlace(
                                  const double &_latitude,
                                  const double &_longitude,
                                  const double &_azimuth,
                                  const vector<point_2d_t> &_coordinates,
                                  const bool &_is_new
                                      );
        string unloadPlaceScan(const string &_start_time, const string &_end_time);
        string unloadPlaceScan(const utc_time &_start_time, const utc_time &_end_time);
        string loadoffDispatch(
                  const vector<point_2d_t> &_coordinates,
                  const int &_itemType,
                  const double &_azimuth
                  );
        string loadoffDispatch(
                  const string &_vin,
                  const vector<point_2d_t> &_coordinates,
                  const int &_itemType,
                  const double &_azimuth
                  );
        string loadoffDispatch(
                  const string &_vin,
                  const vector<point_2d_t> &_coordinates,
                  const int &_itemType,
                  const double &_latitude,
                  const double &_longitude,
                  const double &_azimuth
                  );
        //string getAroundVehicles();

        string taskFinish(
                  const string &_vin,
                  const string &_taskNo
        );

        string stopAroundVehicle(
                  const string &_vehicleNo,
                  const int &_vehicleType
        );

        getVehicleTask_respond_t getVehicleTask(const string &_vehicleNo);

        string clearLoadPosition(const string &_vin, const int &_type, const point_3d_t &_pos);

        ~SingletonAuxiliaryClient();

        string vehicleVin() const;
        void vehicleVin(const string&_value);
        int vehicleType() const;
        void vehicleType(const int &_type);
        string url() const;
        void url(const string &_url);
        string domain() const;
        void domain(const string &_url);
        int port() const;
        void port(const int &_type);

        string vehicleNumber() const;
        void vehicleNumber(const string &_value);
        string driverId() const;
        void driverId(const string &_value);
        string driverName() const;
        void driverName(const string &_value);
        string driverNumber() const;
        void driverNumber(const string &_value);
        vector<string> vehicleNumbers() const;
        void vehicleNumbers(const vector<string> &_value);
        string platformId() const;
        void platformId(const string &_value);
        string loadAreaId() const;
        void loadAreaId(const string &_value) ;
        string loadSpaceId() const;
        void loadSpaceId(const string &_value) ;
        string itemType() const;
        void itemType(const string &_value) ;

        void vehicleStatus(const int &_value);
        int vehicleStatus() const;

        void faultType(const int &_value);
        int faultType() const;
        void faultMessage(const string &_value);
        string faultMessage() const;

        void delayType(const int &_value);
        int delayType() const;
        void duration(const int &_value);
        int duration() const;

        void requestType(const int &_value);
        int requestType() const;

        void simId(const string &_value);
        string simId() const;

        void terminalNumber(const string &_value);
        string terminalNumber() const;

        void passwd(const string &_value);
        string passwd() const;

        void readParams();
        void updateParams();

        bool load_params();
        static int params_callback(void *pVoid, int argc, char **argv, char **azColName);

        bool readParams() const;
        void readParams(const bool &_value) ;


    private:
        static SingletonAuxiliaryClient *instance;
        boost::mutex mutex_;
        bool read_params_ = false;

        string header_token_ = "";
        void header_token(const string &_value);
        string header_token();
        /// params about login 2.1.1
        //char vehicle_vin_[17] = {'f'};
        string passwd_ = "ffffff";
        string url_ = "fff";
        string domain_ = "fff";
        int port_ = 0;
        string vehicle_vin_;
        int vehicle_type_ = 0xff;
        string driver_number_ = "";

        login_respond_t generate_login_respond(const string &_code);
        //login_respond_t generate_login_respond(
        //           const string &_code,
        //           const point_3d_t &_gps_xyz,
        //           const point_3d_t &_gps_xyz,
        //           const point_3d_t &_gps_xyz,
        //           const double &_deviationAngle
        //          );
        string vehicle_number_ = "NULL";
        vector<string> vehicle_numbers_;
        string driver_id_ = "NULL";
        string driver_name_ = "NULL";
        string platform_id_ = "NULL";
        string load_area_id_ = "NULL";
        string item_type_ = "NULL";
    
        /// params about loginOut 2.1.2
        /// *** vin


        /// params about updateVehicleStatus 2.1.3
        /// *** vin
        int status_;

        /// params about faultRequest 2.1.4
        /// *** vin
        /// *** vehicle_type_
        int fault_type_;
        string fault_message_;
        /// ** driver_id_

        /// params about delayedRequest 2.1.5
        /// *** vin
        /// *** vehicle_type_
        int delay_type_ = 0;
        int duration_ = 0;
        /// ** driver_id_

        getRequestStatus_respond_t generate_getRequestStatus_respond(const string &_code, const string &_operate_time, const string &_examin_time, const string &_request_type, const string &_result);
        /// params about getRequestStatus 2.1.6
        /// *** vin
        /// *** driver_id_
        int request_type_ = 0;
        
        /// params about terminalSystemSet 2.1.7
        /// *** vin
        /// int request_type_ = 0;
        string terminal_number_ = "fff";
        string sim_id_ = "000";
        /// *** vehicle_type_

        /// operate time, examine time, type, result

        /// params about loadFinish 2.1.8
        /// *** vin
        /// *** driver_id_
        bool is_hard_ = false;

        /// params about confirmLoadPlace 2.1.9
        /// *** vin
        /// *** load_area_id_
        string load_space_id_ = "fff";
        double load_latitude_ = 0;
        double load_longitude_ = 0;
        double load_azimuth_ = 0;
        bool load_is_new = false;

        /// *** load_space_id_

        /// params about getLoadPlace 2.1.10
        /// *** vin
        /// *** load_area_id_

        /// *** string load_space_id_ = "fff";
        vector<string> load_space_ids_;
        /// *** double load_latitude_ = 0;
        /// *** double load_longitude_ = 0;
        /// *** double load_azimuth_ = 0;

        /// params about unloadPlaceScan 2.1.11
        /// *** vin
        string start_time_ = ""; ////ptime t(time_from_string(ts))
        string end_time_ = "";

        SingletonAuxiliaryClient();

        void initialize();
    
        string readCode(const string &_body);

        string readValueByKey(const string &_body, const string &_key);
    
        string readReturnDataValueByKey(const string &_body, const string &_key);

        string readReturnDataValueByKeys(const string &_body, const vector<string> &_keys);

        vector<string> readReturnDataValuesByKey(const string &_body, const string &_key);
        bool ResponseBodyValid(const string &_body);

        string http_post(const string &url, const string &request_body);

        bool getPjh(simdjson::ParsedJson::Iterator &pjh, const string &_content);
    };
}

