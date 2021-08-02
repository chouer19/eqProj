#include "header/protocol/pro_gnss_receiver.h"


template <class Type>
Type stringToNum(const string& str)
 {
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}


IGNSS_RECEIVER_718D::IGNSS_RECEIVER_718D(DRIVE *_dri, SYSTEM *_sys, RUN_CORE _core)
    :IGNSS_RECEIVER(pub_time, TASK_PERIOD_GNSS_RECEIVER)
    ,dri(_dri)
    ,sys(_sys)
    ,core(_core)
{
    GpsManager::Instance()->Init("/dev/ttyS0",115200);
//    GpsManager::Instance()->Init("/dev/ttyUSB0",115200);
    GpsManager::Instance()->Run();
}

IGNSS_RECEIVER_718D::~IGNSS_RECEIVER_718D()
{
    abort();
    sys->taskList_p[core]->erase(taskId);
    m_isAlive = false;
    m_thread.join();
}

void IGNSS_RECEIVER_718D::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);
}

void IGNSS_RECEIVER_718D::parse()
{

    nav_t _gnssData=getGnssOutput();
    size_t timestamp_posa=GpsManager::Instance()->Posa().TimeStamp();
    size_t timestamp_vela=GpsManager::Instance()->Vela().TimeStamp();
    size_t timestamp_heading=GpsManager::Instance()->Heading().TimeStamp();
    size_t timestamp_utc=GpsManager::Instance()->Utc().TimeStamp();

    size_t t1=pub_time->getDiffTimeMs(timestamp_posa);
    size_t t2=pub_time->getDiffTimeMs(timestamp_vela);
    size_t t3=pub_time->getDiffTimeMs(timestamp_heading);
    size_t t4=pub_time->getDiffTimeMs(timestamp_utc);

    _gnssData.time_stamp=MAX(timestamp_posa,MAX(timestamp_vela,MAX(timestamp_heading,timestamp_utc)));
    TERMINAL_INFO _terminalInfo=getTerminalInfo();
    if(pub_time->getDiffTimeMs(timestamp_posa)<1000)
    {
        _gnssData.latitude= GpsManager::Instance()->Posa().Latitude();
        _gnssData.longitude= GpsManager::Instance()->Posa().Longitude();
        _gnssData.altitude= GpsManager::Instance()->Posa().Altitude();
        _gnssData.align_status= GpsManager::Instance()->Posa().PosType();
        _gnssData.solnSVs= GpsManager::Instance()->Posa().SatelliteNumber();




        point_3d_t pos;
        geographicLib_coordinate_transfer gct;
        gct.setGroundOrigin(_terminalInfo.ground_origin);
        gct.forward(_gnssData.latitude, _gnssData.longitude, _gnssData.altitude, pos.x, pos.y, pos.z);
        _gnssData.pos=pos;


    }

    if(pub_time->getDiffTimeMs(timestamp_vela)<1000)
    {
        _gnssData.horizontalSpeed=GpsManager::Instance()->Vela().Speed();
        _gnssData.verticalSpeed=GpsManager::Instance()->Vela().VertSpeed();
    }

    if(pub_time->getDiffTimeMs(timestamp_heading)<1000)
    {
        _gnssData.heading= GpsManager::Instance()->Heading().Azimuth();
        _gnssData.roll= GpsManager::Instance()->Heading().Pitch();

        _gnssData.heading+=_terminalInfo.heading_offset;

        while(_gnssData.heading<0)
            _gnssData.heading+=360;

        while(_gnssData.heading>=360)
            _gnssData.heading-=360;
    }

    if(pub_time->getDiffTimeMs(timestamp_utc)<1000)
    {
        _gnssData.utcTime=UTCToBeijing(GpsManager::Instance()->Utc().UtcTime());
    }


    //test
//    nav_t _testGnssData;
//    _testGnssData.latitude= 39.882934693+0.00009;
//    _testGnssData.longitude= 110.257838608;
//    _testGnssData.altitude=1431.25;

//    nav_t _testGnssData;
//    _testGnssData.latitude= 39.883373;
//    _testGnssData.longitude= 110.258045+0.00005;
//    _testGnssData.altitude=1431.25;




//    point_3d_t pos;

//    geographicLib_coordinate_transfer gct;
//    gct.setGroundOrigin(_terminalInfo.ground_origin);
//    gct.forward(_testGnssData.latitude, _testGnssData.longitude, _testGnssData.altitude, pos.x, pos.y, pos.z);
//    _testGnssData.pos=pos;

//    _testGnssData.heading+=_terminalInfo.heading_offset;

//    _testGnssData.heading=270;

//    while(_testGnssData.heading<0)
//        _testGnssData.heading+=360;

//    while(_testGnssData.heading>=360)
//        _testGnssData.heading-=360;
    //----------------

    setGnssOutput(_gnssData);
}

nav_t IGNSS_RECEIVER_718D::getGnssOutput()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return gnssOutput;
}

nav_t IGNSS_RECEIVER_718D::getReferencePos()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return referencePos;
}

nav_t IGNSS_RECEIVER_718D::getBasePoint()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return basePoint;
}

void IGNSS_RECEIVER_718D::setBasePointOffset(point_3d_t _point)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    basePointOffset=_point;
}

void IGNSS_RECEIVER_718D::setRefPointOffset(point_3d_t _point)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    refPointOffset=_point;
}

void IGNSS_RECEIVER_718D::setTerminalInfo(TERMINAL_INFO _info)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalInfo=_info;
}

void IGNSS_RECEIVER_718D::setGnssOutput(nav_t _gnssOutput)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    gnssOutput=_gnssOutput;
}

void IGNSS_RECEIVER_718D::setBasePoint(nav_t _basePoint)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    basePoint=_basePoint;
}

utc_time IGNSS_RECEIVER_718D::UTCToBeijing(utc_time _utc_time)
{
    utc_time beijing_time=_utc_time;
    int year=0,month=0,day=0,hour=0;
    int lastday = 0;// 月的最后一天日期
    int lastlastday = 0;//上月的最后一天日期

    year=_utc_time.year;
    month=_utc_time.month;
    day=_utc_time.day;
    hour=_utc_time.hour+8;//UTC+8转换为北京时间

    if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
    {
        lastday = 31;
        if(month == 3)
        {
            if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//判断是否为闰年
                lastlastday = 29;//闰年的2月为29天，平年为28天
            else
                lastlastday = 28;
        }
        if(month == 8)
            lastlastday = 31;
    }
    else  if(month == 4 || month == 6 || month == 9 || month == 11)
    {
        lastday = 30;
        lastlastday = 31;
    }
    else
    {
        lastlastday = 31;
        if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//闰年的2月为29天，平年为28天
            lastday = 29;
        else
            lastday = 28;
    }

    if(hour >= 24)//当算出的时大于或等于24：00时，应减去24：00，日期加一天
    {
        hour -= 24;
        day += 1;
        if(day > lastday)//当算出的日期大于该月最后一天时，应减去该月最后一天的日期，月份加上一个月
        {
            day -= lastday;
            month += 1;

            if(month > 12)//当算出的月份大于12，应减去12，年份加上1年
            {
                month -= 12;
                year += 1;
            }
        }
    }

    beijing_time.year=year;
    beijing_time.month=month;
    beijing_time.day=day;
    beijing_time.hour=hour;

    return beijing_time;
}

point_3d_t IGNSS_RECEIVER_718D::Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData)
{
    point_3d_t output_data;
    //
    double x=srcPoint.x;
    double y=srcPoint.y;
    double z=srcPoint.z;

    //绕y轴旋转
    double x2,y2,z2;
    double pitch_com=(insData.pitch)*M_PI/180.0;
    z2=z*cos(pitch_com)-x*sin(pitch_com);
    x2=z*sin(pitch_com)+x*cos(pitch_com);
    y2=y;

    //绕x轴旋转
    double x1,y1,z1;
    double roll_com=(insData.roll)*M_PI/180.0;
    y1 = y2*cos(roll_com)-z2*sin(roll_com);
    z1 = y2*sin(roll_com)+z2*cos(roll_com);
    x1 = x2;

    //绕z轴旋转
    double x3,y3,z3;
    double yaw_com=(-(insData.heading)+90)*M_PI/180.0;
    x3=x1*cos(yaw_com)-y1*sin(yaw_com);
    y3=x1*sin(yaw_com)+y1*cos(yaw_com);
    z3=z1;



    output_data.x =x3 +insData.pos.x ;//x no change
    output_data.y =y3 +insData.pos.y;
    output_data.z =z3 +insData.pos.z;



    return output_data;
}

void IGNSS_RECEIVER_718D::setReferencePos(nav_t _referencePos)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    referencePos=_referencePos;
}

point_3d_t IGNSS_RECEIVER_718D::getBasePointOffset()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return basePointOffset;
}

point_3d_t IGNSS_RECEIVER_718D::getRefPointOffset()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return refPointOffset;
}

bool IGNSS_RECEIVER_718D::getParasSet()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return paras_set;
}

TERMINAL_INFO IGNSS_RECEIVER_718D::getTerminalInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalInfo;
}

void IGNSS_RECEIVER_718D::task_func()
{
    parse();
    nav_t _gnss_info=getGnssOutput();
    TERMINAL_INFO _terminalInfo=getTerminalInfo();
    geographicLib_coordinate_transfer gct;
    gct.setGroundOrigin(_terminalInfo.ground_origin);

    //base point
    nav_t _basepoint=_gnss_info;
//    _basepoint.pos=Coordinate_TransfertoWorld(getBasePointOffset(),_gnss_info);
    _basepoint.pos=Coordinate_TransfertoWorld(_terminalInfo.base_point_offset,_gnss_info);
    gct.reverse(_basepoint.pos.x,_basepoint.pos.y,_basepoint.pos.z,_basepoint.latitude,_basepoint.longitude,_basepoint.altitude);
    setBasePoint(_basepoint);

    //buckget
    nav_t _buckgetPos=_gnss_info;
//    _buckgetPos.pos=Coordinate_TransfertoWorld(getRefPointOffset(),_gnss_info);
    _buckgetPos.pos=Coordinate_TransfertoWorld(_terminalInfo.ref_point_offset,_gnss_info);
    gct.reverse(_buckgetPos.pos.x,_buckgetPos.pos.y,_buckgetPos.pos.z,_buckgetPos.latitude,_buckgetPos.longitude,_buckgetPos.altitude);
    setReferencePos(_buckgetPos);

    //telecom update
    //vehicle_pos_message
    telecom_vehicle_pos_message _tvpm;
    uint8_t _status=0;

    if(_basepoint.time_stamp>0&&_terminalInfo.parasGet)
        _tvpm.message_valid=true;
    else
        _tvpm.message_valid=false;

    if(pub_time->getDiffTimeMs(_basepoint.time_stamp)>1000)
    {
        _status=_status|0x01;
    }
    else
    {
        _status=_status&0xfe;
    }

    vehicle_pos_message _vpm;
    _vpm.id=0x05;
    _vpm.data_length=37;
    if(_basepoint.latitude>=0)
        _status=_status&0xfd;
    else
        _status=_status|0x02;

    if(_basepoint.longitude>=0)
        _status=_status&0xfb;
    else
        _status=_status|0x04;

    _vpm.status=_status;
    _vpm.lontitude=fabs(_basepoint.longitude)*pow(10,9);
    _vpm.latitude=fabs(_basepoint.latitude)*pow(10,9);
    _vpm.height=(_basepoint.altitude+1000)*10000;
    _vpm.yaw=_basepoint.heading*100;
    _vpm.roll=(_basepoint.roll+180)*100;
    _vpm.pitch=(_basepoint.pitch+90)*100;
    _vpm.satellite_num=_basepoint.solnSVs;

    switch (_basepoint.anglesPositionType)
    {
    case 0:
    {
        _vpm.dual_atenna_status=0;
        break;
    }
    case 8:
    {
        _vpm.dual_atenna_status=1;
        break;
    }
    case 16:
    {
        _vpm.dual_atenna_status=2;
        break;
    }
    case 17:
    {
        _vpm.dual_atenna_status=3;
        break;
    }
    case 18:
    {
        _vpm.dual_atenna_status=4;
        break;
    }
    case 19:
    {
        _vpm.dual_atenna_status=5;
        break;
    }
    case 20:
    {
        _vpm.dual_atenna_status=6;
        break;
    }
    case 32:
    {
        _vpm.dual_atenna_status=7;
        break;
    }
    case 33:
    {
        _vpm.dual_atenna_status=8;
        break;
    }
    case 34:
    {
        _vpm.dual_atenna_status=9;
        break;
    }
    case 48:
    {
        _vpm.dual_atenna_status=10;
        break;
    }
    case 50:
    {
        _vpm.dual_atenna_status=11;
        break;
    }
    case 64:
    {
        _vpm.dual_atenna_status=12;
        break;
    }
    case 65:
    {
        _vpm.dual_atenna_status=12;
        break;
    }
    case 68:
    {
        _vpm.dual_atenna_status=12;
        break;
    }
    case 69:
    {
        _vpm.dual_atenna_status=12;
        break;
    }
    case 77:
    {
        _vpm.dual_atenna_status=12;
        break;
    }
    case 78:
    {
        _vpm.dual_atenna_status=12;
        break;
    }


    }

    _vpm.rtk_fixed_status=(_basepoint.align_status=="NARROW_INT")?1:0;

    big_uint16_t position_system_error=0;
    position_system_error=(_basepoint.usw&0x00ff)|position_system_error;
    _vpm.position_system_error=position_system_error;
    _vpm.voltage_status=(_basepoint.usw>>8)&0x03;
    _vpm.magnetic_exceed=(_basepoint.usw>>13)&0x01;
    _vpm.temprature_exceed=(_basepoint.usw>>14)&0x01;
    _vpm.temprature_exceed=(_basepoint.usw>>15)&0x01;
    _vpm.calibration_result=(_basepoint.usw>>16)&0x01;

    if(((_basepoint.usw>>10)&0x07)==0)
        _vpm.angle_speed_exceed=0;
    else if(((_basepoint.usw>>10)&0x07)==1)
        _vpm.angle_speed_exceed=1;
    else if(((_basepoint.usw>>10)&0x07)==2)
        _vpm.angle_speed_exceed=2;
    else if(((_basepoint.usw>>10)&0x07)==4)
        _vpm.angle_speed_exceed=3;

    _tvpm.vpm=_vpm;

    sys->telecom->setVehiclePosMessage(_tvpm);


}
