#include "header/protocol/pro_vehicle.h"

void IVEH_TLI65::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[m_run_core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);
}

void IVEH_TLI65::parse(void *frm)
{
    can_frm_timed_t * can_frm = (can_frm_timed_t*)frm;

    throtParse(can_frm);
    gearParse(can_frm);
    epsParse(can_frm);
    ebsParse(can_frm);
    retarderParse(can_frm);
    bodyelecParse(can_frm);
    handshakeParse(can_frm);
    diagnose(can_frm);

}

void IVEH_TLI65::package()
{
    static uint8_t mainCounter=1;
    if(mainCounter>=1000)
        mainCounter=1;
    else
        mainCounter++;


    //
    for(int i=0;i<12;i++)
    {
        if(mainCounter%sendNum[i]==0)
        {
            if(sendCounter[i]>=15)
                sendCounter[i]=1;
            else
                sendCounter[i]++;


            uint8_t s_data[8] = {0,0,0,0,0,0,0,0};
            can_frm_t can_send_buff(0,0,1,s_data,8,0);

            switch (i)
            {
                case 0:
                {

                    handshakePackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 1:
                {

                    throtPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 2:
                {

                    ebsPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 3:
                {

                    epbPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 4:
                {

                    retarderPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 5:
                {
                    epsPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 6:
                {
                    gearPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 7:
                {
                    liftPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 8:
                {
                    positionPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 9:
                {
                    elevationPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 10:
                {
                    orientationPackage(&can_send_buff,sendCounter[i]);
                    break;
                }
                case 11:
                {
                    bodyelecPackage(&can_send_buff,sendCounter[i]);
                    break;
                }

            }

            if(i!=11)
                dri->can[CAN_SEND_CHAN_VEH]->send(&can_send_buff);
            else if(i==11&&controlled())
                dri->can[CAN_SEND_CHAN_VEH]->send(&can_send_buff);

        }
    }


}

VCU1 IVEH_TLI65::getVcu1()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu1;
}

VCU2 IVEH_TLI65::getVcu2()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu2;
}

VCU3 IVEH_TLI65::getVcu3()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu3;
}

VCU4 IVEH_TLI65::getVcu4()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu4;
}

VCU5 IVEH_TLI65::getVcu5()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu5;
}

VCU6 IVEH_TLI65::getVcu6()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu6;
}

VCU7 IVEH_TLI65::getVcu7()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu7;
}

VCU8 IVEH_TLI65::getVcu8()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu8;
}

VCU9 IVEH_TLI65::getVcu9()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu9;
}

VCU10 IVEH_TLI65::getVcu10()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu10;
}

VCU11 IVEH_TLI65::getVcu11()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu11;
}

VCU12 IVEH_TLI65::getVcu12()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu12;
}

VCU13 IVEH_TLI65::getVcu13()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu13;
}

VCU14 IVEH_TLI65::getVcu14()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu14;
}

VCU15 IVEH_TLI65::getVcu15()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return vcu15;
}

void IVEH_TLI65::setAdcu1(ADCU_1 _adcu1)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu1=_adcu1;
}

void IVEH_TLI65::setAdcu2(ADCU_2 _adcu2)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu2=_adcu2;
}

void IVEH_TLI65::setAdcu3(ADCU_3 _adcu3)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu3=_adcu3;
}

void IVEH_TLI65::setAdcu4(ADCU_4 _adcu4)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu4=_adcu4;
}

void IVEH_TLI65::setAdcu5(ADCU_5 _adcu5)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu5=_adcu5;
}

void IVEH_TLI65::setAdcu6(ADCU_6 _adcu6)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu6=_adcu6;
}

void IVEH_TLI65::setAdcu7(ADCU_7 _adcu7)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu7=_adcu7;
}

void IVEH_TLI65::setAdcu8(ADCU_8 _adcu8)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu8=_adcu8;
}

void IVEH_TLI65::setAdcu9(ADCU_9 _adcu9)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu9=_adcu9;
}

void IVEH_TLI65::setAdcu10(ADCU_10 _adcu10)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu10=_adcu10;
}

void IVEH_TLI65::setAdcu11(ADCU_11 _adcu11)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu11=_adcu11;
}

void IVEH_TLI65::setAdcu12(ADCU_12 _adcu12)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    adcu12=_adcu12;
}

float IVEH_TLI65::getVspd()
{
    return getVcu6().VCU_VhclSpd/256.0/3.6;
}

bool IVEH_TLI65::controlled()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_to_control;
}

void IVEH_TLI65::setVcu1(VCU1 _vcu1)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu1=_vcu1;
}

void IVEH_TLI65::setVcu2(VCU2 _vcu2)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu2=_vcu2;
}

void IVEH_TLI65::setVcu3(VCU3 _vcu3)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu3=_vcu3;
}

void IVEH_TLI65::setVcu4(VCU4 _vcu4)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu4=_vcu4;
}

void IVEH_TLI65::setVcu5(VCU5 _vcu5)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu5=_vcu5;
}

void IVEH_TLI65::setVcu6(VCU6 _vcu6)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu6=_vcu6;
}

void IVEH_TLI65::setVcu7(VCU7 _vcu7)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu7=_vcu7;
}

void IVEH_TLI65::setVcu8(VCU8 _vcu8)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu8=_vcu8;
}

void IVEH_TLI65::setVcu9(VCU9 _vcu9)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu9=_vcu9;
}

void IVEH_TLI65::setVcu10(VCU10 _vcu10)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu10=_vcu10;
}

void IVEH_TLI65::setVcu11(VCU11 _vcu11)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu11=_vcu11;
}

void IVEH_TLI65::setVcu12(VCU12 _vcu12)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu12=_vcu12;
}

void IVEH_TLI65::setVcu13(VCU13 _vcu13)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu13=_vcu13;
}

void IVEH_TLI65::setVcu14(VCU14 _vcu14)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu14=_vcu14;
}

void IVEH_TLI65::setVcu15(VCU15 _vcu15)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    vcu15=_vcu15;
}

ADCU_1 IVEH_TLI65::getAdcu1()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu1;
}

ADCU_2 IVEH_TLI65::getAdcu2()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu2;
}

ADCU_3 IVEH_TLI65::getAdcu3()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu3;
}

ADCU_4 IVEH_TLI65::getAdcu4()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu4;
}

ADCU_5 IVEH_TLI65::getAdcu5()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu5;
}

ADCU_6 IVEH_TLI65::getAdcu6()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu6;
}

ADCU_7 IVEH_TLI65::getAdcu7()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu7;
}

ADCU_8 IVEH_TLI65::getAdcu8()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu8;
}

ADCU_9 IVEH_TLI65::getAdcu9()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu9;
}

ADCU_10 IVEH_TLI65::getAdcu10()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu10;
}

ADCU_11 IVEH_TLI65::getAdcu11()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu11;
}

ADCU_12 IVEH_TLI65::getAdcu12()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return adcu12;
}

void IVEH_TLI65::throtParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C300021)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU2 _vcu2=getVcu2();
            _vcu2.VCU_AccPedalPosition=_can_obj->can_frame.data[0];
            _vcu2.VCU_EngineTrqPer=_can_obj->can_frame.data[1];
            _vcu2.VCU_EngineSpd=_can_obj->can_frame.data[2]+_can_obj->can_frame.data[3]*256;
            _vcu2.VCU_AccPedalDrvrOprtnPstn=_can_obj->can_frame.data[4];
            _vcu2.VCU_2_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu2.VCU_2_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;
            setVcu2(_vcu2);
        }
    }
}

void IVEH_TLI65::gearParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C303021)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU6 _vcu6=getVcu6();
            _vcu6.VCU_CrrntTranGearPstn=_can_obj->can_frame.data[0];
            _vcu6.VCU_CrrntTranTopGearPstn=_can_obj->can_frame.data[1];
            _vcu6.VCU_VhclSpd=_can_obj->can_frame.data[2]+_can_obj->can_frame.data[3]*256;
            _vcu6.VCU_6_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu6.VCU_6_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;
            setVcu6(_vcu6);
        }
    }
}

void IVEH_TLI65::epsParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C302021)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU5 _vcu5=getVcu5();
            _vcu5.VCU_StrWhlAngleCrrnt=_can_obj->can_frame.data[0]+_can_obj->can_frame.data[1]*256;
            _vcu5.VCU_StrWhlMtrAsstTrq=_can_obj->can_frame.data[2]+_can_obj->can_frame.data[3]*256;
            _vcu5.VCU_StrWhlAngleSpd=_can_obj->can_frame.data[4];
            _vcu5.VCU_StrWhlTakeOverStatus=_can_obj->can_frame.data[5];
            _vcu5.VCU_5_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu5.VCU_5_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;
            setVcu5(_vcu5);
        }
    }
}

void IVEH_TLI65::ebsParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C301121)
    {
        VCU3 _vcu3=getVcu3();
        _vcu3.VCU_FShaftAvrgSpd=_can_obj->can_frame.data[0]+_can_obj->can_frame.data[1]*256;
        _vcu3.VCU_WhlSpdFrLeRaw=_can_obj->can_frame.data[2];
        _vcu3.VCU_WhlSpdFrRiRaw=_can_obj->can_frame.data[3];
        _vcu3.VCU_WhlSpdReLeRaw=_can_obj->can_frame.data[6];
        _vcu3.VCU_WhlSpdReRiRaw=_can_obj->can_frame.data[7];

        setVcu3(_vcu3);
    }
    else if(_can_obj->can_frame.canid==0x0C301021)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU15 _vcu15=getVcu15();
            _vcu15.VCU_BrakePedalPosition=_can_obj->can_frame.data[1];
            _vcu15.VCU_15_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu15.VCU_15_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;

            setVcu15(_vcu15);
        }
    }
}

void IVEH_TLI65::retarderParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C301221)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU4 _vcu4=getVcu4();
            _vcu4.VCU_RetarerTrqPer=_can_obj->can_frame.data[0];
            _vcu4.VCU_4_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu4.VCU_4_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;

            setVcu4(_vcu4);
        }
    }
}

void IVEH_TLI65::bodyelecParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x0C304021)
    {
        if(_can_obj->can_frame.checksum())
        {
            VCU7 _vcu7=getVcu7();
            _vcu7.VCU_VhclLngtdnlAngl=_can_obj->can_frame.data[0]+_can_obj->can_frame.data[1]*256;
            _vcu7.VCU_VhclLtrlAngl=_can_obj->can_frame.data[2]+_can_obj->can_frame.data[3]*256;
            _vcu7.VCU_CntnrLftAngle=_can_obj->can_frame.data[4]+_can_obj->can_frame.data[5]*256;
            _vcu7.VCU_CntnrLftStatus=_can_obj->can_frame.data[6];
            _vcu7.VCU_7_CheckSum=(_can_obj->can_frame.data[7]>>0)&0x0f;
            _vcu7.VCU_7_RollingCounter=(_can_obj->can_frame.data[7]>>4)&0x0f;

            setVcu7(_vcu7);
        }
    }
    else  if(_can_obj->can_frame.canid==0x1830F021)
    {
        VCU8 _vcu8=getVcu8();
        _vcu8.VCU_VIN1=_can_obj->can_frame.data[0];
        _vcu8.VCU_VIN2=_can_obj->can_frame.data[1];
        _vcu8.VCU_VIN3=_can_obj->can_frame.data[2];
        _vcu8.VCU_VIN4=_can_obj->can_frame.data[3];
        _vcu8.VCU_VIN5=_can_obj->can_frame.data[4];
        _vcu8.VCU_VIN6=_can_obj->can_frame.data[5];
        _vcu8.VCU_VIN7=_can_obj->can_frame.data[6];
        _vcu8.VCU_VIN8=_can_obj->can_frame.data[7];

        setVcu8(_vcu8);
    }
    else if(_can_obj->can_frame.canid==0x1830A321)
    {
        VCU9 _vcu9=getVcu9();
        _vcu9.VCU_EmrgcyTwo_FlshSwtch=(_can_obj->can_frame.data[0]>>0)&0x03;
        _vcu9.VCU_AirFltBlckAlarm=(_can_obj->can_frame.data[0]>>2)&0x03;
        _vcu9.VCU_StrOilFltBlckAlarm=(_can_obj->can_frame.data[0]>>4)&0x03;

        setVcu9(_vcu9);
    }
    else if(_can_obj->can_frame.canid==0x1830A021)
    {
        VCU10 _vcu10=getVcu10();
        _vcu10.VCU_LTurnIndicator=(_can_obj->can_frame.data[0]>>0)&0x03;
        _vcu10.VCU_RTurnIndicator=(_can_obj->can_frame.data[0]>>2)&0x03;
        _vcu10.VCU_LowBeamStatus=(_can_obj->can_frame.data[0]>>4)&0x03;
        _vcu10.VCU_HighBeamStatus=(_can_obj->can_frame.data[0]>6)&0x03;

        _vcu10.VCU_RotationLampStatus=(_can_obj->can_frame.data[1]>>0)&0x03;
        _vcu10.VCU_HooterStatus=(_can_obj->can_frame.data[1]>>2)&0x03;
        _vcu10.VCU_FFogLampStatus=(_can_obj->can_frame.data[1]>>4)&0x03;
        _vcu10.VCU_FWorkBeamStatus=(_can_obj->can_frame.data[1]>>6)&0x03;

        _vcu10.VCU_SideBeamStatus=(_can_obj->can_frame.data[2]>>0)&0x03;
        _vcu10.VCU_PositionLampStatus=(_can_obj->can_frame.data[2]>>2)&0x03;

        _vcu10.VCU_WipeMtrPowerSupply=(_can_obj->can_frame.data[3]>>0)&0x03;
        _vcu10.VCU_EBSPowerSupply=(_can_obj->can_frame.data[3]>>2)&0x03;
        _vcu10.VCU_BaroceptorPowerSupply=(_can_obj->can_frame.data[3]>>4)&0x03;

        _vcu10.VCU_Solenoid_ValveUpSwtch=(_can_obj->can_frame.data[4]>>0)&0x03;
        _vcu10.VCU_Solenoid_ValveDwnSwtch=(_can_obj->can_frame.data[4]>>2)&0x03;
        _vcu10.VCU_ManuBrkSolenoid_Valve=(_can_obj->can_frame.data[4]>>4)&0x03;

        setVcu10(_vcu10);
    }
    else if(_can_obj->can_frame.canid==0x1830A221)
    {
        VCU11 _vcu11=getVcu11();
        _vcu11.VCU_Baroceptor1=_can_obj->can_frame.data[0];
        _vcu11.VCU_Baroceptor2=_can_obj->can_frame.data[1];
        _vcu11.VCU_FuelQuantityTransducer=_can_obj->can_frame.data[2];

        setVcu11(_vcu11);
    }
    else if(_can_obj->can_frame.canid==0x1830A121)
    {
        VCU12 _vcu12=getVcu12();
        _vcu12.VCU_LRTurnIndicator=(_can_obj->can_frame.data[0]>>0)&0x03;
        _vcu12.VCU_RRTurnIndicator=(_can_obj->can_frame.data[0]>>2)&0x03;
        _vcu12.VCU_RPositionLampStatus=(_can_obj->can_frame.data[0]>>4)&0x03;
        _vcu12.VCU_BrakeIndicator=(_can_obj->can_frame.data[0]>6)&0x03;

        _vcu12.VCU_ReverseIndicator=(_can_obj->can_frame.data[1]>>0)&0x03;
        _vcu12.VCU_RWorkBeamStatus=(_can_obj->can_frame.data[1]>>2)&0x03;

        _vcu12.VCU_AuxiliaryRadiator1=(_can_obj->can_frame.data[2]>>0)&0x03;
        _vcu12.VCU_AuxiliaryRadiator2=(_can_obj->can_frame.data[2]>>2)&0x03;
        _vcu12.VCU_ShftDiffSolenoid_Valve=(_can_obj->can_frame.data[2]>>4)&0x03;

        _vcu12.VCU_DryerPowerSupply=(_can_obj->can_frame.data[3]>>0)&0x03;
        _vcu12.VCU_DipMeterPowerSupply=(_can_obj->can_frame.data[3]>>2)&0x03;
        _vcu12.VCU_Water_Cold_KeepPowerSupply=(_can_obj->can_frame.data[3]>>4)&0x03;

        _vcu12.VCU_ExhaustBrakeOutput=(_can_obj->can_frame.data[4]>>0)&0x03;

        setVcu12(_vcu12);
    }
}

void IVEH_TLI65::handshakeParse(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x1830FF21)
    {
        VCU1 _vcu1=getVcu1();
        _vcu1.VCU_Handshake1=_can_obj->can_frame.data[0];
        _vcu1.VCU_Handshake2=_can_obj->can_frame.data[1];
        _vcu1.VCU_Handshake3=_can_obj->can_frame.data[2];
        _vcu1.VCU_Handshake4=_can_obj->can_frame.data[3];
        _vcu1.VCU_Handshake5=_can_obj->can_frame.data[4];
        _vcu1.VCU_Handshake6=_can_obj->can_frame.data[5];
        _vcu1.VCU_Handshake7=_can_obj->can_frame.data[6];
        _vcu1.VCU_Handshake8=_can_obj->can_frame.data[7];
        setVcu1(_vcu1);
    }
}

void IVEH_TLI65::diagnose(can_frm_timed_t* _can_obj)
{
    if(_can_obj->can_frame.canid==0x1830A121)
    {
        VCU13 _vcu13=getVcu13();
        _vcu13.VCU_VehCtrlMode=_can_obj->can_frame.data[0];

        _vcu13.VCU_EngineActiveStatus=(_can_obj->can_frame.data[1]>>0)&0x03;
        _vcu13.VCU_HandBrakeActiveStatus=(_can_obj->can_frame.data[1]>>2)&0x03;
        _vcu13.VCU_EBSActiveStatus=(_can_obj->can_frame.data[1]>>4)&0x03;
        _vcu13.VCU_RetarderActiveStatus=(_can_obj->can_frame.data[1]>>6)&0x03;

        _vcu13.VCU_GearBoxActiveStatus=(_can_obj->can_frame.data[2]>>0)&0x03;
        _vcu13.VCU_EPSActiveStatus=(_can_obj->can_frame.data[2]>>2)&0x03;
        _vcu13.VCU_LiftActiveStatus=(_can_obj->can_frame.data[2]>>4)&0x03;

        _vcu13.VCU_EStopActiveStatus=_can_obj->can_frame.data[7];


        setVcu13(_vcu13);
    }
    else if(_can_obj->can_frame.canid==0x1830E021)
    {
        VCU14 _vcu14=getVcu14();
        _vcu14.VCU_EngineFault=_can_obj->can_frame.data[0];
        _vcu14.VCU_GearBoxFault=_can_obj->can_frame.data[1];
        _vcu14.VCU_EBSFault=_can_obj->can_frame.data[2];
        _vcu14.VCU_EPSFault=_can_obj->can_frame.data[3];
        _vcu14.VCU_ReserveFaultSig=_can_obj->can_frame.data[4];

        setVcu14(_vcu14);
    }
}

void IVEH_TLI65::handshakePackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_1 _adcu1=getAdcu1();

    _can_frm->canid=CAN_ID_ADCU1;

//                    can_send_buff.data[0]=_adcu1.ADCU_Handshake1;
//                    can_send_buff.data[1]=_adcu1.ADCU_Handshake2;
//                    can_send_buff.data[2]=_adcu1.ADCU_Handshake3;
//                    can_send_buff.data[3]=_adcu1.ADCU_Handshake4;
//                    can_send_buff.data[4]=_adcu1.ADCU_Handshake5;
//                    can_send_buff.data[5]=_adcu1.ADCU_Handshake6;
//                    can_send_buff.data[6]=_adcu1.ADCU_Handshake7;
//                    can_send_buff.data[7]=_adcu1.ADCU_Handshake8;

    _can_frm->data[0]=0x45;
    _can_frm->data[1]=0x51;
    _can_frm->data[2]=0x49;
    _can_frm->data[3]=0x36;
    _can_frm->data[4]=0x35;
    _can_frm->data[5]=0x43;
    _can_frm->data[6]=0x4D;
    _can_frm->data[7]=0x44;
}

void IVEH_TLI65::throtPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_2 _adcu2=getAdcu2();

    _can_frm->canid=CAN_ID_ADCU2;

    _can_frm->data[0]=(uint8_t)(_adcu2.ADCU_AccPedalPosition*2.5);
    _can_frm->data[1]=(uint8_t)(_adcu2.ADCU_EngineStrtStp);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::ebsPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_3 _adcu3=getAdcu3();

    _can_frm->canid=CAN_ID_ADCU3;

    _can_frm->data[0]=(uint8_t)((_adcu3.ADCU_BrkDeceleration*2048)%256);
    _can_frm->data[1]=(uint8_t)((_adcu3.ADCU_BrkDeceleration*2048)/256);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::epbPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_4 _adcu4=getAdcu4();

    _can_frm->canid=CAN_ID_ADCU4;

    _can_frm->data[0]=(uint8_t)(_adcu4.ADCU_EPBStatus);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::retarderPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_5 _adcu5=getAdcu5();

    _can_frm->canid=CAN_ID_ADCU5;

    _can_frm->data[0]=(uint8_t)(_adcu5.ADCU_RetarderTrqPer+125);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::epsPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_6 _adcu6=getAdcu6();

    _can_frm->canid=CAN_ID_ADCU6;

    _can_frm->data[0]=(uint8_t)(((-_adcu6.ADCU_DsrStrAngl+1575)*10)%256);
    _can_frm->data[1]=(uint8_t)(((-_adcu6.ADCU_DsrStrAngl+1575)*10)/256);
    _can_frm->data[4]=(uint8_t)(_adcu6.ADCU_DsrStrAnglSpd*0.1);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::gearPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_7 _adcu7=getAdcu7();

    _can_frm->canid=CAN_ID_ADCU7;

    _can_frm->data[0]=(uint8_t)(_adcu7.ADCU_GearDemand);
    _can_frm->data[1]=(uint8_t)(_adcu7.ADCU_TranTopGearPstnLmt+125);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::liftPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_8 _adcu8=getAdcu8();

    _can_frm->canid=CAN_ID_ADCU8;

    _can_frm->data[0]=(uint8_t)(_adcu8.ADCU_LftReqStatus);
    _can_frm->data[6]=controlled()?1:0;
    _can_frm->data[7] = counter<<4;
    _can_frm->calcChecksum();
}

void IVEH_TLI65::positionPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_9 _adcu9=getAdcu9();

    _can_frm->canid=CAN_ID_ADCU9;

    uint32_t lat_send=(uint32_t)((_adcu9.ADCU_latitude+210)*pow(10,7));
    for(int i=0;i<4;i++)
    {
        _can_frm->data[i]=(uint8_t)(((uint32_t)(lat_send/pow(256,i)))%256);
    }

    uint32_t lon_send=(uint32_t)((_adcu9.ADCU_longitude+210)*pow(10,7));
    for(int i=0;i<4;i++)
    {
        _can_frm->data[i+4]=(uint8_t)(((uint32_t)(lon_send/pow(256,i)))%256);
    }

}

void IVEH_TLI65::elevationPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_10 _adcu10=getAdcu10();

    _can_frm->canid=CAN_ID_ADCU10;

    uint64_t alt_send=(uint64_t)((_adcu10.ADCU_elevation+1500)*8);
    for(int i=0;i<8;i++)
    {
        _can_frm->data[i]=(uint8_t)(((uint64_t)(alt_send/pow(256,i)))%256);
    }

}

void IVEH_TLI65::orientationPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_11 _adcu11=getAdcu11();

    _can_frm->canid=CAN_ID_ADCU11;

    uint16_t pitch_send=(uint16_t)((_adcu11.ADCU_PitchDeg+200)*128);
    for(int i=0;i<2;i++)
    {
        _can_frm->data[i]=(uint8_t)(((uint16_t)(pitch_send/pow(256,i)))%256);
    }

    uint16_t roll_send=(uint16_t)((-_adcu11.ADCU_RollDeg+200)*128);
    for(int i=0;i<2;i++)
    {
        _can_frm->data[i+2]=(uint8_t)(((uint16_t)(roll_send/pow(256,i)))%256);
    }

    uint16_t yaw_send=(uint16_t)((_adcu11.ADCU_YawDeg+200)*128);
    for(int i=0;i<2;i++)
    {
        _can_frm->data[i+4]=(uint8_t)(((uint16_t)(yaw_send/pow(256,i)))%256);
    }
}

void IVEH_TLI65::bodyelecPackage(can_frm_t *_can_frm,uint8_t counter)
{
    ADCU_12 _adcu12=getAdcu12();

    _can_frm->canid=CAN_ID_ADCU12;


}

bool IVEH_TLI65::active()
{
    if(running())
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_to_control = true;
        m_activated = true;
        return true;
    }
    else
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_to_control = false;
        m_activated = false;
        return false;
    }
}

void IVEH_TLI65::deactive()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_to_control = false;
    m_activated = false;
}

bool IVEH_TLI65::activated()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_activated;
}

void IVEH_TLI65::err_check()
{

}

void IVEH_TLI65::task_func()
{

    err_check();

    package();

}
