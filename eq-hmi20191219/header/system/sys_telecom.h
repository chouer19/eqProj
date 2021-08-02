#ifndef SYS_TELECOM_H
#define SYS_TELECOM_H

#include"header/system/pub_types.h"
#include"sys_interface.hpp"

#include"header/system/ser_outside_xc/thirdParty/geographicLib/Geocentric.hpp"
#include"header/system/ser_outside_xc/thirdParty/geographicLib/LocalCartesian.hpp"
#include "header/system/ser_outside_xc/eqDriving/serial/SimpleSerial.hpp"

using namespace eqDriving;
using namespace io;


enum{ TASK_PERIOD_TELECOM = 10 };

class ITELECOM_AUXILIARY:implements ITELECOM
{
public:
    ITELECOM_AUXILIARY(ITASKLIST *_tlist);
    virtual ~ITELECOM_AUXILIARY();

    virtual void install();

    virtual void setUtcTime(utc_time _utcTime);
    virtual void setDeviceStatus(uint8_t _deviceStatus);
    virtual void setLoadFinish(bool _ishard);
    virtual void setVehiclePosMessage(telecom_vehicle_pos_message _vpm);

    virtual TERMINAL_INFO getTerminalInfo();
    virtual TERMINAL_CONFIG getTerminalConfig();
    virtual void setDriverNum(string _driverNum);
    virtual void setlogin(bool _login);
    virtual void setlogout(bool _logout);
    virtual void setErrorDeclare(bool _errorDeclare);
    virtual void setErrorType(int _errorType);
    virtual void setErrorDescription(string _errorDescription);
    virtual void setDelayRequest(DELAY_REQUEST _delayRequest);
    virtual void setTerminalRequest(TERMINAL_REQUEST _terminalRequest);
    virtual void setTerminalConfig(TERMINAL_CONFIG _terminalConfig);
    virtual void setWriteConfig(bool _writeConfig);
    virtual getRequestStatus_respond_t getRequestAnswer();
    virtual LOAD_POS getLoadposition();
    virtual LOAD_POS getAutoLoadposition();
    virtual void setLoadposition(LOAD_POS _loadposition);
    virtual void setNewLoadPos(LOAD_POS_UPDATE _newLoadPos);
    virtual map_data_packet_t getMapData();
    virtual vector<point_lidarmap_t> getLeftBoundary();
    virtual vector<point_lidarmap_t> getRightBoundary();
    virtual surrd_vehicle_packet_t getSurrdVehicleInfo();
    virtual cloud_info getNewMessage();
    virtual void setForkliftSampleGisPacket(FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket);
    virtual void setCmdInfo(CMD_INFO _cmdInfo);
    virtual void setSurrdTruckInfoReq(SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq);
    virtual void setSurrdTruckCtrlReq(SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq);
    virtual getVehicleTask_respond_t getSurrdTruckInfo();
    virtual INTERNAL_WARNING getInternalWarning();
    virtual bool getLoadPosCalibrated();

    TaskFuncDeclare(ITELECOM_AUXILIARY, telecom, (TASK_PERIOD_TELECOM+10))

private:
    std::mutex m_mutex;
    ITASKLIST *taskList;
    std::thread t1;
    std::thread t2;
    std::thread t3;
    std::thread t_tcp_send;
    std::thread t_tcp_recv;
    std::thread t_ntrip;
    static void tcp_send_thread(void *_P);
    void tcpSendTask();


    static void tcp_recv_thread(void *_P);
    void tcpRecvTask();


    static void ntrip_thread(void *_P);
    void ntripTask();

    utc_time utcTime;
    utc_time getUtcTime();

    uint8_t deviceStatus=0;
    uint8_t getDeviceStatus();

    bool loadFinish=false;
    bool getLoadFinish();
    bool clearLoadFinish();

    bool isHard=false;
    bool getIsHard();

    //
    telecom_vehicle_ego_message vem;
    telecom_vehicle_ego_message getVehicleEgoMessage();

    //
    telecom_engine_message em;
    telecom_engine_message getEngineMessage();

    //
    telecom_vehicle_pos_message vpm;
    telecom_vehicle_pos_message getVehiclePosMessage();

    //
    telecom_error_message erm;
    telecom_error_message getErrorMessage();

    //
    telecom_vehicle_ego_appendix_message veam;
    telecom_vehicle_ego_appendix_message getVehicleEgoAppendixMessage();

    uint64_t lastSendTicks250ms=0;
    uint64_t lastSendTicks500ms=0;
    uint64_t lastSendTicks1s=0;
    uint64_t lastSendTicks5s=0;

    TERMINAL_INFO terminalInfo;
    void setTerminalInfo(TERMINAL_INFO _terminalInfo);

    TERMINAL_CONFIG terminalConfig;

    string driverNum;
    string getDriverNum();

    bool login=true;
    bool getlogin();

    bool logout=true;
    bool getlogout();

    bool errorDeclare=true;
    bool getErrorDeclare();

    int errorType=0;
    int getErrorType();

    string errorDescription;
    string getErrorDescription();

    DELAY_REQUEST delayRequest;
    DELAY_REQUEST getDelayRequest();

    TERMINAL_REQUEST terminalRequest;
    TERMINAL_REQUEST getTerminalRequest();

    bool writeConfig;
    bool getWriteConfig();

    getRequestStatus_respond_t requestAnswer;
    void setRequestAnswer(getRequestStatus_respond_t _requestAnswer);

    LOAD_POS_UPDATE newLoadPos;
    LOAD_POS_UPDATE getNewLoadPos();

    map_data_packet_t mapData;
    void setMapData(map_data_packet_t _mapData);


    bool isSendRequest=true;

    LOAD_POS loadposition;

    LOAD_POS autoLoadposition;
    void setAutoLoadposition(LOAD_POS _autoLoadposition);

    point_lidarmap_t lastslidarmappoint;
    uint8_t map_lidar_version[23];

    vector<point_lidarmap_t> left_boundary_position_read;
    void setLeftBoundary(vector<point_lidarmap_t> _left_boundary_position_read);

    vector<point_lidarmap_t> right_boundary_position_read;
    void setRightBoundary(vector<point_lidarmap_t> _right_boundary_position_read);

    void ReadLidarMapFromServer(vector<map_road> &lidar_map_road);
    void InserPointsInTwoPoints(point_lidarmap_t firstpoints, point_lidarmap_t secondpoints, vector<point_lidarmap_t> &_des);

    surrd_vehicle_packet_t surrd_vehicle_info;
    void setSurrdVehicleInfo(surrd_vehicle_packet_t _surrd_vehicle_info);

    cloud_info newMessage;
    void setNewMessage(cloud_info _newMessage);

    FORKLIFT_SAMPLE_GIS_PACKET getForkliftSampleGisPacket();
    FORKLIFT_SAMPLE_GIS_PACKET forkliftSampleGisPacket;

    CMD_INFO cmdInfo;
    CMD_INFO getCmdInfo();

    SURRD_TRUCK_INFO_REQ surrdTruckInfoReq;
    SURRD_TRUCK_INFO_REQ getSurrdTruckInfoReq();

    SURRD_TRUCK_CTRL_REQ surrdTruckCtrlReq;
    SURRD_TRUCK_CTRL_REQ getSurrdTruckCtrlReq();

    getVehicleTask_respond_t surrdTruckInfo;
    void setSurrdTruckInfo(getVehicleTask_respond_t _surrdTruckInfo);

    INTERNAL_WARNING internalWarning;
    void setInternalWarning(INTERNAL_WARNING _internalWarning);

    bool loadPosCalibrated=false;
    void setLoadPosCalibrated(bool _loadPosCalibrated);
};

#endif // SYS_TELECOM_H
