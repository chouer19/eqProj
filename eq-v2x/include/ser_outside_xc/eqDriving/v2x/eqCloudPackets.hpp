//======================================================================aaa
/*! \file eqCloudPackets.hpp
 *
 * \copydoc Copyright
 * \author Xue Chong(xc)
 * \date July 29, 2019
 *
 * packets definition between cloud and endpoint
 *///-------------------------------------------------------------------aaa
#pragma once

#include <boost/endian/arithmetic.hpp>
#include <iostream>
#include <vector>
#include "ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
#include "pub_types.hpp"
using namespace std;
using namespace boost::endian;

#define MAXIMUM_PACKET_PERIODS 50

#define START_VEHICLE_PACKETS 0x01
#define START_SERVER_PACKETS 0x80

////  message defination
typedef enum{
    vehicle_ego = 0x01, // yes for now
    motors,
    battery,
    engine,  // yes for now
    vehicle_pos,  // yes
    extreme, // jizhi 
    error,  // yes
    chargable_equipment_voltage,
    chargable_equipment_temprature,
    vehicle_ego_appendix,  // yes
    battery_management_system,
}report_message_type;

typedef struct{
    uint8_t id = 0;                    //0x01
    big_uint16_t data_length = 0;      // should be 39, 0x0027 - 2 = 0x0025
    uint8_t start_status = 0;          // 0x01
    uint8_t power_source = 0;          // 0x03
    big_uint16_t speed = 0;            // 250, 0x00fa
    big_uint32_t odometer = 0;         // 35670, 0x00008b56
    uint8_t gear = 0;                  //0x25, has motion, five gear
    uint8_t key_status = 0;            //0x03, START
    uint8_t compartments_status = 0;   //0x01, up
    uint8_t compartments_angle = 0;    // 0x33, 51
    uint8_t work_status = 0;           //  0x02, burden
    uint8_t task_id[20] = {0};           // abcdefghijklmnopqrst
    uint8_t task_status = 0;           //0x02
    uint8_t driving_mode = 0;          // 0x02, auto driving
    uint8_t task_type = 0;             // 0x03
    big_uint16_t limit_speed = 0;             // 0x03
    uint8_t reserved = 0;              // 0x00
}vehicle_ego_message;

typedef struct{
    uint8_t id = 0;                    // 0x04
    big_uint16_t data_length = 0;      // should be 8, 0x08 - 2 = 6
    uint8_t status = 0;                // 0x01
    big_uint16_t rotate_speed = 0;     //55555, 0xd903
    big_uint16_t consumption = 0;      //22222, 0x56ce
    uint8_t torque_propotion = 0;      // 0x32, 50
}engine_message;

typedef struct{
    uint8_t id = 0;                    // 0x05
    big_uint16_t data_length = 0;      // should be 39, 0x27 - 2 = 0x0025
    uint8_t status = 0;                // 0x01
    big_uint64_t lontitude = 0;        // 112.333752898  0x0000001a279d0a42 
    big_uint64_t latitude = 0;         // 40.008645625   0x0000000950b37bf9
    big_uint32_t height = 0;           // 11280000       0x00ac1e80
    big_uint16_t yaw = 0;              // 200.00         0x4e20
    big_uint16_t pitch = 0;            // 0.50           0x32
    big_uint16_t roll = 0;             // 30.00          0x0bb8
    uint8_t satellite_num = 0;         // 11, 0x0b
    uint8_t dual_atenna_status = 0;    // 0x03
    uint8_t rtk_fixed_status = 0;      // 0x01
    big_uint16_t position_system_error = 0; // 0x00
    uint8_t voltage_status = 0;          // 0x02, high voltage
    uint8_t angle_speed_exceed = 0;      //0x03
    uint8_t magnetic_exceed;        // 0x00
    uint8_t temprature_exceed = 0;       // 0x01
    uint8_t calibration_result = 0;       // 0x01
}vehicle_pos_message;

typedef struct{
    uint8_t id = 0;                      //0x07 
    big_uint16_t data_length = 0;        // should be 19, 0x0013 - 2 = 0x0011
    uint8_t max_error_level = 0;         //  0x03
    big_uint64_t error_tag = 0;          // 0x0202030305050606
    big_uint64_t reserved = 0;           // 0x0000000000000000
}error_message;

typedef struct{
    uint8_t id = 0;                               // 0x0a
    big_uint16_t data_length = 0;                 // should be 31+10+10 = 51, 0x33 - 2 = 0x0031
    uint8_t left_front_door = 0;                  // 0x00
    uint8_t right_front_door = 0;                 // 0x00
    uint8_t left_front_door_lock = 0;                 // 0x00
    uint8_t right_front_door_lock = 0;                 // 0x00
    uint8_t steal_alert = 0;                 // 0x00
    uint8_t frost_fog = 0;                 // 0x00
    uint8_t reverse = 0;                 // 0x00
    uint8_t seat_belt = 0;                 // 0x00
    uint8_t wiper = 0;                 // 0x01
    uint8_t emergency_double_light_input = 0; // 0x01
    uint8_t left_front_turn_light = 0;   // 0x00
    uint8_t right_front_turn_light = 0;  //0x00
    uint8_t low_beam = 0;             //0x02
    uint8_t high_beam = 0;             // 0x01
    uint8_t rotate_light_alert = 0;      //0x01
    uint8_t air_horn = 0;            // 0x00
    uint8_t front_fog_light = 0;           // 0x00
    uint8_t front_work_light = 0;        // 0x00
    uint8_t left_back_turn_light = 0;   //0x00
    uint8_t right_back_turn_light = 0;   // 0x00
    uint8_t front_pos_light = 0;    // 0x00
    uint8_t back_pos_light = 0;    // 0x00
    uint8_t brake_light = 0;         // 0x00
    uint8_t reverse_light = 0;        // 0x01
    uint8_t back_work_light = 0;    // 0x01
    uint8_t ptc = 0;           // 0x01
    uint8_t air = 0;              // 0x01
    big_uint16_t oil_left = 0;           //50000, 0xc350
    uint8_t tire_pressure[10]; // 100,110... 0x64 6e 78 82 8c 96 a0 aa b4 be
    uint8_t tire_temprature[10];  // 100,110... 0x64 6e 78 82 8c 96 a0 aa b4 be
}vehicle_ego_appendix_message;

/// message report packet
typedef struct{
    vehicle_ego_message vehicle_ego_data;
    vehicle_ego_appendix_message appendix_data;
    error_message error_data;
    vehicle_pos_message pos_data;
    engine_message engine_data;
}message_report_data_t;
typedef struct{
    uint8_t time[8];
    uint8_t bytes[1];
}message_report_packet_t;
void reset_message_report_data(message_report_data_t *message_report_data);

/// log out packet
typedef struct{
    uint8_t time[8];
}vehicle_logout_packet_t;

/// rap packet
typedef struct{
}rap;

/// calibration time packet
typedef struct{
    uint8_t time[8];
}time_calibration_packet_t; //
int decode_calibration_time(uint8_t *time, an_packet_t *an_packet);

/// map request packet ?????????????
typedef struct{
    uint8_t time[8];
}map_request_packet_t; //

typedef struct{
    double x = 0;
    double y = 0;
    double z = 0;
    double c = 0;
}map_road_point;
typedef struct{
    big_uint32_t data_length = 0;
    uint8_t id[36];
    uint8_t type = 0;
    uint8_t format = 0;
    big_uint32_t number = 0;
    big_uint32_t real_number = 0;
    vector<map_road_point> points;
}map_road;

struct map_data_packet_t
{
    map_data_packet_t()
        :count(0)
        ,number(0)
        ,real_number(0)
    { for(int i=0;i<23;i++) { version[i]=0xff; } }

    big_uint16_t count;
    uint8_t time[8];
    uint8_t version[23];
    uint8_t number;
    uint8_t real_number;
    vector<map_road> roads;
};

int decode_map_data_packet(map_data_packet_t *map_data_packet, an_packet_t *an_packet);

/// assignment request  packet
typedef struct{
    uint8_t task_type = 0;
}assignment_request_packet_t; //

///  end check packet
typedef struct{
    uint8_t code[20];
}check_packet_t; // zhong duan jianquan

///  gis update packet
typedef struct{
    uint8_t time[8];
    uint8_t area = 0;
    uint8_t lidar_pos = 0;
    uint8_t lidar_type = 0;
    big_uint16_t data[1];
}gis_update; //

/// params enumeration
typedef enum{
    report_cycle=0x01,
    domain_length,
    domain,
    cloud_port,
    software_version,
    hardware_version,
    vehicle_vin
}param_e;


typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t number = 0;
    uint8_t real_number = 0;
    vector<uint8_t> ids;
}params_inquiry_packet_t;
int decode_params_inquiry_packet(params_inquiry_packet_t *params_inquiry_packet, an_packet_t *an_packet);

/// params config packet, down
typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t number = 0;
    uint8_t real_number = 0;
    vector<big_uint16_t> report_cycle;
    vector<uint8_t> domain_length;
    vector<uint8_t> domain_params;
    vector<big_uint16_t> cloud_port;
    vector<uint8_t> software_version;
    vector<uint8_t> hardware_version;
    vector<uint8_t> vehicle_vin;
}params_config_packet_t;
int decode_params_config_packet(params_config_packet_t *params_config_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    vector<uint8_t> url;
}upgrade_packet_t;

int decode_upgrade_packet(upgrade_packet_t *upgrade_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t start = 0;
    uint8_t redundancy = 0;
}engine_packet_t;
int decode_engine_packet(engine_packet_t *engine_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    double speed = 100;
    bool is_new = true;
}restrict_speed_packet_t;
int decode_restrict_speed_packet(restrict_speed_packet_t *restrict_speed_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t estop = 0x01;
    bool is_new = true;
}estop_packet_t;
int decode_estop_packet(estop_packet_t *estop_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t auto_avoid = 0x01;
    bool is_new = true;
}auto_avoid_packet_t;
int decode_auto_avoid_packet(auto_avoid_packet_t *auto_avoid_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t take_over = 0;
    uint8_t redundancy = 0;
}take_over_packet_t;
int decode_take_over_packet(take_over_packet_t *take_over_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t gas_brake = 0;
    uint8_t hydraulic_retarder = 0;
    uint8_t pedal_percent = 0;
    big_uint16_t brake_acc = 0;
    big_uint16_t hand_wheel;
    uint8_t push_up = 0;
    //uint16_t ego_vehicle;
    //big_uint16_t ego_vehicle;
    uint8_t ego_vehicle[2];
    uint8_t gear = 0;
    uint8_t gear_limit = 0;
    uint8_t hand_brake = 0;
}take_over_cmd_packet_t;
int decode_take_over_cmd_packet(take_over_cmd_packet_t *take_over_cmd_packet, an_packet_t *an_packet);

typedef struct{
    uint8_t jianquan[20];
    uint8_t invalid_time[8];
}login_respond_packet_t;
int decode_login_respond_packet(login_respond_packet_t *login_respond_packet, an_packet_t *an_packet);


typedef struct{
    double latitude = 0;
    double lontitude = 0;
    double height = 0;
    uint8_t limit_speed = 0;
    double heading = 0;
    double coefficient = 0;
}trajectory_point;

typedef struct{
    big_uint32_t data_length = 0;
    uint8_t serial_number = 0;
    //uint8_t id[17];
    uint8_t id[20];
    uint8_t type = 0;
    uint8_t gear = 0;
    uint8_t format = 0;
    big_uint32_t point_number = 0;
    big_uint32_t real_point_number = 0;
    vector<trajectory_point> points;
}task;

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t number = 0;
    uint8_t real_number = 0;
    vector<task> tasks;
}assignment_packet_t;
int decode_assignment_packet(assignment_packet_t *assignment_packet, an_packet_t *an_packet);

typedef struct{
    uint16_t count = 0;
    uint8_t time[8];
    uint8_t type = 0;
    uint8_t number = 0;
    uint8_t real_number = 0;
    vector<task> tasks;
}assignment_ajust_packet_t;
int decode_assignment_ajust_packet(assignment_ajust_packet_t *assignment_ajust_packet, an_packet_t *an_packet);


typedef struct{
    uint8_t time[8];
    uint8_t type = 0;
    vector<uint8_t> content;
}information_packet_t;

typedef struct
{
    bool new_message_flag = false;
    information_packet_t message_packet;
}cloud_info;

int decode_information_packet(information_packet_t *information_packet, an_packet_t *an_packet);

typedef struct{
    uint8_t time[8];
    big_uint16_t data_length = 0;
    vector<uint8_t> data;
    bool is_new = true;
}rtcm_packet_t;
int decode_rtcm_packet(rtcm_packet_t *rtcm_packet, an_packet_t *an_packet);

typedef struct{
    uint8_t id[10];
    double x = 0;
    double y = 0;
    double z = 0;
    uint8_t speed = 0;
    double heading = 0;
    uint8_t type = 0;
    uint8_t length = 0;
    uint8_t width = 0;
    uint8_t height = 0;
}one_vehicle;

typedef struct{
    bool is_new = true;
    uint8_t time[8];
    uint8_t number = 0;
    vector<one_vehicle> vehicles;
}surrd_vehicle_packet_t;

int decode_surrd_packet(surrd_vehicle_packet_t *surrd_vehicle_packet, an_packet_t *an_packet);

/// vehicle log in packet
typedef struct{
    uint8_t time[8];
    uint8_t token[20];
    uint8_t reserved[2];
}vehicle_login_data;

struct telecom_vehicle_ego_message
{
    telecom_vehicle_ego_message()
        :message_valid(false)
    {}

    bool message_valid;
    vehicle_ego_message vem;
};

struct telecom_engine_message
{
    telecom_engine_message()
        :message_valid(false)
    {}

    bool message_valid;
    engine_message em;
};

struct telecom_vehicle_pos_message
{
    telecom_vehicle_pos_message()
        :message_valid(false)
    {}

    bool message_valid;
    vehicle_pos_message vpm;
};

struct telecom_error_message
{
    telecom_error_message()
        :message_valid(false)
    {}

    bool message_valid;
    error_message em;
};

struct telecom_vehicle_ego_appendix_message
{
    telecom_vehicle_ego_appendix_message()
        :message_valid(false)
    {}
    bool message_valid;
    vehicle_ego_appendix_message veam;
};

//struct sample_gis_packet_t{
//    uint8_t time[8];
//    uint8_t type = 0;
//    big_uint16_t duration = 0;
//    uint8_t area = 0;
//    bool is_new = true;   
//};
int decode_sample_gis_packet(sample_gis_packet_t *sample_gis_packet, an_packet_t *an_packet);


///////////////////////////http/////////////////////////
        struct login_respond_t{                                                
            string code = "NULL";
            string vehicle_number = "NULL";
            vector<string> vehicle_numbers;
            string driver_id = "NULL";                                         
            string driver_name = "NULL";
            string platform_id = "NULL";
            string load_area_id = "NULL";
            string item_type = "NULL";
        }; 

struct getRequestStatus_respond_t{
    string code = "NULL";
    utc_time operate_time;
    utc_time examine_time;
    string request_type = "NULL";
    string result = "NULL";
};

struct confirmLoadPlace_respond_t{
    string code = "NULL";
    string load_space_id = "NULL";
};
struct getLoadPlace_respond_t{
    string code = "NULL";
    string load_space_id = "NULL";
    vector<string> load_space_ids;
    double latitude = 0;
    double longitude = 0;
    double azimuth = 0;
};
