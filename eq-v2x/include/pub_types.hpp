#pragma once
#include <iostream>
#include <vector>
#include <boost/endian/arithmetic.hpp>                                         
#include "pub_types.hpp"

using namespace std;
using namespace boost::endian;

struct utc_time
{
    utc_time()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}

    utc_time(uint16_t _year,uint8_t _month,uint8_t _day,uint8_t _hour,
             uint8_t _minute,uint8_t _second,uint16_t _millisecond)
        :year(_year)
        ,month(_month)
        ,day(_day)
        ,hour(_hour)
        ,minute(_minute)
        ,second(_second)
        ,millisecond(_millisecond)
    {}

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millisecond;

};

struct GIS_UPDATE_PACKET{
    GIS_UPDATE_PACKET():state(0),uuid(""),updata_area(0),sensor_pos(0),sensor_type(0),lidar_data(0){}
    GIS_UPDATE_PACKET(uint8_t _state, string _id, uint8_t _area, uint8_t _pos, uint8_t _type, vector<uint8_t> _lidar)
     :state(_state),uuid(_id),updata_area(_area),sensor_pos(_pos),sensor_type(_type),lidar_data(_lidar){ }
    utc_time timestamp;
    uint8_t state;       
    string uuid;
    uint8_t updata_area; 
    uint8_t sensor_pos; 
    uint8_t sensor_type;
    vector<uint8_t> lidar_data;
};

struct GIS_VECTOR_POINT{
    double x = 0;
    double y = 0;
    double z = 0;
};

struct point_2d_t
{
    point_2d_t()
    :x(0), y(0)
    {}
    point_2d_t(const double &_x, const double &_y)
    :x(_x), y(_y)
    {}
    double x;
    double y;

};

struct point_3d_t
{
    point_3d_t()
    :x(0), y(0), z(0)
    {}
    point_3d_t(const double &_x, const double &_y, const double &_z)
    :x(_x), y(_y), z(_z)
    {}
    double x;
    double y;
    double z;
};

struct sample_gis_packet_t{
    uint8_t time[8];          
    uint8_t type = 0;         
    big_uint16_t duration = 0;
    uint8_t area = 0;         
    uint8_t unload_position[32];          
    bool is_new = false;       
}; 

struct GIS_VECTOR_PACKET{
    string uuid;
    uint8_t updata_area; 
    size_t point_number;
    vector<point_3d_t> points;
};

struct task_db_t{
    string dbfile;
    vector<string> ids;
    vector<uint8_t> types;
    vector<uint8_t> gears;
    vector<uint8_t> formats;
};

