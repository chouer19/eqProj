#ifndef PUBCLASS_H
#define PUBCLASS_H

#include<QSerialPort>
#include"typedefs.h"

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned char       byte;


struct SERIALPORT_WITH_Time
{
    qint64 timeticks;
    QByteArray data;

    SERIALPORT_WITH_Time()
    {
        timeticks=0;
        data.clear();
    }
};

struct INS_OPVT2AHR_ORIGIDATA
{
    UINT16 Heading;		//航向角
    INT16 Pitch;			//俯仰角
    INT16 Roll;			//侧倾角/横滚角
    int GyroX;			//陀螺角速度X
    int GyroY;			//陀螺角速度Y
    int GyroZ;			//陀螺角速度Z
    int AccX;			//加速度X
    int AccY;			//加速度Y
    int AccZ;			//加速度Z
    INT16 MagX;			//磁场X
    INT16 MagY;			//磁场Y
    INT16 MagZ;			//磁场Z
    UINT16 USW;			//uint状态
    UINT16 Vinp;			//供电电压
    INT16 Temper;			//温度
    long Latitude;			//纬度
    long Longitude;			//经度
    int Altitude;			//海拔
    int EastSpeed;			//东向速度
    int NorthSpeed;		//北向速度
    int VerticalSpeed;		//垂直速度
    long LatitudeGNSS;		//
    long LongitudeGNSS;		//
    int AltitudeGNSS;		//
    int HorizontalSpeed;	//
    UINT16 TrackOverGround;//
    int VerticalSpeed2;	//
    UINT ms_gps;			//毫秒
    byte GNSS_info1;		//
    byte GNSS_info2;		//
    byte solnSVs;			//卫星数
    UINT16 V_latency;		//速度时间标签延迟时间
    byte AnglesPositionType;//
    UINT16 HeadingGNSS;	//
    INT16 latencyMsHead;		//
    INT16 latencyMsPos;	//
    INT16 latencyMsVel;	//
    UINT16 P_bar;			//
    int H_bar;				//
    byte NewGPS;			//
};

struct INS_OPVT2AHR_DATA
{
    float heading;		//航向角
    float pitch;			//俯仰角
    float roll;			//侧倾角/横滚角

    float gyroX;			//陀螺角速度X
    float gyroY;			//陀螺角速度Y
    float gyroZ;			//陀螺角速度Z

    float accX;			//加速度X
    float accY;			//加速度Y
    float accZ;			//加速度Z

    float magX;			//磁场X
    float magY;			//磁场Y
    float magZ;			//磁场Z

    UINT16 usw;			//uint状态

    float vInp;			//供电电压
    float temper;			//温度

    float latitude;			//纬度
    float longitude;			//经度
    float altitude;			//海拔

    float eastSpeed;			//东向速度
    float northSpeed;		//北向速度
    float verticalSpeed;		//垂直速度

    float latitudeGNSS;		//
    float longitudeGNSS;		//
    float altitudeGNSS;		//

    float horizontalSpeed;	//
    float trackOverGround;//
    float verticalSpeed2;	//

    uint32_t msGps;			//毫秒

    uint8_t GNSSInfo1;		//
    uint8_t GNSSInfo2;		//

    uint8_t solnSVs;			//卫星数
    float vLatency;		//速度时间标签延迟时间
    uint8_t anglesPositionType;//

    float headingGNSS;	//
    INT16 latencyMsHead;		//
    INT16 latencyMsPos;	//
    INT16 latencyMsVel;	//
    float pBar;			//
    float hBar;				//
    uint8_t newGPS;			//
};

struct SERIALPORT_COM_PARA
{
    qint64 baudRate;
    QSerialPort::Parity parity;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;

    SERIALPORT_COM_PARA()
    {
        baudRate=QSerialPort::Baud115200;
        parity=QSerialPort::NoParity;
        dataBits =QSerialPort::Data8;
        stopBits=QSerialPort::OneStop;
        flowControl=QSerialPort::NoFlowControl;
    }


};

#endif // PUBCLASS_H
