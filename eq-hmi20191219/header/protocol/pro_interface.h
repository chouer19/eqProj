#ifndef PRO_INTERFACE_H
#define PRO_INTERFACE_H

#include"header/system/pub_types.h"
#include"controlcan.h"

//--------------------------------------------

struct ADCU_1
{
    ADCU_1()
        :ADCU_Handshake1(0x45)
        ,ADCU_Handshake2(0x51)
        ,ADCU_Handshake3(0x49)
        ,ADCU_Handshake4(0x36)
        ,ADCU_Handshake5(0x35)
        ,ADCU_Handshake6(0x43)
        ,ADCU_Handshake7(0x4D)
        ,ADCU_Handshake8(0x44)
    {}

    uint8_T ADCU_Handshake1;
    uint8_T ADCU_Handshake2;
    uint8_T ADCU_Handshake3;
    uint8_T ADCU_Handshake4;
    uint8_T ADCU_Handshake5;
    uint8_T ADCU_Handshake6;
    uint8_T ADCU_Handshake7;
    uint8_T ADCU_Handshake8;
};//1820FF2F 100ms

struct ADCU_2
{
    ADCU_2()
        :ADCU_AccPedalPosition(0)
        ,ADCU_EngineStrtStp(3)
        ,ADCU_2_ControlActiveStatus(0)
        ,ADCU_2_CheckSum(0)
        ,ADCU_2_RollingCounter(0)

    {}

    uint8_T ADCU_AccPedalPosition;
    uint8_T ADCU_EngineStrtStp;
    uint8_T ADCU_2_ControlActiveStatus;
    uint8_T ADCU_2_CheckSum;
    uint8_T ADCU_2_RollingCounter;
};//0C20002F 10ms

struct ADCU_3
{
    ADCU_3()
        :ADCU_BrkDeceleration(0)
        ,ADCU_3_ControlActiveStatus(0)
        ,ADCU_3_CheckSum(0)
        ,ADCU_3_RollingCounter(0)

    {}

    uint16_T ADCU_BrkDeceleration;
    uint8_T ADCU_3_ControlActiveStatus;
    uint8_T ADCU_3_CheckSum;
    uint8_T ADCU_3_RollingCounter;
};//0C20102F 20ms

struct ADCU_4
{
    ADCU_4()
        :ADCU_EPBStatus(0)
        ,ADCU_4_ControlActiveStatus(0)
        ,ADCU_4_CheckSum(0)
        ,ADCU_4_RollingCounter(0)

    {}

    uint8_T ADCU_EPBStatus;
    uint8_T ADCU_4_ControlActiveStatus;
    uint8_T ADCU_4_CheckSum;
    uint8_T ADCU_4_RollingCounter;
};//0C20112F 100ms

struct ADCU_5
{
    ADCU_5()
        :ADCU_RetarderTrqPer(0)
        ,ADCU_5_ControlActiveStatus(0)
        ,ADCU_5_CheckSum(0)
        ,ADCU_5_RollingCounter(0)

    {}

    uint8_T ADCU_RetarderTrqPer;
    uint8_T ADCU_5_ControlActiveStatus;
    uint8_T ADCU_5_CheckSum;
    uint8_T ADCU_5_RollingCounter;
};//0C20122F 100ms

struct ADCU_6
{
    ADCU_6()
        :ADCU_DsrStrAngl(0)
        ,ADCU_DsrStrAnglSpd(500)
        ,ADCU_6_ControlActiveStatus(0)
        ,ADCU_6_CheckSum(0)
        ,ADCU_6_RollingCounter(0)

    {}

    uint16_T ADCU_DsrStrAngl;
    uint8_T ADCU_DsrStrAnglSpd;
    uint8_T ADCU_6_ControlActiveStatus;
    uint8_T ADCU_6_CheckSum;
    uint8_T ADCU_6_RollingCounter;
};//0C20202F 50ms

struct ADCU_7
{
    ADCU_7()
        :ADCU_GearDemand(0x40)
        ,ADCU_TranTopGearPstnLmt(6)
        ,ADCU_7_ControlActiveStatus(0)
        ,ADCU_7_CheckSum(0)
        ,ADCU_7_RollingCounter(0)

    {}

    uint8_T ADCU_GearDemand;
    uint8_T ADCU_TranTopGearPstnLmt;
    uint8_T ADCU_7_ControlActiveStatus;
    uint8_T ADCU_7_CheckSum;
    uint8_T ADCU_7_RollingCounter;
};//0C20302F 50ms

struct ADCU_8
{
    ADCU_8()
        :ADCU_LftReqStatus(0x11)
        ,ADCU_8_ControlActiveStatus(0)
        ,ADCU_8_CheckSum(0)
        ,ADCU_8_RollingCounter(0)

    {}

    uint8_T ADCU_LftReqStatus;
    uint8_T ADCU_8_ControlActiveStatus;
    uint8_T ADCU_8_CheckSum;
    uint8_T ADCU_8_RollingCounter;
};//0C20402F 100ms

struct ADCU_9
{
    ADCU_9()
        :ADCU_latitude(0)
        ,ADCU_longitude(0)

    {}

    uint32_T ADCU_latitude;
    uint32_T ADCU_longitude;

};//1820F02F 5000ms

struct ADCU_10
{
    ADCU_10()
        :ADCU_elevation(0)

    {}

    uint16_T ADCU_elevation;

};//1820F12F 5000ms

struct ADCU_11
{
    ADCU_11()
        :ADCU_PitchDeg(0)
        ,ADCU_RollDeg(0)
        ,ADCU_YawDeg(0)

    {}

    uint16_T ADCU_PitchDeg;
    uint16_T ADCU_RollDeg;
    uint16_T ADCU_YawDeg;

};//1820F22F 100ms

struct ADCU_12
{
    ADCU_12()
        :ADCU_LTurnIndicatorDemand(0)
        ,ADCU_RTurnIndicatorDemand(0)
        ,ADCU_LowBeamStatusDemand(0)
        ,ADCU_HighBeamStatusDemand(0)
        ,ADCU_RotationLampDemand(0)
        ,ADCU_HooterDemand(0)
        ,ADCU_FFogLampDemand(0)
        ,ADCU_FWorkBeamDemand(0)
        ,ADCU_SideBeamDemand(0)
        ,ADCU_PositionLampDemand(0)
        ,ADCU_EmrgcyTwoFlshSwtchDemand(0)
        ,ADCU_BrakeIndicatorDemand(0)
        ,ADCU_ReverseIndicatorDemand(0)
        ,ADCU_RWorkBeamDemand(0)
        ,ADCU_AuxiliaryRadiatorDemand(0)
        ,ADCU_ShftDiffSolenoidValveDemand(0)
        ,ADCU_ExhaustBrakeDemand(0)
    {}

    uint8_T ADCU_LTurnIndicatorDemand;
    uint8_T ADCU_RTurnIndicatorDemand;
    uint8_T ADCU_LowBeamStatusDemand;
    uint8_T ADCU_HighBeamStatusDemand;
    uint8_T ADCU_RotationLampDemand;
    uint8_T ADCU_HooterDemand;
    uint8_T ADCU_FFogLampDemand;
    uint8_T ADCU_FWorkBeamDemand;
    uint8_T ADCU_SideBeamDemand;
    uint8_T ADCU_PositionLampDemand;
    uint8_T ADCU_EmrgcyTwoFlshSwtchDemand;
    uint8_T ADCU_BrakeIndicatorDemand;
    uint8_T ADCU_ReverseIndicatorDemand;
    uint8_T ADCU_RWorkBeamDemand;
    uint8_T ADCU_AuxiliaryRadiatorDemand;
    uint8_T ADCU_ShftDiffSolenoidValveDemand;
    uint8_T ADCU_ExhaustBrakeDemand;

};//1820A02F 100ms

struct VCU1
{
    VCU1()
        :VCU_Handshake1(0)
        ,VCU_Handshake2(0)
        ,VCU_Handshake3(0)
        ,VCU_Handshake4(0)
        ,VCU_Handshake5(0)
        ,VCU_Handshake6(0)
        ,VCU_Handshake7(0)
        ,VCU_Handshake8(0)
    {}

    uint8_T VCU_Handshake1;
    uint8_T VCU_Handshake2;
    uint8_T VCU_Handshake3;
    uint8_T VCU_Handshake4;
    uint8_T VCU_Handshake5;
    uint8_T VCU_Handshake6;
    uint8_T VCU_Handshake7;
    uint8_T VCU_Handshake8;

};//1830FF21 100ms

struct VCU2
{
    VCU2()
        :VCU_AccPedalPosition(0)
        ,VCU_EngineTrqPer(0)
        ,VCU_EngineSpd(0)
        ,VCU_AccPedalDrvrOprtnPstn(0)
        ,VCU_2_CheckSum(0)
        ,VCU_2_RollingCounter(0)
    {}

    uint8_T VCU_AccPedalPosition;
    uint8_T VCU_EngineTrqPer;
    uint16_T VCU_EngineSpd;
    uint8_T VCU_AccPedalDrvrOprtnPstn;
    uint8_T VCU_2_CheckSum;
    uint8_T VCU_2_RollingCounter;


};//0C300021 10ms

struct VCU3
{
    VCU3()
        :VCU_FShaftAvrgSpd(0)
        ,VCU_WhlSpdFrLeRaw(0)
        ,VCU_WhlSpdFrRiRaw(0)
        ,VCU_WhlSpdReLeRaw(0)
        ,VCU_WhlSpdReRiRaw(0)
    {}

    uint16_T VCU_FShaftAvrgSpd;
    uint8_T VCU_WhlSpdFrLeRaw;
    uint8_T VCU_WhlSpdFrRiRaw;
    uint8_T VCU_WhlSpdReLeRaw;
    uint8_T VCU_WhlSpdReRiRaw;

};//0C301121 100ms

struct VCU4
{
    VCU4()
        :VCU_RetarerTrqPer(0)
        ,VCU_4_CheckSum(0)
        ,VCU_4_RollingCounter(0)

    {}

    uint8_T VCU_RetarerTrqPer;
    uint8_T VCU_4_CheckSum;
    uint8_T VCU_4_RollingCounter;

};//0C301221 100ms

struct VCU5
{
    VCU5()
        :VCU_StrWhlAngleCrrnt(0)
        ,VCU_StrWhlMtrAsstTrq(0)
        ,VCU_StrWhlAngleSpd(0)
        ,VCU_StrWhlTakeOverStatus(0)
        ,VCU_5_CheckSum(0)
        ,VCU_5_RollingCounter(0)
    {}

    uint16_T VCU_StrWhlAngleCrrnt;
    uint16_T VCU_StrWhlMtrAsstTrq;
    uint8_T VCU_StrWhlAngleSpd;
    uint8_T VCU_StrWhlTakeOverStatus;
    uint8_T VCU_5_CheckSum;
    uint8_T VCU_5_RollingCounter;

};//0C302021 50ms

struct VCU6
{
    VCU6()
        :VCU_CrrntTranGearPstn(0)
        ,VCU_CrrntTranTopGearPstn(0)
        ,VCU_VhclSpd(0)
        ,VCU_6_CheckSum(0)
        ,VCU_6_RollingCounter(0)
    {}

    uint8_T VCU_CrrntTranGearPstn;
    uint8_T VCU_CrrntTranTopGearPstn;
    uint16_T VCU_VhclSpd;
    uint8_T VCU_6_CheckSum;
    uint8_T VCU_6_RollingCounter;

};//0C303021 100ms

struct VCU7
{
    VCU7()
        :VCU_VhclLngtdnlAngl(0)
        ,VCU_VhclLtrlAngl(0)
        ,VCU_CntnrLftAngle(0)
        ,VCU_CntnrLftStatus(0)
        ,VCU_7_CheckSum(0)
        ,VCU_7_RollingCounter(0)
    {}

    uint16_T VCU_VhclLngtdnlAngl;
    uint16_T VCU_VhclLtrlAngl;
    uint16_T VCU_CntnrLftAngle;
    uint8_T VCU_CntnrLftStatus;
    uint8_T VCU_7_CheckSum;
    uint8_T VCU_7_RollingCounter;

};//0C304021 100ms

struct VCU8
{
    VCU8()
        :VCU_VIN1(0)
        ,VCU_VIN2(0)
        ,VCU_VIN3(0)
        ,VCU_VIN4(0)
        ,VCU_VIN5(0)
        ,VCU_VIN6(0)
        ,VCU_VIN7(0)
        ,VCU_VIN8(0)
    {}

    uint8_T VCU_VIN1;
    uint8_T VCU_VIN2;
    uint8_T VCU_VIN3;
    uint8_T VCU_VIN4;
    uint8_T VCU_VIN5;
    uint8_T VCU_VIN6;
    uint8_T VCU_VIN7;
    uint8_T VCU_VIN8;

};//1830F021 100ms

struct VCU9
{
    VCU9()
        :VCU_EmrgcyTwo_FlshSwtch(0)
        ,VCU_AirFltBlckAlarm(0)
        ,VCU_StrOilFltBlckAlarm(0)
    {}

    uint8_T VCU_EmrgcyTwo_FlshSwtch;
    uint8_T VCU_AirFltBlckAlarm;
    uint8_T VCU_StrOilFltBlckAlarm;

};//1830A321 100ms

struct VCU10
{
    VCU10()
        :VCU_LTurnIndicator(0)
        ,VCU_RTurnIndicator(0)
        ,VCU_LowBeamStatus(0)
        ,VCU_HighBeamStatus(0)
        ,VCU_RotationLampStatus(0)
        ,VCU_HooterStatus(0)
        ,VCU_FFogLampStatus(0)
        ,VCU_FWorkBeamStatus(0)
        ,VCU_SideBeamStatus(0)
        ,VCU_PositionLampStatus(0)
        ,VCU_WipeMtrPowerSupply(0)
        ,VCU_EBSPowerSupply(0)
        ,VCU_BaroceptorPowerSupply(0)
        ,VCU_Solenoid_ValveUpSwtch(0)
        ,VCU_Solenoid_ValveDwnSwtch(0)
        ,VCU_ManuBrkSolenoid_Valve(0)

    {}

    uint8_T VCU_LTurnIndicator;
    uint8_T VCU_RTurnIndicator;
    uint8_T VCU_LowBeamStatus;
    uint8_T VCU_HighBeamStatus;

    uint8_T VCU_RotationLampStatus;
    uint8_T VCU_HooterStatus;
    uint8_T VCU_FFogLampStatus;
    uint8_T VCU_FWorkBeamStatus;

    uint8_T VCU_SideBeamStatus;
    uint8_T VCU_PositionLampStatus;

    uint8_T VCU_WipeMtrPowerSupply;
    uint8_T VCU_EBSPowerSupply;
    uint8_T VCU_BaroceptorPowerSupply;

    uint8_T VCU_Solenoid_ValveUpSwtch;
    uint8_T VCU_Solenoid_ValveDwnSwtch;
    uint8_T VCU_ManuBrkSolenoid_Valve;


};//1830A021 100ms

struct VCU11
{
    VCU11()
        :VCU_Baroceptor1(0)
        ,VCU_Baroceptor2(0)
        ,VCU_FuelQuantityTransducer(0)
    {}

    uint8_T VCU_Baroceptor1;
    uint8_T VCU_Baroceptor2;
    uint8_T VCU_FuelQuantityTransducer;

};//1830A221 100ms

struct VCU12
{
    VCU12()
        :VCU_LRTurnIndicator(0)
        ,VCU_RRTurnIndicator(0)
        ,VCU_RPositionLampStatus(0)
        ,VCU_BrakeIndicator(0)
        ,VCU_ReverseIndicator(0)
        ,VCU_RWorkBeamStatus(0)
        ,VCU_AuxiliaryRadiator1(0)
        ,VCU_AuxiliaryRadiator2(0)
        ,VCU_ShftDiffSolenoid_Valve(0)
        ,VCU_DryerPowerSupply(0)
        ,VCU_DipMeterPowerSupply(0)
        ,VCU_Water_Cold_KeepPowerSupply(0)
        ,VCU_ExhaustBrakeOutput(0)

    {}

    uint8_T VCU_LRTurnIndicator;
    uint8_T VCU_RRTurnIndicator;
    uint8_T VCU_RPositionLampStatus;
    uint8_T VCU_BrakeIndicator;

    uint8_T VCU_ReverseIndicator;
    uint8_T VCU_RWorkBeamStatus;

    uint8_T VCU_AuxiliaryRadiator1;
    uint8_T VCU_AuxiliaryRadiator2;
    uint8_T VCU_ShftDiffSolenoid_Valve;

    uint8_T VCU_DryerPowerSupply;
    uint8_T VCU_DipMeterPowerSupply;
    uint8_T VCU_Water_Cold_KeepPowerSupply;

    uint8_T VCU_ExhaustBrakeOutput;

};//1830A121 100ms

struct VCU13
{
    VCU13()
        :VCU_VehCtrlMode(0)
        ,VCU_EngineActiveStatus(0)
        ,VCU_HandBrakeActiveStatus(0)
        ,VCU_EBSActiveStatus(0)
        ,VCU_RetarderActiveStatus(0)
        ,VCU_GearBoxActiveStatus(0)
        ,VCU_EPSActiveStatus(0)
        ,VCU_LiftActiveStatus(0)
        ,VCU_EStopActiveStatus(0)

    {}

    uint8_T VCU_VehCtrlMode;
    uint8_T VCU_EngineActiveStatus;
    uint8_T VCU_HandBrakeActiveStatus;
    uint8_T VCU_EBSActiveStatus;
    uint8_T VCU_RetarderActiveStatus;
    uint8_T VCU_GearBoxActiveStatus;
    uint8_T VCU_EPSActiveStatus;
    uint8_T VCU_LiftActiveStatus;
    uint8_T VCU_EStopActiveStatus;

};//1830B021 100ms

struct VCU14
{
    VCU14()
        :VCU_EngineFault(0)
        ,VCU_GearBoxFault(0)
        ,VCU_EBSFault(0)
        ,VCU_EPSFault(0)
        ,VCU_ReserveFaultSig(0)

    {}

    uint8_T VCU_EngineFault;
    uint8_T VCU_GearBoxFault;
    uint8_T VCU_EBSFault;
    uint8_T VCU_EPSFault;
    uint8_T VCU_ReserveFaultSig;


};//1830E021 1000ms

struct VCU15
{
    VCU15()
        :VCU_BrakePedalPosition(0)
        ,VCU_15_CheckSum(0)
        ,VCU_15_RollingCounter(0)

    {}

    uint8_T VCU_BrakePedalPosition;
    uint8_T VCU_15_CheckSum;
    uint8_T VCU_15_RollingCounter;

};//0C301021  100ms

/* vehicle */
DeclareBasedInterface(IVEH, TASK)
    IVEH()
    :TASK(pub_time, 50)
    {}
    IVEH(CCTIME *_time, uint8_t _period)
    :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual bool active()=0;
    virtual void deactive()=0;
    virtual bool activated()=0;
    virtual void parse(void *frm) = 0;

    virtual VCU1 getVcu1()=0;
    virtual VCU2 getVcu2()=0;
    virtual VCU3 getVcu3()=0;
    virtual VCU4 getVcu4()=0;
    virtual VCU5 getVcu5()=0;
    virtual VCU6 getVcu6()=0;
    virtual VCU7 getVcu7()=0;
    virtual VCU8 getVcu8()=0;
    virtual VCU9 getVcu9()=0;
    virtual VCU10 getVcu10()=0;
    virtual VCU11 getVcu11()=0;
    virtual VCU12 getVcu12()=0;
    virtual VCU13 getVcu13()=0;
    virtual VCU14 getVcu14()=0;
    virtual VCU15 getVcu15()=0;

    virtual void setAdcu1(ADCU_1 _adcu1)=0;
    virtual void setAdcu2(ADCU_2 _adcu2)=0;
    virtual void setAdcu3(ADCU_3 _adcu3)=0;
    virtual void setAdcu4(ADCU_4 _adcu4)=0;
    virtual void setAdcu5(ADCU_5 _adcu5)=0;
    virtual void setAdcu6(ADCU_6 _adcu6)=0;
    virtual void setAdcu7(ADCU_7 _adcu7)=0;
    virtual void setAdcu8(ADCU_8 _adcu8)=0;
    virtual void setAdcu9(ADCU_9 _adcu9)=0;
    virtual void setAdcu10(ADCU_10 _adcu10)=0;
    virtual void setAdcu11(ADCU_11 _adcu11)=0;
    virtual void setAdcu12(ADCU_12 _adcu12)=0;

    virtual float getVspd()=0;

EndInterface


/* DGPS */
DeclareBasedInterface(IDGPS, TASK)
    IDGPS()
    :TASK(pub_time, 50)
    {}
    IDGPS(CCTIME *_time, uint8_t _period)
    :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual void parse(void *frm) = 0;

    virtual point_3d_t position() = 0;

    virtual float heading() = 0;
    virtual float pitch() = 0;
    virtual float pitchAver()=0;
    virtual float roll() = 0;

    virtual float headingSTD()=0;
    virtual float pitchSTD()=0;
    virtual float rollSTD()=0;

    virtual double latitude()=0;
    virtual double longitude()=0;
    virtual double altitude()=0;

    virtual double latitudeSTD()=0;
    virtual double longitudeSTD()=0;
    virtual double altitudeSTD()=0;

    virtual float vsdNorth()=0;
    virtual float vsdEast()=0;
    virtual float vsdUp()=0;

    virtual float vsdNorthSTD()=0;
    virtual float vsdEastSTD()=0;
    virtual float vsdUpSTD()=0;

    virtual string RTKStatus()=0;
    virtual string AlignStatus()=0;
    virtual uint8_t anglesPositionType()=0;

    virtual float yaw_rate() = 0;
    virtual float roll_rate()=0;
    virtual float pitch_rate()=0;

    virtual float lon_accel() = 0;
    virtual float lat_accel() = 0;
    virtual float alt_accel() = 0;

    virtual nav_t fram_data() = 0;
    virtual vector<nav_t> fram_data_1s()=0;
    virtual uint8_t imuWorkStatus()=0;
EndInterface

/* GNSS */
DeclareBasedInterface(IGNSS_RECEIVER, TASK)
IGNSS_RECEIVER()
:TASK(pub_time, 50)
{}
IGNSS_RECEIVER(CCTIME *_time, uint8_t _period)
:TASK(_time, _period)
{}

virtual void install() = 0;
virtual void parse() = 0;
virtual nav_t getGnssOutput()=0;
virtual nav_t getReferencePos()=0;
virtual nav_t getBasePoint()=0;
virtual void setBasePointOffset(point_3d_t _point)=0;
virtual void setRefPointOffset(point_3d_t _point)=0;
virtual void setTerminalInfo(TERMINAL_INFO _info)=0;

EndInterface

/**************************Protocol Interface********************************/
class PROTOCOL
{
public:
    PROTOCOL();
    virtual ~PROTOCOL();

    IVEH *vehicle;
    IDGPS *dgps;
    IGNSS_RECEIVER *gnss_receiver;
};

#endif // PRO_INTERFACE_H
