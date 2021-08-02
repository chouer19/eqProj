#include "header/system/sys_telecom.h"

SingletonClient *SingletonClient::instance=0;
eqDriving::SingletonAuxiliaryClient *eqDriving::SingletonAuxiliaryClient::instance=0;


ITELECOM_AUXILIARY::ITELECOM_AUXILIARY(ITASKLIST *_tlist)
    :ITELECOM(pub_time, TASK_PERIOD_TELECOM)
{
    taskList = _tlist;
    SingletonClient *s=s->getInstance();
    eqDriving::SingletonAuxiliaryClient *sa=sa->getInstance();
    sa->vehicleVin(s->get_vin());

    TERMINAL_CONFIG _terminalConfig;
    _terminalConfig.binding_device_num=sa->vehicleVin();
    _terminalConfig.sim_card_num=sa->simId();
    _terminalConfig.terminal_code=sa->passwd();
    _terminalConfig.terminal_num=sa->terminalNumber();
    _terminalConfig.terminal_type=TERMINAL_TYPE(sa->vehicleType());
    setTerminalConfig(_terminalConfig);

    TERMINAL_INFO _terminaInfo;
    _terminaInfo.device_type=TERMINAL_TYPE(sa->vehicleType());
    _terminaInfo.config=_terminalConfig;
    setTerminalInfo(_terminaInfo);
}

ITELECOM_AUXILIARY::~ITELECOM_AUXILIARY()
{
    taskList->erase(taskId);
    m_isAlive = false;
    m_thread.join();
    t1.join();
    t2.join();
    t3.join();
    t_tcp_send.join();
    t_tcp_recv.join();
    t_ntrip.join();
}

void ITELECOM_AUXILIARY::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = taskList->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);

    SingletonClient *s=s->getInstance();
    t1=std::thread(&SingletonClient::read_thread,s);
    t2=std::thread(&SingletonClient::write_thread,s);
    t3=std::thread(&SingletonClient::respond_thread,s);
    t_tcp_send=std::thread(tcp_send_thread, this);
    t_tcp_recv=std::thread(tcp_recv_thread, this);
    t_ntrip=std::thread(ntrip_thread, this);
}

void ITELECOM_AUXILIARY::tcp_send_thread(void *_P)
{
    ITELECOM_AUXILIARY *_this = (ITELECOM_AUXILIARY*)_P;
    _this->tcpSendTask();
}

void ITELECOM_AUXILIARY::tcpSendTask()
{
    SingletonClient *s=s->getInstance();
    while(m_isAlive)
    {
        if(pub_time->getDiffTimeMs(lastSendTicks250ms)>=250)
        {
            lastSendTicks250ms=pub_time->getTimeMs();
            //send message
            if(!s->jianquan_valid())
                s->login();
            else
            {
                if(pub_time->getDiffTimeMs(lastSendTicks1s)>=1000)
                {
                    lastSendTicks1s=pub_time->getTimeMs();
                    if(getVehiclePosMessage().message_valid)
                        s->report_message(getVehicleEgoMessage(),getEngineMessage(),getVehiclePosMessage()
                                          ,getErrorMessage(),getVehicleEgoAppendixMessage());
                }

                if(pub_time->getDiffTimeMs(lastSendTicks5s)>=5000)
                {
                    lastSendTicks5s=pub_time->getTimeMs();
                    s->rap();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ITELECOM_AUXILIARY::tcp_recv_thread(void *_P)
{
    ITELECOM_AUXILIARY *_this = (ITELECOM_AUXILIARY*)_P;
    _this->tcpRecvTask();
}

void ITELECOM_AUXILIARY::InserPointsInTwoPoints(point_lidarmap_t firstpoints, point_lidarmap_t secondpoints, vector<point_lidarmap_t> &_des)
{
    point_lidarmap_t pp;
    double delta_x=fabs(firstpoints.p.x -secondpoints.p.x);
    double delta_y=fabs(firstpoints.p.y -secondpoints.p.y);
    double deltaxy=0.2;
    double safe_distance=30;
    if(delta_x >= delta_y && delta_x <=safe_distance)
    {
        double x_min=MIN(firstpoints.p.x,secondpoints.p.x);
        double x_max=MAX(firstpoints.p.x,secondpoints.p.x);
        for(double x=x_min; x<x_max; x=x+deltaxy)
        {
            pp.p.x=x;
            pp.p.y=(x-firstpoints.p.x)*(secondpoints.p.y-firstpoints.p.y)/(secondpoints.p.x-firstpoints.p.x +0.000001)+firstpoints.p.y;
            pp.p.z=(firstpoints.p.z +secondpoints.p.z)/2.0;
            pp.index=firstpoints.index;
            _des.push_back(pp);
        }

    }
    else if(delta_y <=safe_distance)
    {
        double y_min=MIN(firstpoints.p.y,secondpoints.p.y);
        double y_max=MAX(firstpoints.p.y,secondpoints.p.y);
        for(double y=y_min; y<y_max; y=y+deltaxy)
        {
            pp.p.y=y;
            pp.p.x=(y-firstpoints.p.y)*(secondpoints.p.x-firstpoints.p.x)/(secondpoints.p.y-firstpoints.p.y +0.000001)+firstpoints.p.x;
            pp.p.z=(firstpoints.p.z +secondpoints.p.z)/2.0;
            pp.index=firstpoints.index;
            _des.push_back(pp);
        }
    }
}

void ITELECOM_AUXILIARY::setSurrdVehicleInfo(surrd_vehicle_packet_t _surrd_vehicle_info)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrd_vehicle_info=_surrd_vehicle_info;
}

void ITELECOM_AUXILIARY::setNewMessage(cloud_info _newMessage)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    newMessage=_newMessage;
}

FORKLIFT_SAMPLE_GIS_PACKET ITELECOM_AUXILIARY::getForkliftSampleGisPacket()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return forkliftSampleGisPacket;
}

CMD_INFO ITELECOM_AUXILIARY::getCmdInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return cmdInfo;
}

SURRD_TRUCK_INFO_REQ ITELECOM_AUXILIARY::getSurrdTruckInfoReq()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrdTruckInfoReq;
}

SURRD_TRUCK_CTRL_REQ ITELECOM_AUXILIARY::getSurrdTruckCtrlReq()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrdTruckCtrlReq;
}

void ITELECOM_AUXILIARY::setSurrdTruckInfo(getVehicleTask_respond_t _surrdTruckInfo)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrdTruckInfo=_surrdTruckInfo;
}

void ITELECOM_AUXILIARY::setInternalWarning(INTERNAL_WARNING _internalWarning)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    internalWarning=_internalWarning;
}

void ITELECOM_AUXILIARY::setLoadPosCalibrated(bool _loadPosCalibrated)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadPosCalibrated=_loadPosCalibrated;
}

bool ITELECOM_AUXILIARY::getLoadPosCalibrated()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadPosCalibrated;
}

void ITELECOM_AUXILIARY::ReadLidarMapFromServer(vector<map_road> &lidar_map_road)
{
    vector<point_lidarmap_t> left_boundary_points;
    vector<point_lidarmap_t> right_boundary_points;
    point_lidarmap_t p_lidar;
    if(!lidar_map_road.empty())
    {
        int size_lidar_map_road_i=lidar_map_road.size();
        for(int i=0;i<size_lidar_map_road_i;i++)
        {
            //            if(lidar_map_road[i].type <=3 || lidar_map_road[i].type ==7)
            {
                int size_lidar_map_road_j=lidar_map_road[i].points.size();
                p_lidar.index = (lidar_map_road[i].type ==3)?3:1;
                for(int j=0;j<size_lidar_map_road_j;j++)
                {
                    p_lidar.p.x = lidar_map_road[i].points[j].x;
                    p_lidar.p.y = lidar_map_road[i].points[j].y;
                    p_lidar.p.z = lidar_map_road[i].points[j].z;

                    int end_point=(i == size_lidar_map_road_i-1)?(size_lidar_map_road_j-1):size_lidar_map_road_j;
                    if( j>0 && j<end_point)
                    {
                        InserPointsInTwoPoints(lastslidarmappoint,p_lidar,left_boundary_points);
                    }
                    lastslidarmappoint=p_lidar;
                    left_boundary_points.push_back(p_lidar);


                }
            }
        }
    }


    setLeftBoundary(left_boundary_points);
    setRightBoundary(right_boundary_points);

}

void ITELECOM_AUXILIARY::tcpRecvTask()
{
    SingletonClient *s=s->getInstance();
    while(m_isAlive)
    {
        //set utc time
        QDate date=QDate::currentDate();
        QTime time=  QTime::currentTime();
        utc_time _utcTime;
        _utcTime.year=date.year();
        _utcTime.month=date.month();
        _utcTime.day=date.day();
        _utcTime.hour=time.hour();
        _utcTime.minute=time.minute();
        _utcTime.second=time.second();
        _utcTime.millisecond=time.msec();
        s->update_time(_utcTime);

        if(s->jianquan_valid())
        {
            //get map
            static int countNumber=0;
            static bool ismapread=false;
            if(s->read_ok())
            {
                map_data_packet_t _mapData=s->read_map_boundary();
                setMapData(_mapData);
                ismapread=true;
                countNumber=0;

                bool is_version_same=true;
                for(int i=0;i<23;i++)
                {
                    if(map_lidar_version[i] != _mapData.version[i])
                    {
                        is_version_same=false;
                        break;
                    }
                }
                if(!is_version_same)
                {
                    memcpy(map_lidar_version,_mapData.version,23);
                    ReadLidarMapFromServer(_mapData.roads);
                }

            }
            else if(!ismapread)
            {
                if(countNumber>=200)
                {
                    countNumber=0;
                    s->get_map();
                }
                else
                    countNumber++;
            }

            //get message
            cloud_info _cloudInfo= s->get_info();
            if(_cloudInfo.new_message_flag)
            {
                switch(_cloudInfo.message_packet.type)
                {
                case 1:
                {
                    ismapread=false;
                    break;
                }
                case 2:
                {
                    setNewMessage(_cloudInfo);
                    break;
                }
                }
            }


            //surrd vehicle
            surrd_vehicle_packet_t _surrd_vehicle_info=s->surround_vehicles();
//            surrd_vehicle_packet_t _surrd_vehicle_info;
//            _surrd_vehicle_info.number
            setSurrdVehicleInfo(_surrd_vehicle_info);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ITELECOM_AUXILIARY::ntrip_thread(void *_P)
{
    ITELECOM_AUXILIARY *_this = (ITELECOM_AUXILIARY*)_P;
    _this->ntripTask();
}

void ITELECOM_AUXILIARY::ntripTask()
{
    SimpleSerial serial("/dev/ttyS1", 115200);
//    SimpleSerial serial("/dev/ttyUSB1", 115200);

    SingletonClient *s = s->getInstance();

    while(m_isAlive)
    {
        rtcm_packet_t rtcm = s->rtcm();
        if(rtcm.is_new)
        {
            cout << "writing rtcm to serial port ......\n";
            serial.writeBytes(rtcm.data);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

utc_time ITELECOM_AUXILIARY::getUtcTime()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return utcTime;
}

uint8_t ITELECOM_AUXILIARY::getDeviceStatus()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return deviceStatus;
}

bool ITELECOM_AUXILIARY::getLoadFinish()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadFinish;
}

bool ITELECOM_AUXILIARY::clearLoadFinish()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadFinish=false;
}


bool ITELECOM_AUXILIARY::getIsHard()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return isHard;
}

telecom_vehicle_ego_message ITELECOM_AUXILIARY::getVehicleEgoMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vem;
}

telecom_engine_message ITELECOM_AUXILIARY::getEngineMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return em;
}

telecom_vehicle_pos_message ITELECOM_AUXILIARY::getVehiclePosMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vpm;
}

telecom_error_message ITELECOM_AUXILIARY::getErrorMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return erm;
}

telecom_vehicle_ego_appendix_message ITELECOM_AUXILIARY::getVehicleEgoAppendixMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return veam;
}

void ITELECOM_AUXILIARY::setTerminalInfo(TERMINAL_INFO _terminalInfo)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalInfo=_terminalInfo;
}

void ITELECOM_AUXILIARY::setTerminalConfig(TERMINAL_CONFIG _terminalConfig)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalConfig=_terminalConfig;
}

void ITELECOM_AUXILIARY::setWriteConfig(bool _writeConfig)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    writeConfig=_writeConfig;
}

getRequestStatus_respond_t ITELECOM_AUXILIARY::getRequestAnswer()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return requestAnswer;
}

LOAD_POS ITELECOM_AUXILIARY::getLoadposition()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadposition;
}

LOAD_POS ITELECOM_AUXILIARY::getAutoLoadposition()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return autoLoadposition;
}

void ITELECOM_AUXILIARY::setLoadposition(LOAD_POS _loadposition)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadposition=_loadposition;
}

void ITELECOM_AUXILIARY::setNewLoadPos(LOAD_POS_UPDATE _newLoadPos)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    newLoadPos=_newLoadPos;
}

map_data_packet_t ITELECOM_AUXILIARY::getMapData()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return mapData;
}

vector<point_lidarmap_t> ITELECOM_AUXILIARY::getLeftBoundary()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return left_boundary_position_read;
}

vector<point_lidarmap_t> ITELECOM_AUXILIARY::getRightBoundary()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return right_boundary_position_read;
}

surrd_vehicle_packet_t ITELECOM_AUXILIARY::getSurrdVehicleInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrd_vehicle_info;
}

cloud_info ITELECOM_AUXILIARY::getNewMessage()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    cloud_info _newMessage=newMessage;
    newMessage.new_message_flag=false;
    return _newMessage;
}

void ITELECOM_AUXILIARY::setForkliftSampleGisPacket(FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    forkliftSampleGisPacket=_forkliftSampleGisPacket;
}

void ITELECOM_AUXILIARY::setCmdInfo(CMD_INFO _cmdInfo)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    cmdInfo=_cmdInfo;
}

void ITELECOM_AUXILIARY::setSurrdTruckInfoReq(SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrdTruckInfoReq=_surrdTruckInfoReq;
}

void ITELECOM_AUXILIARY::setSurrdTruckCtrlReq(SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrdTruckCtrlReq=_surrdTruckCtrlReq;
}

getVehicleTask_respond_t ITELECOM_AUXILIARY::getSurrdTruckInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrdTruckInfo;
}

INTERNAL_WARNING ITELECOM_AUXILIARY::getInternalWarning()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    INTERNAL_WARNING _internalWarning=internalWarning;
    internalWarning.newWarning=false;
    return _internalWarning;
}

string ITELECOM_AUXILIARY::getDriverNum()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return driverNum;
}

bool ITELECOM_AUXILIARY::getlogin()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return login;
}

bool ITELECOM_AUXILIARY::getlogout()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return logout;
}

bool ITELECOM_AUXILIARY::getErrorDeclare()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return errorDeclare;
}

int ITELECOM_AUXILIARY::getErrorType()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return errorType;
}

string ITELECOM_AUXILIARY::getErrorDescription()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return errorDescription;
}

DELAY_REQUEST ITELECOM_AUXILIARY::getDelayRequest()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return delayRequest;
}

TERMINAL_REQUEST ITELECOM_AUXILIARY::getTerminalRequest()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalRequest;
}

bool ITELECOM_AUXILIARY::getWriteConfig()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return writeConfig;
}

void ITELECOM_AUXILIARY::setRequestAnswer(getRequestStatus_respond_t _requestAnswer)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    requestAnswer=_requestAnswer;
}

LOAD_POS_UPDATE ITELECOM_AUXILIARY::getNewLoadPos()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return newLoadPos;
}

void ITELECOM_AUXILIARY::setMapData(map_data_packet_t _mapData)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    mapData=_mapData;
}

void ITELECOM_AUXILIARY::setAutoLoadposition(LOAD_POS _autoLoadposition)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    autoLoadposition=_autoLoadposition;
}

void ITELECOM_AUXILIARY::setLeftBoundary(vector<point_lidarmap_t> _left_boundary_position_read)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    left_boundary_position_read=_left_boundary_position_read;
}

void ITELECOM_AUXILIARY::setRightBoundary(vector<point_lidarmap_t> _right_boundary_position_read)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    right_boundary_position_read=_right_boundary_position_read;
}


void ITELECOM_AUXILIARY::setErrorType(int _errorType)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    errorType=_errorType;
}

void ITELECOM_AUXILIARY::setErrorDescription(string _errorDescription)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    errorDescription=_errorDescription;
}

void ITELECOM_AUXILIARY::setDelayRequest(DELAY_REQUEST _delayRequest)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    delayRequest=_delayRequest;
}

void ITELECOM_AUXILIARY::setTerminalRequest(TERMINAL_REQUEST _terminalRequest)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalRequest=_terminalRequest;
}


void ITELECOM_AUXILIARY::setUtcTime(utc_time _utcTime)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    utcTime=_utcTime;
}

void ITELECOM_AUXILIARY::setDeviceStatus(uint8_t _deviceStatus)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    deviceStatus=_deviceStatus;
}

void ITELECOM_AUXILIARY::setLoadFinish(bool _ishard)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    isHard=_ishard;
    loadFinish=true;
}

void ITELECOM_AUXILIARY::setVehiclePosMessage(telecom_vehicle_pos_message _vpm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vpm=_vpm;
}

TERMINAL_INFO ITELECOM_AUXILIARY::getTerminalInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalInfo;
}

TERMINAL_CONFIG ITELECOM_AUXILIARY::getTerminalConfig()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalConfig;
}

void ITELECOM_AUXILIARY::setDriverNum(string _driverNum)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    driverNum=_driverNum;
}

void ITELECOM_AUXILIARY::setlogin(bool _login)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    login=_login;
}

void ITELECOM_AUXILIARY::setlogout(bool _logout)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    logout=_logout;
}

void ITELECOM_AUXILIARY::setErrorDeclare(bool _errorDeclare)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    errorDeclare=_errorDeclare;
}


void ITELECOM_AUXILIARY::task_func()
{

    SingletonClient *s=s->getInstance();
    eqDriving::SingletonAuxiliaryClient *sa=sa->getInstance();

    if(getWriteConfig())
    {
        TERMINAL_CONFIG _terminalConfig=getTerminalConfig();
        setWriteConfig(false);
        sa->terminalNumber(_terminalConfig.terminal_num);
        sa->vehicleType(int(_terminalConfig.terminal_type));
        sa->simId(_terminalConfig.sim_card_num);
        sa->vehicleVin(_terminalConfig.binding_device_num);
        sa->updateParams();
    }


    if(s->jianquan_valid())
    {
        //http
        if(pub_time->getDiffTimeMs(lastSendTicks500ms)>=1000)
        {
            lastSendTicks500ms=pub_time->getTimeMs();


            //driver log in
            bool isLoginOk=getlogin();
            if(!isLoginOk)
            {
                TERMINAL_INFO _terminalInfo=getTerminalInfo();
                login_respond_t loginRespond= sa->login(getDriverNum(),_terminalInfo.config.binding_device_num,int(_terminalInfo.device_type));
                if("200"==loginRespond.code)
                {

                    setlogin(true);
                    _terminalInfo.driver_log_status=1;
                    _terminalInfo.device_num=loginRespond.vehicle_number;
                    _terminalInfo.driver_name=loginRespond.driver_name;
                    _terminalInfo.driver_id=loginRespond.driver_id;
                    _terminalInfo.es.group_truck=loginRespond.vehicle_numbers;
                    _terminalInfo.es.material_type=loginRespond.item_type;
                    _terminalInfo.es.work_platform=loginRespond.platform_id;
                    _terminalInfo.es.load_area_id=loginRespond.load_area_id;
                    _terminalInfo.base_point_offset=loginRespond.gps_xyz;
                    _terminalInfo.ref_point_offset=loginRespond.spoon_xyz;
                    _terminalInfo.ground_origin=loginRespond.origin_llh;
                    _terminalInfo.heading_offset=loginRespond.deviationAngle;
                    _terminalInfo.parasGet=true;
                    setTerminalInfo(_terminalInfo);

                }
                else if("201"==loginRespond.code)
                {
                    setlogin(true);
                }
                else if("500"==loginRespond.code)
                {
                    setlogin(true);
                }
            }

            //driver log out
            bool isLogOutOk=getlogout();
            if(!isLogOutOk)
            {
                TERMINAL_INFO _terminalInfo=getTerminalInfo();
                string result= sa->logout(_terminalInfo.config.binding_device_num);
                if("200"==result)
                {
                    TERMINAL_INFO _terminalInfo_temp;
                    _terminalInfo_temp.config=getTerminalConfig();
                    _terminalInfo_temp.device_type=_terminalInfo_temp.config.terminal_type;
                    setlogout(true);
                    setTerminalInfo(_terminalInfo_temp);
//                    getRequestStatus_respond_t _requestAnswer;
//                    setRequestAnswer(_requestAnswer);
                    isSendRequest=true;
                }
                else if("500"==result)
                {
                    setlogout(true);
                }

            }

            //error declare
            bool _isErrorDeclareOk=getErrorDeclare();
            if(!_isErrorDeclareOk)
            {
                TERMINAL_INFO _info=getTerminalInfo();
                string _result=sa->faultRequest(_info.config.binding_device_num,int(_info.device_type),getErrorType(),getErrorDescription(),_info.driver_id);
                if(_result=="200")
                {
                    setErrorDeclare(true);
                    isSendRequest=true;
                }
                else if("500"==_result)
                {
                    setErrorDeclare(true);
                }
            }

            //delay request
            DELAY_REQUEST _delayRequest=getDelayRequest();
            if(!_delayRequest.isDelayRequestOk)
            {
                TERMINAL_INFO _info=getTerminalInfo();
                string _result=sa->delayedRequest(_info.config.binding_device_num,int(_info.device_type),_delayRequest.delayType,_delayRequest.delayMinutes,_info.driver_id);
                if(_result=="200")
                {
                    _delayRequest.isDelayRequestOk=true;
                    setDelayRequest(_delayRequest);
                    isSendRequest=true;
                }
                else if("500"==_result)
                {
                    _delayRequest.isDelayRequestOk=true;
                    setDelayRequest(_delayRequest);
                }
            }

            //getRequestStatus
            {
                TERMINAL_INFO _terminalInfo=getTerminalInfo();
//                getRequestStatus_respond_t _requestAnswer=getRequestAnswer();

                if(_terminalInfo.driver_log_status==1&&isSendRequest)
                {
                    getRequestStatus_respond_t requestRepond= sa->getRequestStatus(_terminalInfo.config.binding_device_num,_terminalInfo.driver_id);
                    setRequestAnswer(requestRepond);
                    if(requestRepond.result=="NULL"||requestRepond.result==""||requestRepond.result=="1"||requestRepond.result=="2")
                        isSendRequest=false;

                }
            }

            //device status
            static uint8_t device_work_status=0;
            static bool isSendDeviceWorkStatusOk=true;
            uint8_t _deviceStatus= getDeviceStatus();

            {
                TERMINAL_INFO _terminalInfo=getTerminalInfo();

                if(_deviceStatus==1)
                {
                    _terminalInfo.device_status=1;
                    isSendRequest=false;
                }
                else if(getRequestAnswer().result=="1")
                {
                    if(getRequestAnswer().request_type=="1"
                            ||getRequestAnswer().request_type=="2"
                            ||getRequestAnswer().request_type=="3"
                            ||getRequestAnswer().request_type=="4")
                    {
                        _terminalInfo.device_status=3;

                    }
                    else if(getRequestAnswer().request_type=="5"
                            ||getRequestAnswer().request_type=="6"
                            ||getRequestAnswer().request_type=="7"
                            ||getRequestAnswer().request_type=="8"
                            ||getRequestAnswer().request_type=="9"
                            ||getRequestAnswer().request_type=="10")
                    {
                        _terminalInfo.device_status=4;
                    }

                }
                setTerminalInfo(_terminalInfo);
                setDeviceStatus(_terminalInfo.device_status);

                if(_terminalInfo.device_status!=device_work_status)
                {
                    isSendDeviceWorkStatusOk=false;
                    device_work_status=_terminalInfo.device_status;
                }

                if(!isSendDeviceWorkStatusOk)
                {
                    string _driverId=getTerminalInfo().driver_id;
                    string _result=sa->updateVehicleStatus(device_work_status,_driverId);
                    if(_result=="200")
                        isSendDeviceWorkStatusOk=true;
                }

            }

            //terminate request
            TERMINAL_REQUEST _terminalRequest=getTerminalRequest();
            if(!_terminalRequest.isTcpTerminalRequestOk)
            {
                if(!s->assignment_request_ok())
                    s->request_assignment(_terminalRequest.request_type);
                else
                {
                    _terminalRequest.isTcpTerminalRequestOk=true;
                    setTerminalRequest(_terminalRequest);
                }
            }

            //load finish
            if(!_terminalRequest.isHttpTerminalRequestOk)
            {
                TERMINAL_INFO _info=getTerminalInfo();
                double  lontitude=getVehiclePosMessage().vpm.lontitude/pow(10,9);
                double latitude=getVehiclePosMessage().vpm.latitude/pow(10,9);
                double height=getVehiclePosMessage().vpm.height/10000-1000;
                point_3d_t _pos=point_3d_t(lontitude,latitude,height);
                loadFinish_respond_t _respond=sa->loadFinish(_info.config.binding_device_num,_info.driver_id,_terminalRequest.isHard,_pos);
                LOAD_POS _loadposition=getLoadposition();
                if(_respond.code=="200")
                {
                    _terminalRequest.isHttpTerminalRequestOk=true;
                    setTerminalRequest(_terminalRequest);


                    if(_loadposition.calibrate_mode==2)
                    {
                        LOAD_POS _respondLoadposition;
                        _respondLoadposition.basePoint.pos_lonlat.x=_respond.longitude;
                        _respondLoadposition.basePoint.pos_lonlat.y=_respond.latitude;
                        _respondLoadposition.basePoint.heading=_respond.azimuth;
                        _respondLoadposition.endpoint_lonlat=_respond.vertexs;
                        _respondLoadposition.isValid=true;

                        geographicLib_coordinate_transfer gct;
                        gct.setGroundOrigin(_info.ground_origin);
                        double temp;
                        gct.forward(_respondLoadposition.basePoint.pos_lonlat.y,_respondLoadposition.basePoint.pos_lonlat.x,0,
                                    _respondLoadposition.basePoint.pos_xy.x,_respondLoadposition.basePoint.pos_xy.y,temp);

                        _respondLoadposition.endpoint_xy.resize(_respondLoadposition.endpoint_lonlat.size());
                        for(size_t i=0;i<_respondLoadposition.endpoint_lonlat.size();i++)
                        {
                            gct.forward(_respondLoadposition.endpoint_lonlat[i].y,_respondLoadposition.endpoint_lonlat[i].x,0,
                                        _respondLoadposition.endpoint_xy[i].x,_respondLoadposition.endpoint_xy[i].y,temp);
                        }
                        setAutoLoadposition(_respondLoadposition);
                        setLoadPosCalibrated(true);
                    }
                    else
                        setLoadPosCalibrated(false);
                }
                else
                {
                    _terminalRequest.isHttpTerminalRequestOk=true;
                    setTerminalRequest(_terminalRequest);
                    setLoadPosCalibrated(false);

                    if(_loadposition.calibrate_mode==2)
                    {
                        LOAD_POS _respondLoadposition;
                        _respondLoadposition.isValid=false;
                        setAutoLoadposition(_respondLoadposition);

                        INTERNAL_WARNING _internalWarning;
                        _internalWarning.newWarning=true;
                        _internalWarning.warningMessage="自动标定失败，请手动标定或联系技术人员";
                        setInternalWarning(_internalWarning);
                    }
                }
            }

            //get load place
            {
                static bool isValidExist=true;
                LOAD_POS _loadposition=getLoadposition();
                TERMINAL_INFO _terminalInfo=getTerminalInfo();
                if((!_loadposition.isValid)&&isValidExist&&_terminalInfo.driver_log_status==1)
                {
                    TERMINAL_INFO _info=getTerminalInfo();
                    getLoadPlace_respond_t loadPlace=sa->getLoadPlace(_info.config.binding_device_num,_info.es.load_area_id);
                    if(loadPlace.code=="200"/*&&(!loadPlace.load_space_ids.empty())*/)
                    {
                        LOAD_POS _loadposition;
                        _loadposition.basePoint.pos_lonlat.x=loadPlace.longitude;
                        _loadposition.basePoint.pos_lonlat.y=loadPlace.latitude;
                        _loadposition.basePoint.heading=loadPlace.azimuth;
                        if(!loadPlace.load_space_ids.empty())
                            _loadposition.load_pos_id=loadPlace.load_space_ids.front();
                        _loadposition.isValid=true;
                        geographicLib_coordinate_transfer gct;
                        gct.setGroundOrigin(_info.ground_origin);
                        double temp;
                        gct.forward(_loadposition.basePoint.pos_lonlat.y,_loadposition.basePoint.pos_lonlat.x,0,
                                    _loadposition.basePoint.pos_xy.x,_loadposition.basePoint.pos_xy.y,temp);

                        nav_t basePoint;
                        basePoint.pos.x=_loadposition.basePoint.pos_xy.x;
                        basePoint.pos.y=_loadposition.basePoint.pos_xy.y;
                        basePoint.heading=_loadposition.basePoint.heading;

                        _loadposition.endpoint_xy.clear();
                        _loadposition.endpoint_lonlat.clear();

                        point_3d_t offsetfl(gct.lonF,gct.lat/2,0);
                        point_3d_t _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetfl,basePoint);
                        _loadposition.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

                        point_3d_t offsetfr(gct.lonF,-gct.lat/2,0);
                        _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetfr,basePoint);
                        _loadposition.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

                        point_3d_t offsetrr(-gct.lonR,-gct.lat/2,0);
                        _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetrr,basePoint);
                        _loadposition.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

                        point_3d_t offsetrl(-gct.lonR,gct.lat/2,0);
                        _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetrl,basePoint);
                        _loadposition.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

                        for(int i=0;i<_loadposition.endpoint_xy.size();i++)
                        {
                            point_3d_t _endpoint_lonlat;
                            gct.reverse(_loadposition.endpoint_xy[i].x,_loadposition.endpoint_xy[i].y,0,_endpoint_lonlat.y,_endpoint_lonlat.x,temp);
                            _loadposition.endpoint_lonlat.push_back(point_2d_t(_endpoint_lonlat.x,_endpoint_lonlat.y));

                        }


                        setLoadposition(_loadposition);
                    }
                    else
                        isValidExist=false;
                }
            }

            //confirm load place
            LOAD_POS_UPDATE _loadPosUpdate=getNewLoadPos();
            if(_loadPosUpdate.isNewLoadPos)
            {
                LOAD_POS _loadposition=getLoadposition();
                geographicLib_coordinate_transfer gct;
                gct.setGroundOrigin(getTerminalInfo().ground_origin);
                double temp;
                gct.reverse(_loadposition.basePoint.pos_xy.x,_loadposition.basePoint.pos_xy.y,0,
                            _loadposition.basePoint.pos_lonlat.y,_loadposition.basePoint.pos_lonlat.x,temp);

                _loadposition.endpoint_lonlat.clear();
                for(int i=0;i<_loadposition.endpoint_xy.size();i++)
                {
                    point_3d_t _endpoint_lonlat;
                    gct.reverse(_loadposition.endpoint_xy[i].x,_loadposition.endpoint_xy[i].y,0,
                                _endpoint_lonlat.y,_endpoint_lonlat.x,temp);
                    _loadposition.endpoint_lonlat.push_back(point_2d_t(_endpoint_lonlat.x,_endpoint_lonlat.y));
                }

                TERMINAL_INFO _info=getTerminalInfo();

                confirmLoadPlace_respond_t _respond;
                if(_loadPosUpdate.updateType==1)
                {
                    _respond=sa->confirmLoadPlace(_info.es.load_area_id,
                                                  _loadposition.load_pos_id,
                                                  _loadposition.basePoint.pos_lonlat.y,
                                                  _loadposition.basePoint.pos_lonlat.x,
                                                  _loadposition.basePoint.heading,
                                                  _loadposition.endpoint_lonlat,
                                                  true,
                                                  _loadposition.calibrate_mode,
                                                  _loadposition.load_mode,
                                                  _loadposition.coordinate);
                }
                else
                {
                    _respond=sa->confirmLoadPlace(_info.es.load_area_id,
                                                  _loadposition.load_pos_id,
                                                  _loadposition.basePoint.pos_lonlat.y,
                                                  _loadposition.basePoint.pos_lonlat.x,
                                                  _loadposition.basePoint.heading,
                                                  _loadposition.endpoint_lonlat,
                                                  false,
                                                  _loadposition.calibrate_mode,
                                                  _loadposition.load_mode,
                                                  _loadposition.coordinate);
                }

                if(_respond.code=="200")
                {
                    _loadPosUpdate.isNewLoadPos=false;
                    setNewLoadPos(_loadPosUpdate);
                    setLoadPosCalibrated(true);

                    if(_loadPosUpdate.updateType==1)
                        _loadposition.load_pos_id=_respond.load_space_id;


                    if(_loadposition.calibrate_mode==2)
                    {
                        LOAD_POS _respondLoadposition;
                        _respondLoadposition.basePoint.pos_lonlat.x=_respond.longitude;
                        _respondLoadposition.basePoint.pos_lonlat.y=_respond.latitude;
                        _respondLoadposition.basePoint.heading=_respond.azimuth;
                        _respondLoadposition.endpoint_lonlat=_respond.vertexs;
                        _respondLoadposition.isValid=true;

                        geographicLib_coordinate_transfer gct;
                        gct.setGroundOrigin(_info.ground_origin);
                        double temp;
                        gct.forward(_respondLoadposition.basePoint.pos_lonlat.y,_respondLoadposition.basePoint.pos_lonlat.x,0,
                                    _respondLoadposition.basePoint.pos_xy.x,_respondLoadposition.basePoint.pos_xy.y,temp);

                        _respondLoadposition.endpoint_xy.resize(_respondLoadposition.endpoint_lonlat.size());
                        for(size_t i=0;i<_respondLoadposition.endpoint_lonlat.size();i++)
                        {
                            gct.forward(_respondLoadposition.endpoint_lonlat[i].y,_respondLoadposition.endpoint_lonlat[i].x,0,
                                        _respondLoadposition.endpoint_xy[i].x,_respondLoadposition.endpoint_xy[i].y,temp);
                        }
                        setAutoLoadposition(_respondLoadposition);


                    }
                    else
                    {
                        setLoadposition(_loadposition);

                    }
                }
                else /*if(_respond.code=="500")*/
                {
                    _loadPosUpdate.isNewLoadPos=false;
                    setNewLoadPos(_loadPosUpdate);
                    setLoadPosCalibrated(false);

                    if(_loadposition.calibrate_mode==2)
                    {
                        LOAD_POS _respondLoadposition;
                        _respondLoadposition.isValid=false;
                        setAutoLoadposition(_respondLoadposition);

                        INTERNAL_WARNING _internalWarning;
                        _internalWarning.newWarning=true;
                        _internalWarning.warningMessage="自动标定失败，请手动标定或联系技术人员";
                        setInternalWarning(_internalWarning);
                    }
                    else
                    {

                        INTERNAL_WARNING _internalWarning;
                        _internalWarning.newWarning=true;
                        _internalWarning.warningMessage="手动标定失败，请联系技术人员";
                        setInternalWarning(_internalWarning);
                    }
                }






            }

            //unload Place Scan
            FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket=getForkliftSampleGisPacket();
            if(_forkliftSampleGisPacket.isNewMessage)
            {
                string _result=sa->unloadPlaceScan(_forkliftSampleGisPacket.start_time,_forkliftSampleGisPacket.end_time);
                if(_result=="200")
                {
                    _forkliftSampleGisPacket.isNewMessage=false;
                    setForkliftSampleGisPacket(_forkliftSampleGisPacket);
                }
            }

            //clearLoadPosition
            CMD_INFO _cmdInfo=getCmdInfo();
            if(_cmdInfo.isNewCmd)
            {
                TERMINAL_INFO _terminalInfo=getTerminalInfo();
                string _result=sa->clearLoadPosition(_terminalInfo.config.binding_device_num,_cmdInfo.cmd_type,_cmdInfo.cmd_destination);
                if(_result=="200")
                {
                    _cmdInfo.isNewCmd=false;
                    setCmdInfo(_cmdInfo);
                }
            }

            //surrd Truck Info Req
            SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq=getSurrdTruckInfoReq();
            if(_surrdTruckInfoReq.isNew)
            {
                getVehicleTask_respond_t _respond=sa->getVehicleTask(_surrdTruckInfoReq.vehicleNum);

                if(_respond.code=="200")
                {
                    _surrdTruckInfoReq.isNew=false;
                    setSurrdTruckInfoReq(_surrdTruckInfoReq);
                    setSurrdTruckInfo(_respond);

                }
            }

            //surrd Truck Ctrl Req
            SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq=getSurrdTruckCtrlReq();
            if(_surrdTruckCtrlReq.isNew)
            {
                string _result=sa->stopAroundVehicle(_surrdTruckCtrlReq.vehicleNum,_surrdTruckCtrlReq.ctrlType);

                if(_result=="200")
                {
                    _surrdTruckCtrlReq.isNew=false;
                    setSurrdTruckCtrlReq(_surrdTruckCtrlReq);
                }
            }

        }


    }


}
