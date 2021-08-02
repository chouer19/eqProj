/**
 * File Name: svc_interface.h
 * Create on: Aug 25, 2017
 *    Author: LiFei
 *   Version: V1.0, Aug 25, 2017, LiFei
 */

#ifndef SVC_INTERFACE_H_
#define SVC_INTERFACE_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */

#include"pub_types.h"
#include <boost/thread/thread.hpp>
#include"header/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"
#include"header/system/ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
#include"header/system/ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp"
#include"header/system/ser_outside_xc/eqDriving/v2x/SingletonAuxiliaryClient.hpp"
#include"header/system/ser_outside_xc/eqDriving/sensor/GpsManager.hpp"

using namespace eqDriving;
using namespace sensor;

/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */



/*
 * =====================================================================
 * Global type definitions ('typedef')
 * =====================================================================
 */
/* task management */
DeclareInterface(ITASKLIST)
	virtual bool empty() = 0; 
	virtual Node<TASK*>* push_back(TASK *data) = 0;
	virtual void erase(Node<TASK*>* pos) = 0;
	virtual TASK* data(Node<TASK*>* pos) = 0;
	virtual void clear() = 0;
	virtual size_t size() = 0; 
    virtual RUN_CORE getCore() = 0;
    virtual void exec() = 0;
EndInterface

struct tsk_info_t
{
    tsk_info_t()
        :is_task_running(0)
        ,to_control(0)
        ,real_period(0)
        ,over_time_duty(0)
        ,err_code(0)
    {}
    tsk_info_t(uint8_t _itr, uint8_t _tc, size_t _rp, float _otd, err_t _ec)
        :is_task_running(_itr)
        ,to_control(_tc)
        ,real_period(_rp)
        ,over_time_duty(_otd)
        ,err_code(_ec)
    {}
    uint8_t is_task_running;
    uint8_t to_control;
    size_t real_period;
    float over_time_duty;
    err_t err_code;
};


struct INTERNAL_WARNING
{
    INTERNAL_WARNING()
        :newWarning(false)
        ,warningMessage("NULL")
    {}

    bool newWarning;
    string warningMessage;
};

/* test */
DeclareInterface(ITEST)
	virtual void test_probe(const uint8_t &_val) = 0;
EndInterface




DeclareBasedInterface(ITELECOM,TASK)
    ITELECOM()
    :TASK(pub_time,50)
    {}
    ITELECOM(CCTIME *_time, uint8_t _period)
        :TASK(_time, _period)
    {}
    virtual void install()=0;
    virtual void setUtcTime(utc_time _utcTime)=0;
    virtual void setDeviceStatus(uint8_t _deviceStatus)=0;
    virtual void setLoadFinish(bool _ishard)=0;
    virtual void setVehiclePosMessage(telecom_vehicle_pos_message _vpm)=0;
    virtual TERMINAL_INFO getTerminalInfo()=0;
    virtual TERMINAL_CONFIG getTerminalConfig()=0;
    virtual void setDriverNum(string _driverNum)=0;
    virtual void setlogin(bool _login)=0;
    virtual void setlogout(bool _logout)=0;
    virtual void setErrorDeclare(bool _errorDeclare)=0;
    virtual void setErrorType(int _errorType)=0;
    virtual void setErrorDescription(string _errorDescription)=0;
    virtual void setDelayRequest(DELAY_REQUEST _delayRequest)=0;
    virtual void setTerminalRequest(TERMINAL_REQUEST _terminalRequest)=0;
    virtual void setTerminalConfig(TERMINAL_CONFIG _terminalConfig)=0;
    virtual void setWriteConfig(bool _writeConfig)=0;
    virtual getRequestStatus_respond_t getRequestAnswer()=0;
    virtual LOAD_POS getLoadposition()=0;
    virtual LOAD_POS getAutoLoadposition()=0;
    virtual void setLoadposition(LOAD_POS _loadposition)=0;
    virtual void setNewLoadPos(LOAD_POS_UPDATE _newLoadPos)=0;
    virtual map_data_packet_t getMapData()=0;
    virtual vector<point_lidarmap_t> getLeftBoundary()=0;
    virtual vector<point_lidarmap_t> getRightBoundary()=0;
    virtual surrd_vehicle_packet_t getSurrdVehicleInfo()=0;
    virtual cloud_info getNewMessage()=0;
    virtual void setForkliftSampleGisPacket(FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket)=0;
    virtual void setCmdInfo(CMD_INFO _cmdInfo)=0;
    virtual void setSurrdTruckInfoReq(SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq)=0;
    virtual void setSurrdTruckCtrlReq(SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq)=0;
    virtual getVehicleTask_respond_t getSurrdTruckInfo()=0;
    virtual INTERNAL_WARNING getInternalWarning()=0;
    virtual bool getLoadPosCalibrated()=0;

EndInterface




/****************************Service*************************************/

class SYSTEM
{
public:
    SYSTEM();
    virtual ~SYSTEM();
	
    ITASKLIST *taskList_p[3];
    ITELECOM *telecom;
};

/*
 * =====================================================================
 * Global constant declarations ('extern', definiton&comment in C source)
 * =====================================================================
 */


/*
 * =====================================================================
 * Global variable declarations ('extern', definition&comment in C source)
 * =====================================================================
 */



/*
 * =====================================================================
 * Global function prototypes ('extern', definition&comment in C source)
 * =====================================================================
 */


#endif /* SVC_INTERFACE_H_ */
