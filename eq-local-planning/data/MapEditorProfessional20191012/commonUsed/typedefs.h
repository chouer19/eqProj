/**************************************************************************
 * FILE NAME: $RCSfile: typedefs.h,v $       COPYRIGHT (c) Freescale 2005 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file defines all of the data types for the Motorola header file.  *
 *========================================================================*
 * ORIGINAL AUTHOR: Jeff Loeliger (r12110)                                *
 * $Log: typedefs.h,v $
 * Revision 1.1  2010/07/21 13:28:05  dmihail1
 * Add to DataBase: 'runtime' folder
 * See: MTWX40737, MTWX40738, MTWX40743, MTWX42016
 *
 * Revision 1.7  2007/05/02 22:46:00  dfreeland
 * Use latest web version
 *
 * Revision 1.4  2006/03/27 09:59:34  r47354
 * change __GHS__ to __ghs__. As per bug 13213
 *
 * Revision 1.3  2005/02/22 13:09:38  r47354
 * Fix copyright date.
 *
 * Revision 1.2  2004/11/17 12:43:12  r12110
 * -Removed #ifdef DEBUG from around initial comment block.
 *
 * Revision 1.1  2004/11/17 12:38:48  r12110
 * -Initial version checked into CVS.
 * -Updated copyright from Motorola to Freescale.
 *
 *........................................................................*
 * 0.1   J. Loeliger  17/Feb/03    Initial version of file.               *
 * 0.2   J. Loeliger  06/Mar/03    Added DCC support.                     *
 * 0.3   J. Loeliger  07/May/03    Change to fully use ISO data types.    *
 * 0.4   J. Loeliger  17/Jun/03    Change name to motint.h and merge      *
 *                                  MPC5500 and MAC7100 files.            *
 * 0.5   J. Loeliger  04/Nov/03    Changed name to typedefs.h.            *
 * 0.6   J. Loeliger  09/May/04    Changed to support GHS and GCC.        *
 **************************************************************************/

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include<QList>
#include"GL8_structures.h"
#include<vector>
#include<math.h>

using namespace std;

#ifdef __MWERKS__    //Metrowerk CodeWarrior
    #include <stdint.h>

    // Standard typedefs used by header files, based on ISO C standard
    typedef volatile int8_t vint8_t;
    typedef volatile uint8_t vuint8_t;

    typedef volatile int16_t vint16_t;
    typedef volatile uint16_t vuint16_t;

    typedef volatile int32_t vint32_t;
    typedef volatile uint32_t vuint32_t;

#else
#ifdef __ghs__    //GreenHills
    #include <stdint.h>

    // Standard typedefs used by header files, based on ISO C standard
    typedef volatile int8_t vint8_t;
    typedef volatile uint8_t vuint8_t;

    typedef volatile int16_t vint16_t;
    typedef volatile uint16_t vuint16_t;

    typedef volatile int32_t vint32_t;
    typedef volatile uint32_t vuint32_t;

#else

    // This is needed for compilers that don't have a stdint.h file

    typedef signed char int8_t;
    typedef unsigned char uint8_t;
    typedef volatile signed char vint8_t;
    typedef volatile unsigned char vuint8_t;

    typedef signed short int16_t;
    typedef unsigned short uint16_t;
    typedef volatile signed short vint16_t;
    typedef volatile unsigned short vuint16_t;

    typedef signed int int32_t;
    typedef unsigned int uint32_t;
    typedef volatile signed int vint32_t;
    typedef volatile unsigned int vuint32_t;

    typedef signed char         INT8, *PINT8;
    typedef signed short        INT16, *PINT16;
    typedef signed int          INT32, *PINT32;
    typedef unsigned char       UINT8, *PUINT8;
    typedef unsigned short      UINT16, *PUINT16;
//    typedef unsigned int        UINT32, *PUINT32;
//    typedef int                 INT;
//    typedef unsigned int        UINT;
    typedef unsigned char       byte;

#define UINT unsigned int

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
    INT16 PitchGNSS;
    UINT8 _solution_status;
    UINT8 _pos_and_vel_type;
    UINT16 _gps_week;
    INT16 latencyMsHead;		//
    INT16 latencyMsPos;	//
    INT16 latencyMsVel;	//
    UINT16 P_bar;			//
    int H_bar;				//
    byte NewGPS;			//
};

struct POINT3D
{
    double x;
    double y;
    double z;

    POINT3D()
    {
        x=y=z=0;
    }

    POINT3D(double X,double Y,double Z)
    {
        x=X;
        y=Y;
        z=Z;
    }
};

struct INS_OPVT2AHR_DATA
{
    float x;
    float y;
    float z;

    POINT3D originPos;

    float heading;		//������
    float pitch;			//������
    float roll;			//������/������

    float headingSTD;		//������
    float pitchSTD;			//������
    float rollSTD;			//������/������

    float gyroX;			//���ݽ��ٶ�X
    float gyroY;			//���ݽ��ٶ�Y
    float gyroZ;			//���ݽ��ٶ�Z

    float accX;			//���ٶ�X
    float accY;			//���ٶ�Y
    float accZ;			//���ٶ�Z

    float magX;			//�ų�X
    float magY;			//�ų�Y
    float magZ;			//�ų�Z

    uint16_t usw;			//uint״̬

    float vInp;			//������ѹ
    float temper;			//�¶�

    double latitude;			//γ��
    double longitude;			//����
    double altitude;			//����

    double latitudeSTD;			//???
    double longitudeSTD;			//????
    double altitudeSTD;			//????

    float eastSpeed;			//�����ٶ�
    float northSpeed;		//�����ٶ�
    float verticalSpeed;		//��ֱ�ٶ�

    float eastSpeedSTD;			//�����ٶ�
    float northSpeedSTD;		//�����ٶ�
    float verticalSpeedSTD;		//��ֱ�ٶ�

    double latitudeGNSS;		//
    double longitudeGNSS;		//
    double altitudeGNSS;		//

    float horizontalSpeed;	// lon speed
    float trackOverGround;  //
    float verticalSpeed2;	//

    uint32_t msGps;			//����

    uint8_t GNSSInfo1;		//
    uint8_t GNSSInfo2;		//

    uint8_t solnSVs;			//������
    float vLatency;		//�ٶ�ʱ����ǩ�ӳ�ʱ��
    uint8_t anglesPositionType;//

    QString RTK_status;
    QString align_status;


    float headingGNSS;	//
    int16_t latencyMsHead;		//
    int16_t latencyMsPos;	//
    int16_t latencyMsVel;	//
    float pBar;			//
    float hBar;				//
    uint8_t newGPS;			//
    uint8_t imu_work_status;

    INS_OPVT2AHR_DATA()
    {
        x=0;
        y=0;
        z=0;
        heading=0;		//������
        pitch=0;			//������
        roll=0;			//������/������
        headingSTD=0;		//������
        pitchSTD=0;			//������
        rollSTD=0;			//������/������
        gyroX=0;			//���ݽ��ٶ�X
        gyroY=0;			//���ݽ��ٶ�Y
        gyroZ=0;			//���ݽ��ٶ�Z
        accX=0;			//���ٶ�X
        accY=0;			//���ٶ�Y
        accZ=0;			//���ٶ�Z

        magX=0;			//�ų�X
        magY=0;			//�ų�Y
        magZ=0;			//�ų�Z

        usw=0;			//uint״̬

        vInp=0;			//������ѹ
        temper=0;			//�¶�

        latitude=0;			//γ��
        longitude=0;			//����
        altitude=0;			//����

        latitudeSTD=0;			//???
        longitudeSTD=0;			//????
        altitudeSTD=0;			//????

        eastSpeed=0;			//�����ٶ�
        northSpeed=0;		//�����ٶ�
        verticalSpeed=0;		//��ֱ�ٶ�

        eastSpeedSTD=0;			//�����ٶ�
        northSpeedSTD=0;		//�����ٶ�
        verticalSpeedSTD=0;		//��ֱ�ٶ�

        latitudeGNSS=0;		//
        longitudeGNSS=0;		//
        altitudeGNSS=0;		//

        horizontalSpeed=0;	// lon speed
        trackOverGround=0;  //
        verticalSpeed2=0;	//

        msGps=0;			//����

        GNSSInfo1=0;		//
        GNSSInfo2=0;		//

        solnSVs=0;			//������
        vLatency=0;		//�ٶ�ʱ����ǩ�ӳ�ʱ��
        anglesPositionType=0;//

        headingGNSS=0;	//
        latencyMsHead=0;		//
        latencyMsPos=0;	//
        latencyMsVel=0;	//
        pBar=0;			//
        hBar=0;				//
        newGPS=0;			//
        RTK_status="NONE";
        align_status="NONE";
        imu_work_status=0;
    }
};

struct POINT2D
{
    double x;
    double y;

    POINT2D()
    {
        x=y=0;
    }

    POINT2D(double X,double Y)
    {
        x=X;
        y=Y;
    }

};



struct DISPLAY_DATA
{
    QList<POINT2D> trajectory;


    DISPLAY_DATA()
    {
        trajectory.clear();
    }
};

struct GPRMC_DATA
{
    uint32_t utcTime;
    double latitude;
    double longitude;
    double speed;
    double x;
    double y;
};

enum TRACK_POINT_TYPE
{
    none=0,
    stop=1,
    speedLevel1=2,
    speedLevel2=3,
    speedLevel3=4,
    speedLevel4=5,
    speedLevel5=6,
    speedLevel6=7,
    speedLevel7=8,
    speedLevel8=9,
    speedLevel9=10,
    speedLevel10=11,
    stoplineLeftTurn=90,
    stoplineStraight=91,
    stoplineRightTurn=92,
    stoplineUTurnLeft=93,
    stoplineUTurnRight=94,
    station=99

};

struct MAP_POINT
{
    TRACK_POINT_TYPE type;
    POINT3D point;
    uint8_t anglesPositionType;
    int roadIndex;
    double dis2Src;
    double curvature;
    double heading;
    double roll;
    double pitch;
};

struct TRACK_POINT
{
    POINT2D point;
    double aimSpeed;
};

struct fusion_obj
{
   int id;
   double lon;//m
   double lat;//m
   double v_lon;//m/s
   double v_lat;//m.s
   double l;//m
   double w;//m
   double h;//m
   double obo;//deg
   int property;
};


#endif
#endif
#endif

/*********************************************************************
 *
 * Copyright:
 *	Freescale Semiconductor, INC. All Rights Reserved.
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Freescale
 *  Semiconductor, Inc. This software is provided on an "AS IS"
 *  basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, Freescale
 *  Semiconductor DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED,
 *  INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
 *  PARTICULAR PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH
 *  REGARD TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF)
 *  AND ANY ACCOMPANYING WRITTEN MATERIALS.
 *
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL Freescale Semiconductor BE LIABLE FOR ANY DAMAGES WHATSOEVER
 *  (INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 *  BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER
 *  PECUNIARY LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.
 *
 *  Freescale Semiconductor assumes no responsibility for the
 *  maintenance and support of this software
 *
 ********************************************************************/


