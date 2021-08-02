//======================================================================
/* ! \file eqSingletonClient.hpp
 *
 * \copydoc Copyright
 * \author Xue Chong(xc)
 * \date July 29, 2019
 *
 * a singleton class for client
 * read is synchronous, keep listening
 * write is asynchronous
 * Usage:  
 * client for connectint cloud, send and receive message
 *///-------------------------------------------------------------------
 
#include <stdlib.h>                                                
#include <stdint.h>                                                
#include <string.h> 
#include <chrono>
#include <memory>
#include <deque>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/date_time/gregorian/gregorian.hpp> 

#include "header/system/ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
#include "header/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"
#include "header/system/pub_types.h"
 
using namespace std;
using namespace boost::asio;
using namespace std::chrono;
using namespace boost::endian;

class SingletonClient {
public:
    /// an instance
    static SingletonClient *getInstance() {
        if (instance == 0){
             instance = new SingletonClient;
        }   
        return instance;
    }

    //uint8_t * current_time();
    vector<uint8_t> test_current_time();

    vector<uint8_t> current_time();

    void make_header(an_packet_t * packet, uint8_t cmd_id, uint8_t request_id, uint8_t encryption);

    void make_header_t(std::shared_ptr<an_packet_t> packet, uint8_t cmd_id, uint8_t request_id, uint8_t encryption);
    
    // 0x01
    //// ????????? add time params
    //// local var for  token
    //// keep login if jianquan failed
    void login();
    void test_login();

    size_t delay_counter = 0;
    void check_delay_counter();
    
    //// defian a buffer and a file
    void bufa_message();
    
    // 0x02
    void report_message();
    void report_message(vehicle_ego_message ego, engine_message engine, vehicle_pos_message pos, error_message error, vehicle_ego_appendix_message appendix);
    void report_message(telecom_vehicle_ego_message ego, telecom_engine_message engine, telecom_vehicle_pos_message pos, telecom_error_message error, telecom_vehicle_ego_appendix_message appendix);

    template <class T>
    void report_message( std::initializer_list<T> list ){
        size_t length = 0;
        for( auto elem : list ){ length += sizeof(elem);}
        an_packet_t *packet;
        packet = an_packet_allocate(8 + length);
        make_header(packet, 0x01, 0xfe, 0x01);
        length = 0;
        uint8_t * time = current_time();                                       
        memcpy(&packet->data[length], time, 8); length += 8;
        for( auto elem : list ){
            memcpy(&packet->data[length], &elem, sizeof(elem)); 
            length += sizeof(elem);
        }
        packet->data_length = length;
        an_packet_encode(packet);
        uint8_t *bytes = (uint8_t *) packet;
        an_packet_free(&packet);
    }
    
    template<typename T, typename... Args>
    void func(T t, Args... args) // recursive variadic function
    {
        std::cout << t <<std::endl ;
        func(args...) ;
    }

    template <class T>
    void func2( std::initializer_list<T> list )
    {
        for( auto elem : list ) {
            std::cout << elem << std::endl ;
        }
    }

    // 0x04
    void logout();
    
    // 0x05
    void jianquan();

    void test();
    
    // 0x06
    void update_gis();
    void update_gis(GIS_UPDATE_PACKET lidar_data);
    void update_gis(GIS_VECTOR_PACKET lidar_data);
    
    // 0x07
    void rap();
    
    // 0x08
    void calibrate_time();
    
    void get_map(int id);
    
    //0x09
    void get_map();
    
    //0x0a
    void request_assignment();
    /// 1013
    void request_assignment(const uint8_t &_type);
    bool assignment_request_ok();
    
    //0x0b
    //// 1013
    void report_rtcm();
    void report_rtcm(uint8_t *data, big_uint16_t length);
    void rtcm(const rtcm_packet_t &_value);
    rtcm_packet_t rtcm();

    ///void get_rtcm(vector<uint8_t> &rtcm);
    vector<uint8_t> get_rtcm();
    //vector<uint8_t> get_rtcm();
    //vector<task_header> task_headers;
    task_db_t get_task();
    void set_task(const task_db_t &_value);
    string taskName();
    void taskName(const string &_value);

    void print_params_inquiry_packet();

    // 0x80
    // respond to params inquiry
    void respond_params_inquiry();

    void print_params_config_packet();
    // 0x81
    // respond to params config
    void respond_params_config();

    // print upgrade control
    void print_upgrade_packet();
    // print engine control
    void print_engine_packet();

    void print_take_over_packet();
    void print_take_over_cmd_packet();
    // 0x82
    // respond to remote control 
    void respond_upgrade_control();
    void respond_engine_control();
    void respond_take_over_control();
    void respond_take_over_cmd_control();

    void print_map_data();

    void print_assignment_packet();
    void print_assignment_ajust_packet();
    // 0x83 does not need respond
    // 0x84
    // respond to assingment
    void respond_assignment();

    void respond_assignment_ajust();

    // 0x85 does not neet respond
    // but print
    void print_information_packet();

    // 0x86 does not neet respond
    // but print
    void print_rtcm_packet();
    // 0x87 does not neet respond
    // but print
    void print_sorrd_packet();

    // process raw bytes data when read any data from server
    void process_data(int bytes_received);

    void update_time(utc_time time);
    bool read_ok();
    bool map_ok();
    void map_ok(const bool &_value);
    void map_data(const map_data_packet_t &_value);
    map_data_packet_t read_map_boundary();
    void read_thread();
    void run_thread();

    void write_thread();

//////////////////read member //////////////

    void respond_thread();

    void respond_ontime(const uint8_t *time);
    void respond_ontime();

    /// 1013
    surrd_vehicle_packet_t surround_vehicles();
    void surround_vehicles(surrd_vehicle_packet_t _surrd_vehicles);

    /// 1013
    void sample_gis_cmd(sample_gis_packet_t tmp_packet);
    sample_gis_packet_t sample_gis_cmd();
    sample_gis_packet_t sample_gis_cmd_();

    /// 1013
    void restrict_speed_packet_(const restrict_speed_packet_t &value);
    restrict_speed_packet_t restrict_speed_packet_();
    void respond_restrict_speed();
    double restrict_speed();

    estop_packet_t estop();
    void estop_packet_(const estop_packet_t &value);
    estop_packet_t estop_packet_();
    void respond_estop();

    void auto_avoid_packet_(const auto_avoid_packet_t &value);
    bool auto_avoid();
    auto_avoid_packet_t auto_avoid_packet_();
    void respond_auto_avoid();

    bool has_new_assignment();
    assignment_packet_t & get_assignment_packet() ;
    
    bool has_new_assignment_ajust();
    assignment_ajust_packet_t & get_assignment_ajust_packet() ;

    bool jianquan_valid();
    void jianquanCode(const login_respond_packet_t &_tmp_packet);
    void jianquanCode(uint8_t *jianquan);
    void jianquanClean();
    void jianquan_valid(const bool &_value);

    void config_packet(const params_config_packet_t &_tmp_packet);

    uint8_t full_load();

    void stop();

    string get_vin();

    cloud_info get_info();

    ~SingletonClient();

    void Run(){ this->m_io.reset();this->m_io.run();}
    void connectT(const std::string& host, const std::string& service);
    void start_connect();

private:
    static SingletonClient *instance;
    io_service m_io;
    boost::asio::io_service::strand m_strand;
    ip::tcp::endpoint m_ep;
    ip::tcp::resolver::iterator m_iter;
    ip::tcp::resolver m_resolver;
    
    boost::shared_ptr<ip::tcp::socket> m_sock_ptr;

    utc_time gps_time;

    //uint8_t jianquan_code2[25] = "01234567890123456789";
    uint8_t ontime[8];
    uint8_t *jianquan_code;
    deque<uint8_t> bytes_pool;
    deque<vector<uint8_t>> gis_pool;
    deque<vector<uint8_t>> ontime_pool;
    deque<vector<uint8_t>> rap_pool;
    deque<vector<uint8_t>> respond_pool;
    deque<vector<uint8_t>> bufa_pool;

    deque<vector<uint8_t>> other_pool;
    deque<vector<uint8_t>> getOtherPool();
    void setOtherPool(deque<vector<uint8_t>> _other_pool);

    deque<vector<uint8_t>> packets_pool;

    void push_to_rap(const uint8_t *bytes, int length);
    bool pop_left_rap(vector<uint8_t> &bytes_now);
    bool pop_left_rap(vector<uint8_t> &bytes_now, int &count);

    void push_to_gis(const uint8_t *bytes, int length);
    bool pop_left_gis( vector<uint8_t> &bytes_now);
    bool pop_left_gis( vector<uint8_t> &bytes_now, int &count);

    void push_to_ontime(const uint8_t *bytes, int length);
    bool pop_left_ontime(vector<uint8_t> &bytes_now);
    bool pop_left_ontime(vector<uint8_t> &bytes_now, int &count);

    void push_to_respond(const uint8_t *bytes, int length);
    bool pop_left_respond(vector<uint8_t> &bytes_now);
    bool pop_left_respond(vector<uint8_t> &bytes_now, int &count);

    void push_to_other(const uint8_t *bytes, int length);
    bool pop_left_other(vector<uint8_t> &bytes_now);
    bool pop_left_other(vector<uint8_t> &bytes_now, int &count);

    void push_to_bufa(vector<uint8_t> bytes);
    bool pop_left_bufa(vector<uint8_t> &bytes_now, int &count);

    bool to_jianquan = true;
    bool to_respond_params_inquiry = false;
    bool to_respond_params_config = false;
    bool to_respond_assignment = false;
    bool to_respond_restrict = false;
    bool assignment_new = false; // has_assigment
    bool to_respond_assignment_ajust = false;
    bool assignment_ajust_new = false; // has_assigment
    bool server_got_assignment_request = false;

    int data;
    int write_count;
    int read_count;
    bool running;

    //============== packet define ==================
    an_decoder_t an_decoder;
    params_inquiry_packet_t  params_inquiry_packet;
    params_config_packet_t  params_config_packet;
    upgrade_packet_t  upgrade_packet;
    engine_packet_t  engine_packet;
    restrict_speed_packet_t restrict_speed_packet;
    auto_avoid_packet_t auto_avoid_packet;
    estop_packet_t estop_packet;
    take_over_packet_t  take_over_packet;
    take_over_cmd_packet_t  take_over_cmd_packet;
    login_respond_packet_t login_respond_packet;
    assignment_packet_t assignment_packet;
    cloud_info cloud_info_;
    sample_gis_packet_t  sample_gis_packet;
    ///task
    //vector<string> task_ids;
    //vector<task_header> task_headers;
    task_db_t task_db;
    string task_name;

    assignment_ajust_packet_t assignment_ajust_packet;
    information_packet_t information_packet;
    rtcm_packet_t rtcm_packet;
    bool received_rtcm = false;
    surrd_vehicle_packet_t surrd_vehicle_packet;
    map_data_packet_t map_data_packet;
    bool map_ready_ = false;

    message_report_data_t message_report_data;

    bool received_0x84 = false;
    uint8_t calibration_time[8];
    bool jianquan_result = false;

    boost::mutex mutex;

    void reconnect_server();
    bool connecting = false;
    bool connected = false;
    bool resolved = false;
    bool resolving = false;
    bool Resolved();
    void Resolved(const bool &_value);
    bool Connected();
    void Connected(const bool &_value);
    bool Connecting();
    void Connecting(const bool &_value);
    bool Resolving();
    void Resolving(const bool &_value);
    void resolve_address();

    void conn_handler(const boost::system::error_code&ec);

    void initialize();

    SingletonClient();

    void write_task();

    string vin = "12345678901234567";
    string token = "23412309900234324";
    string mapV = "999.999.999.999";
    string softV = "999.999.999.999";
    string hardV = "999.999.999.999";
    int cycle = 1;
    string domain = "12345678901234567";
    string port = "1234";
    bool read_params = false;
    bool load_params();
    static int params_callback(void *pVoid, int argc, char **argv, char **azColName);

};
