//================================================================aaa
/* ! \file eqSingletonClient.cpp

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
 *///-------------------------------------------------------------aaa
 
#include <sstream>
#include <fstream>
#include <stdlib.h>                                                
#include <stdint.h>                                                
#include <string.h> 
#include <iomanip>
#include <chrono>
#include <memory>
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
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/write.hpp>
#include <cstdlib>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
 
#include "header/system/ser_outside_xc/eqDriving/util/Logger.hpp"
#include "header/system/ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
#include "header/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"
#include "header/system/ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp"

#include "header/system/ser_outside_xc/thirdParty/sqlite/sqlite3.h"
#include "header/system/ser_outside_xc/thirdParty/gzip/compress.hpp"
#include "header/system/ser_outside_xc/thirdParty/gzip/config.hpp"
#include "header/system/ser_outside_xc/thirdParty/gzip/decompress.hpp"
#include "header/system/ser_outside_xc/thirdParty/gzip/utils.hpp"
#include "header/system/ser_outside_xc/thirdParty/gzip/version.hpp"

using namespace std;
using namespace eqDriving;
using namespace util;
using namespace boost::asio;
using namespace std::chrono;
using namespace boost::endian;

void SingletonClient::check_delay_counter(){
    while(true){
        delay_counter = 0;
        boost::this_thread::sleep_for(boost::chrono::seconds{10});
        if(delay_counter ==0){
            this->Connected(false);
        }
    }

}

vector<uint8_t> SingletonClient::test_current_time(){
    vector<uint8_t> time; for(int i=0;i<8;i++){time.push_back(0);}
    system_clock::time_point now = system_clock::now();
    system_clock::duration tp = now.time_since_epoch(); 
    time_t tt = system_clock::to_time_t(now);
    tm utc_tm = *gmtime(&tt);
    tm local_tm = *localtime(&tt);
    time[0] = uint8_t (local_tm.tm_year + 1900 - 2000);
    time[1] = uint8_t (local_tm.tm_mon + 1);
    time[2] = uint8_t (local_tm.tm_mday);
    time[3] = uint8_t (local_tm.tm_hour);
    time[4] = uint8_t (local_tm.tm_min);
    time[5] = uint8_t (local_tm.tm_sec);
    milliseconds ms = duration_cast<milliseconds>(tp);
    big_uint16_t millisec = ms.count() % 1000;
    memcpy(&time[6], &millisec,2);
    return time;
}
vector<uint8_t> SingletonClient::current_time(){
    vector<uint8_t> time;time.resize(8);
    boost::lock_guard<boost::mutex> lock{mutex};
    time[0] = gps_time.year - 2000;
    time[1] = gps_time.month;
    time[2] = gps_time.day;
    time[3] = gps_time.hour;
    time[4] = gps_time.minute;
    time[5] = gps_time.second;
    big_uint16_t millisec = gps_time.millisecond;
    memcpy(&time.data()[6], &millisec,2);
    return time;
}

void SingletonClient::push_to_rap(const uint8_t *bytes, int length){
    vector<uint8_t> bytes_new;
    for(int i=0;i<length;i++){bytes_new.push_back(bytes[i]);}
    boost::lock_guard<boost::mutex> lock{mutex};
    rap_pool.push_back(bytes_new);
}
bool SingletonClient::pop_left_rap(vector<uint8_t> &bytes_now, int &count){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(rap_pool.size()>0)){return false;}
    bytes_now.clear();
    bytes_now = rap_pool.front();
    count -= bytes_now.size();
    rap_pool.pop_front();
    return true;
}
bool SingletonClient::pop_left_rap(vector<uint8_t> &bytes_now){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(rap_pool.size()>0)){return false;}
    for(int i=0; i< rap_pool.front().size();i++){
        bytes_now.push_back(rap_pool.front()[i]);
    }
    rap_pool.pop_front();
    return true;
}
void SingletonClient::push_to_gis(const uint8_t *bytes, int length){
    vector<uint8_t> bytes_new;
    for(int i=0;i<length;i++){bytes_new.push_back(bytes[i]);}
    boost::lock_guard<boost::mutex> lock{mutex};
    gis_pool.push_back(bytes_new);
}
bool SingletonClient::pop_left_gis(vector<uint8_t> &bytes_now){
    vector<uint8_t> vvv;
    {
        boost::lock_guard<boost::mutex> lock{mutex};
        if(!(gis_pool.size()>0)){return false;}
        vvv = gis_pool.front();
        gis_pool.pop_front();
    }
    for(int i=0; i< vvv.size();i++){
        bytes_now.push_back(vvv[i]);
    }
    return true;
}
bool SingletonClient::pop_left_gis(vector<uint8_t> &bytes_now, int &count){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(gis_pool.size()>0)){return false;}
    bytes_now.clear();
    bytes_now = gis_pool.front();
    count -= bytes_now.size();
    gis_pool.pop_front();
    return true;
}
void SingletonClient::push_to_bufa(const vector<uint8_t> bytes){
    boost::lock_guard<boost::mutex> lock{mutex};
    bufa_pool.push_back(bytes);
}
bool SingletonClient::pop_left_bufa(vector<uint8_t> &bytes_now, int &count){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(bufa_pool.size()>0)){return false;}
    bytes_now.clear();
    bytes_now = bufa_pool.front();
    bufa_pool.pop_front();
    if(bytes_now.size() < AN_PACKET_HEADER_SIZE){return false;}
    bytes_now[2] = 0x03;
    bytes_encode(bytes_now);
    count -= bytes_now.size();
    return true;
}
void SingletonClient::push_to_ontime(const uint8_t *bytes, int length){
    vector<uint8_t> bytes_new;
    for(int i=0;i<length;i++){bytes_new.push_back(bytes[i]);}
    boost::lock_guard<boost::mutex> lock{mutex};
    ontime_pool.push_back(bytes_new);
}
bool SingletonClient::pop_left_ontime(vector<uint8_t> &bytes_now, int &count){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(ontime_pool.size()>0)){return false;}
    bytes_now.clear();
    bytes_now = ontime_pool.front();
    count -= bytes_now.size();
    ontime_pool.pop_front();
    return true;
}
bool SingletonClient::pop_left_ontime(vector<uint8_t> &bytes_now){
    vector<uint8_t> vvv;
    {
        boost::lock_guard<boost::mutex> lock{mutex};
        if(!(ontime_pool.size()>0)){return false;}
        vvv = ontime_pool.front();
        ontime_pool.pop_front();
    }
    for(int i=0; i< vvv.size();i++){
        bytes_now.push_back(vvv[i]);
    }
    return true;
}
void SingletonClient::push_to_respond(const uint8_t *bytes, int length){
    vector<uint8_t> bytes_new;
    for(int i=0;i<length;i++){bytes_new.push_back(bytes[i]);}
    boost::lock_guard<boost::mutex> lock{mutex};
    respond_pool.push_back(bytes_new);
}
bool SingletonClient::pop_left_respond(vector<uint8_t> &bytes_now){
    boost::lock_guard<boost::mutex> lock{mutex};
    if((!respond_pool.size()>0)){return false;}
    for(int i=0; i< respond_pool.front().size();i++){
        bytes_now.push_back(respond_pool.front()[i]);
    }
    respond_pool.pop_front();
    return true;
}
void SingletonClient::push_to_other(const uint8_t *bytes, int length){
    vector<uint8_t> bytes_new;
    for(int i=0;i<length;i++){bytes_new.push_back(bytes[i]);}
    {
        boost::lock_guard<boost::mutex> lock{mutex};
        other_pool.push_back(bytes_new);
    }
}

bool SingletonClient::pop_left_other(vector<uint8_t> &bytes_now, int &count){
    boost::lock_guard<boost::mutex> lock{mutex};
    if(!(other_pool.size()>0)){return false;}
    bytes_now.clear();
    bytes_now = other_pool.front();
    count -= bytes_now.size();
    other_pool.pop_front();
    return true;
}
bool SingletonClient::pop_left_other(vector<uint8_t> &bytes_now){
    vector<uint8_t> vvv;
    {
        boost::lock_guard<boost::mutex> lock{mutex};
        if(!(other_pool.size()>0)){return false;}
        vvv = other_pool.front();
        other_pool.pop_front();
    }
    for(int i=0; i< vvv.size();i++){
        bytes_now.push_back(vvv[i]);
    }
    return true;
}

void SingletonClient::write_thread(){
    size_t logincount = 0;
    while(running){
        int mscount = 100;
        boost::this_thread::sleep_for(boost::chrono::milliseconds{mscount});
        vector<uint8_t> bytes_send;
        int count = 3000;
        while(count > 0 && pop_left_other(bytes_send, count)){
            if(!this->Connected()) {continue;}
            boost::system::error_code write_ec;
            m_sock_ptr->write_some(buffer(bytes_send.data(), bytes_send.size()), write_ec);
            if(write_ec){ 
                //this->Connected(false);
                Logger::Instance()->Writeln("ERROR: write other packet!");
                Logger::Instance()->Writeln(write_ec.message());
                cout << "write error !\n";
                cout << write_ec.message() << endl;
            }
            else{ }
        }

        while(count > 0 && pop_left_ontime(bytes_send, count)){
            if(!this->Connected()) {push_to_bufa(bytes_send);continue;}
            boost::system::error_code write_ec;
            m_sock_ptr->write_some(buffer(bytes_send.data(), bytes_send.size()), write_ec);
            if(write_ec){ 
                //this->Connected(false);
                Logger::Instance()->Writeln("ERROR: write other packet!");
                Logger::Instance()->Writeln(write_ec.message());
                push_to_bufa(bytes_send); 
                cout << "write error !\n";
                cout << write_ec.message() << endl;
            }
        }

        delay_counter++;


        while(count > 0 && pop_left_gis(bytes_send, count)){
            if(!this->Connected()) {continue;}
            boost::system::error_code write_ec;
            m_sock_ptr->write_some(buffer(bytes_send.data(), bytes_send.size()), write_ec);
            if(write_ec){ 
                //start_connect();
                //this->Connected(false);
                Logger::Instance()->Writeln("ERROR: write other packet!");
                Logger::Instance()->Writeln(write_ec.message());
                cout << "write error !\n";
                cout << write_ec.message() << endl;
            }
        }

        while(this->Connected() && count > 0 && pop_left_bufa(bytes_send, count)){
            //if(!this->Connected()) {continue;}
            boost::system::error_code write_ec;
            m_sock_ptr->write_some(buffer(bytes_send.data(), bytes_send.size()), write_ec);
            if(write_ec){ 
                //start_connect();
                //this->Connected(false);
                Logger::Instance()->Writeln("ERROR: write other packet!");
                Logger::Instance()->Writeln(write_ec.message());
                push_to_bufa(bytes_send); 
                cout << "write error !\n";
                cout << write_ec.message() << endl;
            }
        }
    }
    return ;
}

void SingletonClient::make_header(an_packet_t * packet, uint8_t cmd_id, uint8_t request_id, uint8_t encryption){
    //string vin = "LFWADRJF011002346";
    //string vin = "LR83STGT1JBS18033";
    //string vin = "LR83STGT1JBS19083";
    packet->header = 0x2323;
    packet->cmd_id = cmd_id;
    packet->request_id = request_id;
    memcpy(&packet->vehicle_id[0], vin.c_str(), vin.size());
    packet->count = write_count++;
    packet->encryption = 0x01;
}

// 0x01
//// ????????? add time params
//// local var for  token
//// keep login if jianquan failed
void SingletonClient::test_login(){
    if(!this->Connected()){return ;}
    vehicle_login_data login;
    vector<uint8_t> time = test_current_time();
    memcpy(&login.time[0], &time.data()[0], 8);

    an_packet_t *packet;
    packet = an_packet_allocate(sizeof(login));
    make_header(packet, 0x01, 0xfe, 0x01);
    packet->data_length = sizeof(login);

    // Login packet data content
    //string TOKEN = "abcdefghijklmnopqrsz";
    memcpy(&login.token[0], token.c_str(), 20);
    login.reserved[0] = 0x00;
    login.reserved[1] = 0x11;
    memcpy(&packet->data[0], &login, sizeof(login));
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::login(){
    if(!this->Connected()){return ;}
    vehicle_login_data login;
    vector<uint8_t> time = current_time();
    memcpy(&login.time[0], &time.data()[0], 8);

    an_packet_t *packet;
    packet = an_packet_allocate(sizeof(login));
    make_header(packet, 0x01, 0xfe, 0x01);
    packet->data_length = sizeof(login);

    // Login packet data content
    //string TOKEN = "abcdefghijklmnopqrsz";
    memcpy(&login.token[0], token.c_str(), 20);
    login.reserved[0] = 0x00;
    login.reserved[1] = 0x11;
    memcpy(&packet->data[0], &login, sizeof(login));
    
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

//// defian a buffer and a file
void SingletonClient::bufa_message(){
}

// 0x02
void SingletonClient::report_message(){
    uint8_t *bytes = new uint8_t[198]{0x23,0x23,0x02,0x01,0x54,0x4c,0x38,0x35,0x30,0x34,0x33,0x30,0x43,0x4b,0x38,0x35,0x37,0x30,0x30,0x31,0x35,0x07,0xd6,0x01,0x00,0x00,0x00,0xa9,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xff,0xba,0xb9,0x7f,0x00,0x00,0xc1,0xcb,0xff,0xba,0xb9,0x7f,0x00,0x00,0xc0,0xcb,0xff,0xba,0xb9,0x49};
    vector<uint8_t> time = test_current_time();
    big_uint16_t ccc = write_count++;
    memcpy(&bytes[28], &time.data()[0], 8);
    memcpy(&bytes[21], &ccc, 2);
    bytes_encode(bytes, 198);
    push_to_ontime(bytes,198);
    delete [] bytes;
} 
void SingletonClient::report_message(telecom_vehicle_ego_message ego, telecom_engine_message engine, telecom_vehicle_pos_message pos, telecom_error_message error, telecom_vehicle_ego_appendix_message appendix){
    big_uint16_t data_length = 0;
    if(ego.message_valid){data_length += sizeof(ego.vem);}
    if(engine.message_valid){data_length += sizeof(engine.em);}
    if(pos.message_valid){data_length += sizeof(pos.vpm);}
    if(error.message_valid){data_length += sizeof(error.em);}
    if(appendix.message_valid){data_length += sizeof(appendix.veam);}
    an_packet_t *packet;
    packet = an_packet_allocate(8 + data_length);
    make_header(packet, 0x02, 0x01, 0x01);
    data_length = 0;
    vector<uint8_t> time = current_time();
    memcpy(&packet->data[data_length], &time.data()[0], 8);
    data_length += 8;
    if(ego.message_valid){
        memcpy(&packet->data[data_length], &ego.vem, sizeof(ego.vem));
        data_length += sizeof(ego.vem);
    }
    if(engine.message_valid){
        memcpy(&packet->data[data_length], &engine.em, sizeof(engine.em));
        data_length += sizeof(engine.em);
    }
    if(pos.message_valid){
        memcpy(&packet->data[data_length], &pos.vpm , sizeof(pos.vpm));
        data_length += sizeof(pos.vpm);
    }
    if(error.message_valid){
        memcpy(&packet->data[data_length], &error.em , sizeof(error.em));
        data_length += sizeof(error.em);
    }
    if(appendix.message_valid){
        memcpy(&packet->data[data_length], &appendix.veam, sizeof(appendix.veam));
        data_length += sizeof(appendix.veam);
    }
    packet->data_length = data_length;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_ontime(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::report_message(vehicle_ego_message ego, engine_message engine, vehicle_pos_message pos, error_message error, vehicle_ego_appendix_message appendix){
    an_packet_t *packet;
    packet = an_packet_allocate(8 + sizeof(ego) + sizeof(engine) + sizeof(pos) + sizeof(error) + sizeof(appendix));
    make_header(packet, 0x02, 0x01, 0x01);
    big_uint16_t data_length = 0;
    vector<uint8_t> time = current_time();
    memcpy(&packet->data[data_length], &time.data()[0], 8);
    data_length += 8;
    memcpy(&packet->data[data_length], &ego, sizeof(ego));
    data_length += sizeof(ego);
    memcpy(&packet->data[data_length], &engine, sizeof(engine));
    data_length += sizeof(engine);
    memcpy(&packet->data[data_length], &pos , sizeof(pos));
    data_length += sizeof(pos);
    memcpy(&packet->data[data_length], &error , sizeof(error));
    data_length += sizeof(error);
    memcpy(&packet->data[data_length], &appendix, sizeof(appendix));
    data_length += sizeof(appendix);
    packet->data_length = data_length;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_ontime(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

// 0x04
void SingletonClient::logout(){
    //uint8_t *logout_time = current_time();
    vector<uint8_t> logout_time = current_time();
    an_packet_t *packet;
    packet = an_packet_allocate(8);
    make_header(packet, 0x04, 0x01, 0x01);
    packet->data_length = 8;
    memcpy(&packet->data[0], &logout_time.data()[0], 8);
    
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::test(){}

// 0x05
void SingletonClient::jianquan(){
    if(!this->Connected()){return ;}
    an_packet_t *packet;
    packet = an_packet_allocate(20); // length of jianquan code is 20
    make_header(packet, 0x05, 0xfe, 0x01);
    packet->data_length = 20;
    uint8_t tmp_jianquan[20];
    jianquanCode(tmp_jianquan);
    memcpy(&packet->data[0], &tmp_jianquan[0] , 20);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

// 0x06
void SingletonClient::update_gis(){
    an_packet_t *packet;
    //uint8_t *time = current_time();
    vector<uint8_t> time = current_time();
    uint8_t update_area = 0x01;
    uint8_t lidar_pos = 0x03;
    uint8_t lidar_type = 0x02;
    uint8_t lidar_data[3000];
    for(int i=0; i<3000; i++){
        lidar_data[i] = i % 256;
    }
    packet = an_packet_allocate(8 + 1 + 1 + 1 + 3000);
    make_header(packet, 0x06, 0x01, 0x01);
    packet->data_length = 3011;
    memcpy(&packet->data[0], &time.data()[0], 8);
    packet->data[8] = 0x01;
    packet->data[9] = 0x03;
    packet->data[10] = 0x02;
    memcpy(&packet->data[11], lidar_data, 3000);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_gis(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::update_gis(GIS_VECTOR_PACKET lidar_data){
    vector<uint8_t> time = current_time();
    uint8_t *uncompressed_bytes = new uint8_t[8 + 32 + 1 + 4 + 20* lidar_data.points.size()];
    memcpy(&uncompressed_bytes[0], &time.data()[0], 8);
    memcpy(&uncompressed_bytes[8], lidar_data.uuid.c_str(), lidar_data.uuid.size());
    uncompressed_bytes[40] = lidar_data.updata_area;
    big_uint32_t number = lidar_data.point_number;
    memcpy(&uncompressed_bytes[41], &number, 4);
    size_t iterator = 45;
    for(auto point : lidar_data.points){
        big_int64_t int64value = point.x * 1000;
        memcpy(&uncompressed_bytes[iterator], &int64value, 8); iterator += 8;
        int64value = point.y * 1000;
        memcpy(&uncompressed_bytes[iterator], &int64value, 8); iterator += 8;
        big_int32_t int32value = (point.z + 1000) * 10000;
        memcpy(&uncompressed_bytes[iterator], &int32value, 4); iterator += 4;
    }
    //string compressed_lidar_data = gzip::compress((char *)uncompressed_bytes, 8 + 32 + 1 + 4 + 20* lidar_data.points.size());
    //an_packet_t *packet;
    //packet = an_packet_allocate(compressed_lidar_data.size());
    //make_header(packet, 0x06, 0x01, 0x01);
    //packet->data_length = compressed_lidar_data.size();
    //memcpy(&packet->data[0], compressed_lidar_data.c_str(), compressed_lidar_data.size());
    an_packet_t *packet;
    packet = an_packet_allocate(8 + 32 + 1 + 4 + 20* lidar_data.points.size());
    make_header(packet, 0x0C, 0x01, 0x01);
    memcpy(&packet->data[0], &uncompressed_bytes[0], 8 + 32 + 1 + 4 + 20* lidar_data.points.size());
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    //push_to_gis(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    delete []uncompressed_bytes;
    an_packet_free(&packet);
}
void SingletonClient::update_gis(GIS_UPDATE_PACKET lidar_data){
    //vector<uint8_t> time = current_time();
    uint8_t *uncompressed_bytes = new uint8_t[8 + 1 + 32 + 1 + 1 + 1 + 32+lidar_data.lidar_data.size()];
    size_t iterator = 0;
    uncompressed_bytes[iterator] = lidar_data.timestamp.year - 2000;iterator++;
    uncompressed_bytes[iterator] = lidar_data.timestamp.month;iterator++;
    uncompressed_bytes[iterator] = lidar_data.timestamp.day;iterator++;
    uncompressed_bytes[iterator] = lidar_data.timestamp.hour;iterator++;
    uncompressed_bytes[iterator] = lidar_data.timestamp.minute;iterator++;
    uncompressed_bytes[iterator] = lidar_data.timestamp.second;iterator++;
    big_uint16_t buint16value = lidar_data.timestamp.millisecond;
    memcpy(&uncompressed_bytes[iterator], &buint16value, 2);iterator+=2;
    uncompressed_bytes[iterator] = lidar_data.state;iterator++;
    memcpy(&uncompressed_bytes[iterator], lidar_data.uuid.c_str(), lidar_data.uuid.size());iterator+=32;
    uncompressed_bytes[iterator] = lidar_data.updata_area;iterator++;
    uncompressed_bytes[iterator] = lidar_data.sensor_pos;iterator++;
    uncompressed_bytes[iterator] = lidar_data.sensor_type;iterator++;
    //sample_gis_packet_t tmp_packet = sample_gis_cmd_();
    //memcpy(&uncompressed_bytes[iterator], tmp_packet.unload_position, 32);iterator+=32;
    memcpy(&uncompressed_bytes[iterator], lidar_data.unload_position, 32);iterator+=32;

    memcpy(&uncompressed_bytes[iterator], lidar_data.lidar_data.data(), lidar_data.lidar_data.size());
    string compressed_lidar_data = gzip::compress((char *)uncompressed_bytes,lidar_data.lidar_data.size() + iterator);
    an_packet_t *packet;
    packet = an_packet_allocate(compressed_lidar_data.size());
    make_header(packet, 0x06, 0x01, 0x01);
    packet->data_length = compressed_lidar_data.size();
    memcpy(&packet->data[0], compressed_lidar_data.c_str(), compressed_lidar_data.size());
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_gis(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    delete []uncompressed_bytes;
    an_packet_free(&packet);
}

// 0x07
void SingletonClient::rap(){
    if(!this->Connected()){return ;}
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x07, 0x01, 0x01);
    packet->data_length = 0;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    //push_to_rap(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

// 0x08
void SingletonClient::calibrate_time(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x08, 0xfe, 0x01);
    packet->data_length = 0;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::get_map(int id){
    an_packet_t *packet;
    packet = an_packet_allocate(32);
    make_header(packet, 0x09, 0xfe, id);
    packet->data_length = 32;

    // data unit
    uint8_t data[32];
    
    //uint8_t *time = current_time();
    vector<uint8_t> time = current_time();
    memcpy(data, &time.data()[0], 8);

    data[8] = 0x01;// whole map
    //uint8_t version[10] = {'a','b','c','d','e','f','g','h','i','j'};
    //mapV = "1.0.0.0";
    int mapVlen = mapV.size();
    memcpy(&data[9 + 23 - mapV.size()], mapV.c_str(), mapV.size());

    memcpy(&packet->data[0], data, 32);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

//0x09
void SingletonClient::get_map(){
    if(!this->Connected()){return ;}
    an_packet_t *packet;
    packet = an_packet_allocate(32);
    make_header(packet, 0x09, 0xfe, 0x01);
    packet->data_length = 32;

    // data unit
    uint8_t data[32];
    //uint8_t *time = current_time();
    vector<uint8_t> time = current_time();
    memcpy(data, &time.data()[0], 8);

    data[8] = 0x01;// whole map
    //mapV = "1.0.0.0";
    int mapVlen = mapV.size();
    memcpy(&data[9 + 23 - mapV.size()], mapV.c_str(), mapV.size());
    //memcpy(&data[9], mapV.c_str(), 23);
    cout << "get map \n";
    for(int i=0;i<32;i++){ printf("%02x ",data[i]); }
    cout << "\n";

    memcpy(&packet->data[0], data, 32);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

//0x0a
void SingletonClient::request_assignment(const uint8_t &_type){
    an_packet_t *packet;
    packet = an_packet_allocate(1);
    //make_header(packet, 0x0a, 0x01, 0x01);
    make_header(packet, 0x0a, 0xfe, 0x01);
    packet->data_length = 1;
    packet->data[0] = _type;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::request_assignment(){
    an_packet_t *packet;
    packet = an_packet_allocate(1);
    make_header(packet, 0x0a, 0x01, 0x01);
    an_packet_encode(packet);
    packet->data[0] = 0x02;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

vector<uint8_t> SingletonClient::get_rtcm(){
    return rtcm_packet.data;
}
string SingletonClient::taskName(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->task_name;
}
void SingletonClient::taskName(const string &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->task_name = _value;
}
task_db_t SingletonClient::get_task(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return task_db;
}
void SingletonClient::set_task(const task_db_t &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->task_db = _value;
}

void SingletonClient::rtcm(const rtcm_packet_t &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->rtcm_packet = _value;
}

rtcm_packet_t SingletonClient::rtcm(){
    boost::lock_guard<boost::mutex> lock{mutex};
    rtcm_packet_t value = this->rtcm_packet;
    this->rtcm_packet.is_new  = false;
    return value;
}

//0x0b
void SingletonClient::report_rtcm(uint8_t *data, big_uint16_t length){
    an_packet_t *packet;
    vector<uint8_t> time = current_time();
    packet = an_packet_allocate(8 + 2 + length);
    make_header(packet, 0x0b, 0x01, 0x01);
    packet->data_length = 8 + 2 + length ;
    memcpy(&packet->data[0], &time.data()[0], 8);
    memcpy(&packet->data[8], &length, 2);
    memcpy(&packet->data[10], &data[0], length);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::report_rtcm(){
    an_packet_t *packet;
    //uint8_t *time = current_time();
    vector<uint8_t> time = current_time();
    uint8_t lidar_data[300];
    for(int i=0; i<300; i++){
        lidar_data[i] = i % 256;
    }
    packet = an_packet_allocate(8 + 2 + 300);
    make_header(packet, 0x0b, 0x01, 0x01);
    packet->data_length = 310;
    memcpy(&packet->data[0], &time.data()[0], 8);
    big_uint16_t length = 300;
    memcpy(&packet->data[8], &length, 2);
    memcpy(&packet->data[10], lidar_data, 300);
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::print_params_inquiry_packet(){
    cout << "\nthis is a message of params_inquiry_packet\n" ;
    cout << "count is : " << (int) params_inquiry_packet.count << endl;
    cout << "number of params is : " << (int) params_inquiry_packet.number << endl;
    cout << "params list ......\n";
    for(int i=0; i < params_inquiry_packet.number; i++){
        printf("%02x\t", params_inquiry_packet.ids[i]);
    }
    cout << "\n";
}
// 0x80
// respond to params inquiry
void SingletonClient::respond_params_inquiry(){
    vector<uint8_t> params;
    uint8_t params_count = 0;
    for(int i=0; i< params_inquiry_packet.ids.size(); i++){
        switch(params_inquiry_packet.ids[i]){
            case 0x01:{
                params.push_back(params_inquiry_packet.ids[i]);
                params.push_back((int) (cycle / 255));
                params.push_back((int) (cycle % 255));
                params_count ++;
                break;}
            case 0x02:{
                params.push_back(params_inquiry_packet.ids[i]);
                uint8_t domain_length = sizeof(domain);
                params.push_back((uint8_t)sizeof(domain));
                params.resize(params.size() + sizeof(domain));
                memcpy(&params[params.size() - sizeof(domain)], domain.c_str(), sizeof(domain));
                params_count ++;
                break;}
            case 0x03:{
                params.push_back(params_inquiry_packet.ids[i]);
                params.push_back((int) (atoi(port.c_str())/255));
                params.push_back(atoi(port.c_str()) % 255); // 5000
                params_count ++;
                break;}
            case 0x04:{
                params.push_back(params_inquiry_packet.ids[i]);
                params.push_back(sizeof(softV));
                params.resize(params.size() + sizeof(softV));
                memcpy(&params[params.size() - sizeof(softV)], softV.c_str(), sizeof(softV));
                params_count ++;
                break;}
            case 0x05:{
                params.push_back(params_inquiry_packet.ids[i]);
                params.push_back(sizeof(hardV));
                params.resize(params.size() + sizeof(hardV));
                memcpy(&params[params.size() - sizeof(hardV)], hardV.c_str(), sizeof(hardV));
                params_count ++;
                break;}
            case 0x06:{
                params.push_back(params_inquiry_packet.ids[i]);
                params.push_back(sizeof(vin));
                params.resize(params.size() + sizeof(vin));
                memcpy(&params[params.size() - sizeof(vin)], vin.c_str(), sizeof(vin));
                params_count ++;
                break;}
        }
    }
    vector<uint8_t> time = current_time();
    an_packet_t *packet;
    packet = an_packet_allocate(8 + 1 + params.size());
    make_header(packet, 0x80, 0x01, 0x01);
    packet->data_length = 8 + 1 + params.size();
    packet->count = params_inquiry_packet.count;
    memcpy(&packet->data[0], &time.data()[0], 8);
    memcpy(&packet->data[8], &params_count, 1);
    memcpy(&packet->data[9], &params[0], params.size());
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::print_params_config_packet(){
    cout << "\nthis is a message of params_config_packet\n" ;
    cout << "count is : " << params_config_packet.count << endl;
    cout << "number of params is : " << (int)params_config_packet.number << endl;
    cout << "params list ......\n";
    if( params_config_packet.report_cycle.size() > 0){
        cout <<  "report cycle is : " << (int)params_config_packet.report_cycle[0] << endl;
    }
    if( params_config_packet.domain_params.size() > 0){
        cout <<  "domain size is : " ;
        printf("%d \n",params_config_packet.domain_length[0]);
        cout <<  "domain is : " ;
        string sss((char *)params_config_packet.domain_params.data());
        cout << sss << endl;
    }
    if( params_config_packet.cloud_port.size() > 0){
        cout <<  "cloud port is : " << params_config_packet.cloud_port[0] << endl;
    }
    if( params_config_packet.software_version.size() > 0){
        cout <<  "software version is : " ;
        string sss((char *)params_config_packet.software_version.data());
        cout << sss << endl;
    }
    if( params_config_packet.hardware_version.size() > 0){
        cout <<  "hardware version is : " << &params_config_packet.hardware_version[0] << endl;
    }
    if( params_config_packet.vehicle_vin.size() > 0){
        cout <<  "vehicle vin is : " << &params_config_packet.vehicle_vin[0] << endl;
    }
    cout << "\n";

}
// 0x81
// respond to params config
void SingletonClient::respond_params_config(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x81, 0x01, 0x01);
    packet->count = params_config_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

// print upgrade control
void SingletonClient::print_upgrade_packet(){
    cout << "\nthis is a control message of upgrade_packet\n" ;
    cout << "count is : " << upgrade_packet.count << endl;
    string sss((char *)upgrade_packet.url.data());
    cout << sss << endl;
    cout << endl;
}
// print engine control
void SingletonClient::print_engine_packet(){
    cout << "\nthis is a message of engine_packet\n" ;
    cout << "count is : " << engine_packet.count << endl;
    printf("%02x",engine_packet.start);
    cout << endl;
}

void SingletonClient::print_take_over_packet(){
    cout << "\nthis is a message of take_over_packet\n" ;
    cout << "count is : " << take_over_packet.count << endl;
    cout << "take over is : " << endl;
    printf("%02x",take_over_packet.take_over );
    cout << endl;
}
void SingletonClient::print_take_over_cmd_packet(){
    cout << "\nthis is a message of take_over_cmd_packet\n" ;
    cout << "count is : " << take_over_cmd_packet.count << endl;
    cout << "gas brake is : " << (int)take_over_cmd_packet.gas_brake<< endl;
    cout << "hydraulic retarder is : " << (int)take_over_cmd_packet.hydraulic_retarder<< endl;
    cout << "pedal percent is : " << (int)take_over_cmd_packet.pedal_percent << endl;
    cout << "brake acc is : " << take_over_cmd_packet.brake_acc << endl;
    cout << "hand wheel is : " << take_over_cmd_packet.hand_wheel << endl;
    cout << "push up is : " << (int) take_over_cmd_packet.push_up << endl;
    cout << "gear is : ";
    printf("%02x\n", take_over_cmd_packet.gear );
    cout << "gear limit is : " << (int) take_over_cmd_packet.gear_limit << endl;
    cout << "ego vehicle is : ";
    printf("%02x %02x\n", take_over_cmd_packet.ego_vehicle[0], take_over_cmd_packet.ego_vehicle[1]);
    cout << "hand brake is : " << (int) take_over_cmd_packet.hand_brake << endl;
    cout << endl;
}
// 0x82
// respond to remote control 
void SingletonClient::respond_upgrade_control(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = upgrade_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::respond_estop(){
    estop_packet_t count_packet = estop_packet_();
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = count_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::respond_auto_avoid(){
    auto_avoid_packet_t count_packet = auto_avoid_packet_();
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = count_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::respond_restrict_speed(){
    restrict_speed_packet_t count_packet = restrict_speed_packet_();
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = count_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
double SingletonClient::restrict_speed(){
    boost::lock_guard<boost::mutex> lock{mutex};
    restrict_speed_packet_t tmp = restrict_speed_packet;
    restrict_speed_packet.is_new = false;
    return tmp.speed;
}

void SingletonClient::respond_engine_control(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = engine_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::respond_take_over_control(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x03, 0x01);
    packet->count = take_over_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}
void SingletonClient::respond_take_over_cmd_control(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x82, 0x01, 0x01);
    packet->count = take_over_cmd_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

void SingletonClient::print_map_data(){
    cout << "\nthis is a message of map\n" ;
    cout << "count is : " << map_data_packet.count << endl;
    for(int i=0; i< map_data_packet.real_number ; i++){
        if(!map_data_packet.roads[i].points.size()){continue; }
        cout << i << " th road, first point:\n";
        cout << "size:" << map_data_packet.roads[i].points.size() << endl;
            cout << "\tx: " << map_data_packet.roads[i].points[0].x << endl;
            cout << "\ty: " << map_data_packet.roads[i].points[0].y << endl;
            cout << "\tz: " << map_data_packet.roads[i].points[0].z << endl;
            cout << "\tc: " << map_data_packet.roads[i].points[0].c << endl;
    }
}

void SingletonClient::print_assignment_packet(){
    cout << "\nthis is a message of assignment_packet\n" ;
    cout << "count is : " << assignment_packet.count << endl;
    cout << "number is : " << (int)assignment_packet.number << endl;
    cout << "task start is : " << &assignment_packet.tasks[0].id[0] << endl;
    cout << "task end is : " << &assignment_packet.tasks[assignment_ajust_packet.tasks.size()-1].id[0] << endl;

    for(int i=0 ; i< assignment_packet.real_number; i++){
        cout << "serial number is : " << (int)assignment_packet.tasks[i].serial_number << endl;
        cout << "================\n";
        printf("gear is : %02x\n",assignment_packet.tasks[i].gear);
        printf("format is : %02x\n",assignment_packet.tasks[i].format);
        string sss((char *)assignment_packet.tasks[i].id);
        cout << "id is : " << sss << endl;
        cout << "type is : " << (int)assignment_packet.tasks[i].serial_number << endl;
        for(int j=0; j<assignment_packet.tasks[i].real_point_number;j++){
            cout << "\t************\n";
            cout << "\tlat is : " << assignment_packet.tasks[i].points[j].latitude << endl;
            cout << "\tlon is : " << assignment_packet.tasks[i].points[j].lontitude << endl;
            cout << "\theight is : " << assignment_packet.tasks[i].points[j].height << endl;
            cout << "\theading is : " << assignment_packet.tasks[i].points[j].heading<< endl;
            cout << "\t************\n";
            break;
        }
        cout << "================\n";

    }
    cout << endl;
}
void SingletonClient::print_assignment_ajust_packet(){
    cout << "\nthis is a message of assignment_packet\n" ;
    cout << "count is : " << assignment_ajust_packet.count << endl;
    cout << "number is : " << assignment_ajust_packet.number << endl;
    cout << "task start is : " << &assignment_ajust_packet.tasks[0].id[0] << endl;
    cout << "task end is : " << &assignment_ajust_packet.tasks[assignment_ajust_packet.tasks.size()-1].id[0] << endl;
    cout << endl;
}
// 0x83 does not need respond
// 0x84
// respond to assingment
void SingletonClient::respond_assignment(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x84, 0x01, 0x01);
    packet->count = assignment_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    cout << "before write assignment respond\n";
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    cout << "wrote assignment respond\n";
    an_packet_free(&packet);
}
void SingletonClient::respond_assignment_ajust(){
    an_packet_t *packet;
    packet = an_packet_allocate(0);
    make_header(packet, 0x84, 0x01, 0x01);
    packet->count = assignment_ajust_packet.count;
    an_packet_encode(packet);
    uint8_t *bytes = (uint8_t *) packet;
    push_to_other(bytes,packet->data_length + AN_PACKET_HEADER_SIZE + 1);
    an_packet_free(&packet);
}

// 0x85 does not neet respond
// but print
void SingletonClient::print_information_packet(){
//    string sss((char *)information_packet.content.data());
//    cout << sss << endl;
}

// 0x86 does not neet respond
// but print
void SingletonClient::print_rtcm_packet(){
    for(int i=0;i<rtcm_packet.data.size();i++){
        printf("%02x ", rtcm_packet.data[i]);
    }
    cout << endl;
}

// 0x87 does not neet respond
// but print
void SingletonClient:: print_sorrd_packet(){
    cout << "sorrounding vehicles message!\n";
    for(int i=0; i< surrd_vehicle_packet.vehicles.size(); i++){
        cout << surrd_vehicle_packet.vehicles[i].id << endl;
    }
}

// process raw bytes data when read any data from server
void SingletonClient::process_data(int bytes_received){
    bool print = true;
    cout << "before increment, buffer length is : " << an_decoder.buffer_length << endl;
    an_decoder.buffer_length += bytes_received;
    cout << "process begining, buffer length is : " << an_decoder.buffer_length << endl;
    an_packet_t * an_packet;

    // an_packet_decode while return an packet, then move the left data to front of an_decoder.buffer, and decrement buffer_length
    //while((an_packet = an_packet_decode(&an_decoder)) != NULL){
    while(true){
        try{
            an_packet = an_packet_decode(&an_decoder);
        }catch(exception error){
            Logger::Instance()->Writeln("ERROR: decode packet");
            Logger::Instance()->Writeln(error.what());
        }
        if(an_packet == NULL){break;}
        cout << "process looping start, buffer length is : " << an_decoder.buffer_length << endl;
        printf("cmd_id is %02x, request_id is %02x\n", an_packet->cmd_id, an_packet->request_id);
        Logger::Instance()->Writeln("MSG: processing packet .........");
        Logger::Instance()->Writeln("\t*cmd_id : "+to_string(an_packet->cmd_id));
        Logger::Instance()->Writeln("\t*requst_id : "+to_string(an_packet->request_id));
        if(an_packet->cmd_id == 0x80 && an_packet->request_id == 0xFE){
        /// this is an params inquery cmd packet from cloud
            Logger::Instance()->Writeln("\t*this is an params inquiry cmd packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{ 
                decode_params_inquiry_packet(&params_inquiry_packet, an_packet);
                respond_params_inquiry();
                /// respond to inquiry
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decode packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x81 && an_packet->request_id == 0xFE){
            Logger::Instance()->Writeln("\t*this is an params config cmd packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{ 
                params_config_packet_t tmp_packet;
                //decode_params_config_packet(&params_config_packet, an_packet);
                decode_params_config_packet(&tmp_packet, an_packet);
                config_packet(tmp_packet);
                /// respond to inquiry
                respond_params_config();
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decode packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x82 && an_packet->request_id == 0xFE){
            Logger::Instance()->Writeln("\t*this is a remote control packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
        /// this is an remote control cmd packet from cloud
            // data[0~7] is time, data[8] is control id
            if(an_packet->data_length <=8){continue;}
            if(an_packet->data[8] == 0x01){
                // this is an upgrade packet
                Logger::Instance()->Writeln("\t* upgrade control packet!");
                try{
                    decode_upgrade_packet(&upgrade_packet, an_packet);
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x02){
                // this is an engine start packet
                Logger::Instance()->Writeln("\t* engine control packet!");
                try{
                    decode_engine_packet(&engine_packet, an_packet);
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x03){
                // this is an take over packet
                Logger::Instance()->Writeln("\t* take over control packet!");
                try{
                    decode_take_over_packet(&take_over_packet, an_packet);
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x04){
                // this is an take over packet
                Logger::Instance()->Writeln("\t* take over cmd control packet!");
                try{
                    decode_take_over_cmd_packet(&take_over_cmd_packet, an_packet);
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x05){
                // this is an take over packet
                Logger::Instance()->Writeln("\t* take speed restrict packet!");
                try{
                    restrict_speed_packet_t tmp_packet;
                    decode_restrict_speed_packet(&tmp_packet, an_packet);
                    restrict_speed_packet_(tmp_packet);
                    respond_restrict_speed();
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x06){
                // this is an take over packet
                Logger::Instance()->Writeln("\t* estop packet!");
                try{
                    estop_packet_t tmp_packet;
                    decode_estop_packet(&tmp_packet, an_packet);
                    estop_packet_(tmp_packet);
                    respond_estop();
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }else if(an_packet->data[8] == 0x07){
                // this is an take over packet
                Logger::Instance()->Writeln("\t* auto avoid packet!");
                try{
                    auto_avoid_packet_t tmp_packet;
                    decode_auto_avoid_packet(&tmp_packet, an_packet);
                    auto_avoid_packet_(tmp_packet);
                    respond_auto_avoid();
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode packet");
                    Logger::Instance()->Writeln(error.what());
                }
            }
        }else if(an_packet->cmd_id==0x83&&an_packet->request_id==0x01){
        /// this is an login back cmd packet from cloud
            Logger::Instance()->Writeln("\t*this is jianquan cmd packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{ 
                login_respond_packet_t tmp_packet;
                decode_login_respond_packet(&tmp_packet, an_packet);
                jianquanCode(tmp_packet);
                jianquan();
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decode packet");
                Logger::Instance()->Writeln(error.what());
            }
        }else if(an_packet->cmd_id==0x84&&an_packet->request_id==0xFE){
        /// this is an assignment cmd packet from cloud
            bool decompressed = false;
            an_packet_t *gzip_packet;
            string decompressed_data;
            Logger::Instance()->Writeln("\t*this is assignment packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{ 
                decompressed_data = gzip::decompress( (char *) an_packet->data, an_packet->data_length);
                gzip_packet = an_packet_allocate(decompressed_data.size());
                memcpy(&gzip_packet->data[0], decompressed_data.c_str(), decompressed_data.size());
                gzip_packet->data_length = decompressed_data.size();
                decompressed = true;
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
                decompressed = false;
            }
            if(gzip_packet->data[8] == 0x01 && decompressed){
                try{
                    decode_assignment_packet(&assignment_packet, gzip_packet);
                    assignment_packet.count = an_packet->count;
                    respond_assignment();
                    print_assignment_packet();
                    write_task();
                }catch(exception error){
                    Logger::Instance()->Writeln("ERROR: decode assignment packet");
                    Logger::Instance()->Writeln(error.what());
                    decompressed = false;
                }
                an_packet_free(&gzip_packet);
            } else if(gzip_packet->data[8] == 0x02 && decompressed){
            }
        } else if(an_packet->cmd_id == 0x85 && an_packet->request_id == 0x01){
        /// this is an event information cmd packet from cloud
            Logger::Instance()->Writeln("\t*this is event information packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                boost::lock_guard<boost::mutex> lock{mutex};
                decode_information_packet(&information_packet, an_packet);
                cloud_info_.new_message_flag = true;
                cloud_info_.message_packet = information_packet;
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x86 && an_packet->request_id == 0x01){
        /// this is an rtcm respond packet from cloud
            Logger::Instance()->Writeln("\t*this is rtcm respond packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                cout << "************************\n";
                rtcm_packet_t tmp_packet;
                decode_rtcm_packet(&tmp_packet, an_packet);
                rtcm(tmp_packet);
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x87 && an_packet->request_id == 0x01){
        /// this is srounding vehicles packet from cloud
            Logger::Instance()->Writeln("\t*this is sroudings packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                surrd_vehicle_packet_t tmp_vehicle;
                decode_surrd_packet(&tmp_vehicle, an_packet);
                surround_vehicles(tmp_vehicle);
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x88 && an_packet->request_id == 0x01){
        /// this is sample gis packet from cloud
            Logger::Instance()->Writeln("\t*this is sample gis packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                sample_gis_packet_t tmp_gis;
                decode_sample_gis_packet(&tmp_gis, an_packet);
                string sss((char *)tmp_gis.unload_position);
                cout << "unload position id is  is : " << sss << endl;
                sample_gis_cmd(tmp_gis);
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: sample gis packet cmd");
                Logger::Instance()->Writeln(error.what());
            }
        
        } else if(an_packet->cmd_id == 0x08 && an_packet->request_id == 0x01){
            // not use for this
        } else if(an_packet->cmd_id == 0x08 && an_packet->request_id == 0x02){
            // not use for this
        } else if(an_packet->cmd_id == 0x05 && an_packet->request_id == 0x01){
        /// this is an jianquan respond packet from cloud
            Logger::Instance()->Writeln("\t*this is jianquan packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                jianquan_valid(true);
                jianquanClean();
                Logger::Instance()->Writeln("\t*jianquan successfully!");
                cout <<  "jianquan successfully\n\n" ;
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x05 && an_packet->request_id == 0x02){
        /// this is an jianquan respond packet from cloud
            Logger::Instance()->Writeln("\t*this is jianquan packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                jianquan_valid(false);
                Logger::Instance()->Writeln("\t*jianquan failed!");
                cout <<  "jianquan failed\n\n" ;
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x09 && an_packet->request_id == 0x01){
        /// this is an get map respond packet from cloud
            an_packet_t *gzip_packet;
            string decompressed_data ;
            Logger::Instance()->Writeln("\t*this is map packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                string decompressed_data = gzip::decompress( (char *) an_packet->data, an_packet->data_length);
                gzip_packet = an_packet_allocate(decompressed_data.size());
                memcpy(&gzip_packet->data[0], decompressed_data.c_str(), decompressed_data.size());
                gzip_packet->data_length = decompressed_data.size();
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decompress packet");
                Logger::Instance()->Writeln(error.what());
            }
            try{
                map_data_packet_t tmp_packet;
                decode_map_data_packet(&tmp_packet, gzip_packet);
                an_packet_free(&gzip_packet);
                map_data(tmp_packet);
                map_ok(true);
                print_map_data();
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decode packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else if(an_packet->cmd_id == 0x0a && an_packet->request_id == 0x01){
            boost::lock_guard<boost::mutex> lock{mutex};
            server_got_assignment_request = true;
        } else if(an_packet->cmd_id == 0x0b && an_packet->request_id == 0x01){
        /// this is an jianquan respond packet from cloud
            Logger::Instance()->Writeln("\t*this is rtcm packet!");
            Logger::Instance()->Writeln("\t*its' data length is : " + to_string(an_packet->data_length));
            try{
                rtcm_packet_t tmp_packet;
                decode_rtcm_packet(&tmp_packet, an_packet);
                rtcm(tmp_packet);
            }catch(exception error){
                Logger::Instance()->Writeln("ERROR: decode rtcm packet");
                Logger::Instance()->Writeln(error.what());
            }
        } else{
        }
        an_packet_free(&an_packet);
    }
    cout << "process end, buffer length is : " << an_decoder.buffer_length << endl;
}

void SingletonClient::respond_ontime(){
    if(to_respond_assignment){respond_assignment(); }
}

void SingletonClient::respond_thread(){
    while(running){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{1000});
        resolve_address();
        start_connect();
//        this->m_io.reset();this->m_io.run();
    }
}

void SingletonClient::stop(){
    running = false;
}

void SingletonClient::run_thread(){
    while(true){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{1000});
        resolve_address();
        start_connect();
//        this->m_io.reset();
//        this->m_io.run();
    }
}
void SingletonClient::read_thread(){
    bool print = false;
    int bytes_read = 0;
    boost::system::error_code read_ec;
    while(running){
        if(!this->Connected()){
            cout << "not connected with server!\n";
            resolve_address();
            start_connect();
            continue;}
        bytes_read = m_sock_ptr->read_some(buffer(an_decoder.buffer)+an_decoder.buffer_length, read_ec);
        cout << "recieved message : " ;
        cout << "bytes_read : " << bytes_read << endl;
        if(read_ec ){
            Logger::Instance()->Writeln("ERROR: reading from server!");
            Logger::Instance()->Writeln(read_ec.message());
            this->Connected(false);
            this->jianquan_valid(false);
            continue;
        }
        if(bytes_read <=0){continue;}
        cout << "bytes_read = " << bytes_read << endl;
        read_count++;
        process_data(bytes_read);
        vector<uint8_t> time = test_current_time();
        cout << "\nread time:";
        cout << "\t"<< setfill('0') << setw(2) << (int) time[3] << ":" << setw(2) <<  (int) time[4] << ":" <<  setw(2) << (int) time[5] << endl;
    }
}

void SingletonClient::conn_handler(const boost::system::error_code&ec) {
    this->Connecting(false);
    if (ec) {
        this->Connected(false);
        this->jianquan_valid(false);
        Logger::Instance()->Writeln("ERROR: connect server failed!");
        Logger::Instance()->Writeln(ec.message());
        cout << "connected server failed!" << endl;
        cout << ec.message() << endl;
        return;
    }
    this->Connected(true);
    cout << "connected successfully! \n";
    Logger::Instance()->Writeln("SUCCESS: connected with server!");
}

void SingletonClient::start_connect()
{
    if(this->Connected()){cout << "no need to connect!\n";return ;}
    if(this->Connecting()){cout << "connecting......\n";return ;}
    this->Connecting(true);
    m_sock_ptr->close();
    m_sock_ptr->async_connect(m_ep, 
       [&](const boost::system::error_code& inec){
        if (inec ){
            cout << "connect server failed!\n";
            cout << inec.message() << endl;
            this->Connected(false);
        }else{ 
            cout << "connect server susscessfully!\n";
            this->Connected(true);
        }
        this->Connecting(false);
    });
    this->m_io.reset();this->m_io.run();
}

void SingletonClient::resolve_address(){
    if(this->Resolved()){return ;}
    if(this->Resolving()){return ;}
    this->Resolving(true);
    boost::asio::ip::tcp::resolver::query query(domain, port);
    m_resolver.async_resolve(query, 
       [&](const boost::system::error_code& inec, 
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
        if (inec || endpoint_iterator == 
            boost::asio::ip::tcp::resolver::iterator()) {
            cout << "resolve address failed!\n";
            this->Resolving(false);
            Logger::Instance()->Writeln("ERROR: resolve address failed!");
            Logger::Instance()->Writeln(inec.message());
        }else{ 
            cout << "resolve address susscessfully!\n";
            m_ep = endpoint_iterator->endpoint();
            this->Resolved(true);
            Logger::Instance()->Writeln("SUCCESS: resolve address successfully!");
            Logger::Instance()->Writeln(inec.message());
        }
        this->Resolving(false);
    });
    this->m_io.reset();this->m_io.run();
}
bool SingletonClient::Resolved(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->resolved;
}
void SingletonClient::Resolved(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->resolved = _value;
}
bool SingletonClient::Connected(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->connected;
}
void SingletonClient::Connected(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->connected = _value;
}
bool SingletonClient::Connecting(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->connecting;
}
void SingletonClient::Connecting(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->connecting = _value;
}
bool SingletonClient::Resolving(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->resolving;
}
void SingletonClient::Resolving(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->resolving = _value;
}

SingletonClient::SingletonClient(): 
    m_strand(m_io), m_resolver(m_io){
    //m_resolver(m_io){
    initialize();
    m_sock_ptr = boost::make_shared<boost::asio::ip::tcp::socket>(m_io);
//    resolve_address();
//    start_connect();
    running = true;
}

void SingletonClient::initialize(){
    load_params();
    while(!read_params){ continue; }
    Logger::Instance()->Init();
    jianquan_code = new uint8_t[20];
    for(int i=0;i<20;jianquan_code[i]=i++){};
    to_respond_assignment = false;
    assignment_new = false;
    assignment_ajust_new = false;
    to_respond_assignment_ajust = false;

    connecting = false;
    connected = false;

    write_count = 1;
    read_count = 0;

    rtcm_packet.is_new = false;
    surrd_vehicle_packet.is_new = false;
    sample_gis_packet.is_new = false;
    restrict_speed_packet.is_new = false;
    estop_packet.is_new = false;
    auto_avoid_packet.is_new = false;
    an_decoder_initialise(&an_decoder);
}

SingletonClient::~SingletonClient(){
    delete []jianquan_code;
}

deque<vector<uint8_t> > SingletonClient::getOtherPool()
{
    boost::lock_guard<boost::mutex> lock{mutex};
    return other_pool;
}

void SingletonClient::setOtherPool(deque<vector<uint8_t> > _other_pool)
{
    boost::lock_guard<boost::mutex> lock{mutex};
    other_pool=_other_pool;
}

bool SingletonClient::has_new_assignment(){
    return assignment_new;
}
assignment_packet_t & SingletonClient::get_assignment_packet() {
    assignment_new = false;   
    return assignment_packet;
}

bool SingletonClient::assignment_request_ok(){
    boost::lock_guard<boost::mutex> lock{mutex};
    bool tmp = server_got_assignment_request;
    server_got_assignment_request = false;
    return tmp;
}

void SingletonClient::surround_vehicles(surrd_vehicle_packet_t _surrd_vehicles){
    boost::lock_guard<boost::mutex> lock{mutex};
    surrd_vehicle_packet = _surrd_vehicles;
}
surrd_vehicle_packet_t SingletonClient::surround_vehicles(){
    boost::lock_guard<boost::mutex> lock{mutex};
    surrd_vehicle_packet_t tmp_surround = surrd_vehicle_packet;
    surrd_vehicle_packet.is_new = false;
    return tmp_surround;
}


void SingletonClient::estop_packet_(const estop_packet_t &value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->estop_packet = value;
}
estop_packet_t SingletonClient::estop(){
    boost::lock_guard<boost::mutex> lock{mutex};
    estop_packet_t tmp_packet = this->estop_packet;
    this->estop_packet.is_new = false;
    return tmp_packet;
}
estop_packet_t SingletonClient::estop_packet_(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->estop_packet;
}

void SingletonClient::auto_avoid_packet_(const auto_avoid_packet_t &value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->auto_avoid_packet = value;
}

auto_avoid_packet_t SingletonClient::auto_avoid_packet_(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return this->auto_avoid_packet;
}

bool SingletonClient::auto_avoid(){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->auto_avoid_packet.is_new = false;
    return (this->auto_avoid_packet.auto_avoid == 0x01);
}

void SingletonClient::config_packet(const params_config_packet_t &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->params_config_packet = _value;
}

uint8_t SingletonClient::full_load(){
    boost::lock_guard<boost::mutex> lock{mutex};
    uint8_t full_load = this->params_config_packet.full_load;
    this->params_config_packet.full_load = 0xff;
    return full_load;
}

void SingletonClient::restrict_speed_packet_(const restrict_speed_packet_t &value){
    boost::lock_guard<boost::mutex> lock{mutex};
    this->restrict_speed_packet = value;
}
restrict_speed_packet_t SingletonClient::restrict_speed_packet_( ){
    boost::lock_guard<boost::mutex> lock{mutex};
    restrict_speed_packet_t tmp = restrict_speed_packet;
    restrict_speed_packet.is_new = false;
    return tmp;
}
                                                     
void SingletonClient::sample_gis_cmd(sample_gis_packet_t tmp_packet){
    boost::lock_guard<boost::mutex> lock{mutex};
    sample_gis_packet = tmp_packet;
}

sample_gis_packet_t SingletonClient::sample_gis_cmd_(){
    boost::lock_guard<boost::mutex> lock{mutex};
    sample_gis_packet_t tmp_gis = sample_gis_packet;
    return tmp_gis;
}
sample_gis_packet_t SingletonClient::sample_gis_cmd(){
    boost::lock_guard<boost::mutex> lock{mutex};
    sample_gis_packet_t tmp_gis = sample_gis_packet;
    sample_gis_packet.is_new = false;
    return tmp_gis;
}

bool SingletonClient::has_new_assignment_ajust(){
    return assignment_ajust_new;
}
assignment_ajust_packet_t & SingletonClient::get_assignment_ajust_packet() {
    assignment_ajust_new = false;   
    return assignment_ajust_packet;
}

void SingletonClient::update_time(utc_time time){
    boost::lock_guard<boost::mutex> lock{mutex};
    gps_time = time;
}
void SingletonClient::map_data(const map_data_packet_t &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    map_data_packet = _value;
    mapV = string((char *)_value.version);
}

bool SingletonClient::map_ok(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return map_ready_;
}
void SingletonClient::map_ok(const bool &_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    map_ready_ = _value;
}

bool SingletonClient::read_ok()
{
    boost::lock_guard<boost::mutex> lock{mutex};
    return map_ready_;
}

cloud_info SingletonClient::get_info(){
    boost::lock_guard<boost::mutex> lock{mutex};
    cloud_info return_value = cloud_info_;
    cloud_info_.new_message_flag = false;
    return return_value;
}

string SingletonClient::get_vin(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return vin;
}

map_data_packet_t SingletonClient::read_map_boundary()
{
    boost::lock_guard<boost::mutex> lock{mutex};
    map_ready_ = false;
    return map_data_packet;
}

static int create_callback(void *NotUsed, int argc, char **argv, char **azColName){
   for(int i=0; i<argc; i++){
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   } printf("\n"); return 0;
}
int SingletonClient::params_callback(void *pVoid, int argc, char **argv, char **azColName){
    SingletonClient* p = (SingletonClient*) pVoid;
   for(int i=0; i<argc; i++){
       p->vin =    string(azColName[i]) == "VIN" ? argv[i] : p->vin;
       p->token =  string(azColName[i]) == "TOKEN" ? argv[i] : p->token;
       p->mapV =   string(azColName[i]) == "MapV" ? argv[i] : p->mapV;
       p->softV =  string(azColName[i]) == "SoftV" ? argv[i] : p->softV;
       p->hardV =  string(azColName[i]) == "HardV" ? argv[i] : p->hardV;
       p->cycle =  string(azColName[i]) == "Cycle" ? atoi(argv[i]) : p->cycle;
       p->domain = string(azColName[i]) == "Domain" ? argv[i] : p->domain;
       p->port =   string(azColName[i]) == "Port" ? argv[i] : p->port;
   }
   cout << "server : " << p->domain << ":" << p->port << endl;
   Logger::Instance()->Writeln("SUCCESS: got params from params.conf!");
   Logger::Instance()->Writeln("\t*** server: " +p->domain + ":" + p->port );
   p->read_params = true;
   return 0;
}
static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName){
   for(int i=0; i<argc; i++){
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   } //printf("\n");
   return 0;
}

void SingletonClient::write_task(){
    cout << "come in write_task() function\n" ;
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    //char *sql;
    string sql;

    /* Open database */
    task_db_t tmp_db;
    tmp_db.dbfile = "task_" + 
              to_string(gps_time.year) + "_" + 
              to_string(gps_time.month) + "_" +
              to_string(gps_time.day) + "_" +
              to_string(gps_time.hour) + "_" +
              to_string(gps_time.minute) + "_" +
              to_string(gps_time.second) + "_" +
              to_string(gps_time.millisecond) + ".map" ;
    cout << "db name is : " << tmp_db.dbfile << endl;

    rc = sqlite3_open(tmp_db.dbfile.c_str(), &db);
    this->taskName(tmp_db.dbfile);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        Logger::Instance()->Writeln("ERROR: Can't open database!");
        Logger::Instance()->Writeln(string(sqlite3_errmsg(db)));
        //exit(0);
    }else{
        fprintf(stdout, "Opened database successfully!\n");
        Logger::Instance()->Writeln("SUCCESS:Opened database successfully!"); 
    }

    /* Create SQL statement */
    sql = "CREATE TABLE TASK("  \
          "roadIndex INT PRIMARY KEY     NOT NULL," \
          "type INT NOT NULL," \
          "x DOUBLE NOT NULL," \
          "y DOUBLE NOT NULL," \
          "z DOUBLE NOT NULL," \
          "heading DOUBLE NOT NULL," \
          "roll DOUBLE NOT NULL," \
          "pitch DOUBLE NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), create_callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        Logger::Instance()->Writeln("ERROR: SQL error!");
        Logger::Instance()->Writeln(string(zErrMsg));
    }else{
        fprintf(stdout, "Table created successfully\n");
        Logger::Instance()->Writeln("SUCCESS: Table created successfully!"); 
    }

    //task_headers.clear();
    //task_db.ids.clear();
    //task_db.types.clear();
    //task_db.gears.clear();
    //task_db.formats.clear();
    sql = "";

    sqlite3_exec(db, "Begin transaction;",0,0,0);
    size_t roadIndex = 0;
    for(task tsk : assignment_packet.tasks){
        string id (reinterpret_cast<char const*>(tsk.id), 20);
        //memcpy(header.id, tsk.id, 20);
        tmp_db.ids.push_back(id);
        tmp_db.types.push_back(tsk.type);
        tmp_db.gears.push_back(tsk.gear);
        tmp_db.formats.push_back(tsk.format);

        //cout << "writing db : " << tsk.format << endl;

        for(trajectory_point pt: tsk.points){
            sqlite3_stmt *stmt;
            if(sqlite3_prepare(db,
                               "INSERT INTO TASK(roadIndex,type,x,y,z,heading,roll,pitch)  values (?,?,?,?,?,?,?,?)",
                               -1,
                               &stmt,
                               0) != SQLITE_OK){sqlite3_finalize(stmt);continue;}
            if(sqlite3_bind_int64(stmt, 1, roadIndex++)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_int64(stmt, 2, (int8_t)pt.limit_speed)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 3, pt.lontitude)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 4, pt.latitude)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 5, pt.height)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 6, pt.heading)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 7, 0)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_bind_double(stmt, 8, 0)!=SQLITE_OK){sqlite3_finalize(stmt); continue;}
            if(sqlite3_step(stmt) != SQLITE_DONE){sqlite3_finalize(stmt);continue;}
            sqlite3_finalize(stmt);
        }
    }
    //rc = sqlite3_exec(db, sql.c_str(), insert_callback, 0, &zErrMsg);
    //if( rc != SQLITE_OK ){
    //   sqlite3_free(zErrMsg);
    //   Logger::Instance()->Writeln("Error: Insert task failed!"); 
    //   Logger::Instance()->Writeln(string(zErrMsg)); 
    //}else{
    //   Logger::Instance()->Writeln("SUCCESS: Insert task successfully!"); 
    //}
    sqlite3_exec(db, "commit transaction;",0,0,&zErrMsg);
    sqlite3_close(db);
    set_task(tmp_db);
    return ;
}

bool SingletonClient::load_params(){
    sqlite3 *db; char *zErrMsg = 0; int  rc; char *sql;                   
    /* Open database */          
    rc = sqlite3_open("params.conf", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        Logger::Instance()->Writeln("ERROR: Can't open database!");
        Logger::Instance()->Writeln(string(sqlite3_errmsg(db)));
        //exit(0);
        this->read_params = true;
        return false;
    }else{
        fprintf(stdout, "Opened database successfully!\n");
        Logger::Instance()->Writeln("SUCCESS:Opened database successfully!"); 
    }
    sql = "SELECT * from PARAMS where ID =  1";
    rc = sqlite3_exec(db, sql, params_callback, this, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        Logger::Instance()->Writeln("ERROR: SQL error!");
        Logger::Instance()->Writeln(string(zErrMsg));
        sqlite3_free(zErrMsg); sqlite3_close(db);
        this->read_params = true;
        return false;
    }
    Logger::Instance()->Writeln("SUCCESS: Table created successfully!"); 
    sqlite3_close(db);
    return true;
} 

void SingletonClient::jianquanClean(){
    boost::lock_guard<boost::mutex> lock{mutex};
    for(int i=0;i<20;jianquan_code[i] = i++){}
}
void SingletonClient::jianquanCode(const login_respond_packet_t &_tmp_packet){
    boost::lock_guard<boost::mutex> lock{mutex};
    memcpy(&jianquan_code[0], &_tmp_packet.jianquan[0],20);
}
void SingletonClient::jianquanCode(uint8_t *jianquan){
    boost::lock_guard<boost::mutex> lock{mutex};
    for(int i=0;i<20;i++){jianquan[i] = jianquan_code[i];}
}

bool SingletonClient::jianquan_valid(){
    boost::lock_guard<boost::mutex> lock{mutex};
    return jianquan_result;
}

void SingletonClient::jianquan_valid(const bool&_value){
    boost::lock_guard<boost::mutex> lock{mutex};
    jianquan_result = _value;
}
