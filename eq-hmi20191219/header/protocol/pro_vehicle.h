#ifndef PRO_VEHICLE_H
#define PRO_VEHICLE_H

#include"header/system/pub_types.h"
#include"header/drive/dri_interface.hpp"
#include"header/system/sys_interface.hpp"
#include"pro_interface.h"

enum{ TASK_PERIOD_VEH = 10 };

enum{ CAN_SEND_CHAN_VEH = 0 };

enum
{
    SEND_CYCLE_ADCU1 = 100,
    SEND_CYCLE_ADCU2 = 10,
    SEND_CYCLE_ADCU3 = 20,
    SEND_CYCLE_ADCU4 = 100,
    SEND_CYCLE_ADCU5 = 100,
    SEND_CYCLE_ADCU6 = 50,
    SEND_CYCLE_ADCU7 = 50,
    SEND_CYCLE_ADCU8 = 100,
    SEND_CYCLE_ADCU9 = 5000,
    SEND_CYCLE_ADCU10 = 5000,
    SEND_CYCLE_ADCU11 = 100,
    SEND_CYCLE_ADCU12 = 100,
};//ms

enum
{
    CAN_ID_ADCU1 = 0x1820FF2F,
    CAN_ID_ADCU2 = 0x0C20002F,
    CAN_ID_ADCU3 = 0x0C20102F,
    CAN_ID_ADCU4 = 0x0C20112F,
    CAN_ID_ADCU5 = 0x0C20122F,
    CAN_ID_ADCU6 = 0x0C20202F,
    CAN_ID_ADCU7 = 0x0C20302F,
    CAN_ID_ADCU8 = 0x0C20402F,
    CAN_ID_ADCU9 = 0x1820F02F,
    CAN_ID_ADCU10 = 0x1820F12F,
    CAN_ID_ADCU11= 0x1820F22F,
    CAN_ID_ADCU12 = 0x1820A02F,
};




class IVEH_TLI65:implements IVEH
{
public:
    IVEH_TLI65(DRIVE *_dri,SYSTEM *_sys,RUN_CORE _r_core)
        :IVEH(pub_time,TASK_PERIOD_VEH)
        , dri(_dri)
        , sys(_sys)
        , m_run_core(_r_core)
        , m_isAlive(false)
        , m_to_control(false)
        , m_activated(false)
        , m_error_code(0)
    {
        sendNum.push_back(SEND_CYCLE_ADCU1/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU2/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU3/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU4/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU5/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU6/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU7/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU8/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU9/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU10/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU11/TASK_PERIOD_VEH);
        sendNum.push_back(SEND_CYCLE_ADCU12/TASK_PERIOD_VEH);

        for(int i=0;i<12;i++)
           sendCounter.push_back(1);
    }
    virtual ~IVEH_TLI65()
    {
        abort();
        sys->taskList_p[m_run_core]->erase(taskId);
        m_isAlive = false;
        m_thread.join();
    }

    TaskFuncDeclare(IVEH_TLI65, vehicle, (TASK_PERIOD_VEH+10))

    /* task related */
    virtual void install();
    virtual bool active();
    virtual void deactive();
    virtual bool activated();

    virtual void parse(void *frm);

    //
    virtual VCU1 getVcu1();
    virtual VCU2 getVcu2();
    virtual VCU3 getVcu3();
    virtual VCU4 getVcu4();
    virtual VCU5 getVcu5();
    virtual VCU6 getVcu6();
    virtual VCU7 getVcu7();
    virtual VCU8 getVcu8();
    virtual VCU9 getVcu9();
    virtual VCU10 getVcu10();
    virtual VCU11 getVcu11();
    virtual VCU12 getVcu12();
    virtual VCU13 getVcu13();
    virtual VCU14 getVcu14();
    virtual VCU15 getVcu15();

    virtual void setAdcu1(ADCU_1 _adcu1);
    virtual void setAdcu2(ADCU_2 _adcu2);
    virtual void setAdcu3(ADCU_3 _adcu3);
    virtual void setAdcu4(ADCU_4 _adcu4);
    virtual void setAdcu5(ADCU_5 _adcu5);
    virtual void setAdcu6(ADCU_6 _adcu6);
    virtual void setAdcu7(ADCU_7 _adcu7);
    virtual void setAdcu8(ADCU_8 _adcu8);
    virtual void setAdcu9(ADCU_9 _adcu9);
    virtual void setAdcu10(ADCU_10 _adcu10);
    virtual void setAdcu11(ADCU_11 _adcu11);
    virtual void setAdcu12(ADCU_12 _adcu12);

    //
    virtual float getVspd();

private:
    //interfaces used
    DRIVE *dri;
    SYSTEM *sys;

    RUN_CORE m_run_core;
    //datas used


    bool m_to_control;
    bool controlled();

    bool m_activated;

    std::mutex m_mutex;

    //
    vector<uint8_t> sendNum;
    vector<uint8_t> sendCounter;



    //
    VCU1 vcu1;
    void setVcu1(VCU1 _vcu1);

    VCU2 vcu2;
    void setVcu2(VCU2 _vcu2);

    VCU3 vcu3;
    void setVcu3(VCU3 _vcu3);

    VCU4 vcu4;
    void setVcu4(VCU4 _vcu4);

    VCU5 vcu5;
    void setVcu5(VCU5 _vcu5);

    VCU6 vcu6;
    void setVcu6(VCU6 _vcu6);

    VCU7 vcu7;
    void setVcu7(VCU7 _vcu7);

    VCU8 vcu8;
    void setVcu8(VCU8 _vcu8);

    VCU9 vcu9;
    void setVcu9(VCU9 _vcu9);

    VCU10 vcu10;
    void setVcu10(VCU10 _vcu10);

    VCU11 vcu11;
    void setVcu11(VCU11 _vcu11);

    VCU12 vcu12;
    void setVcu12(VCU12 _vcu12);

    VCU13 vcu13;
    void setVcu13(VCU13 _vcu13);

    VCU14 vcu14;
    void setVcu14(VCU14 _vcu14);

    VCU15 vcu15;
    void setVcu15(VCU15 _vcu15);

    ADCU_1 adcu1;
    ADCU_1 getAdcu1();

    ADCU_2 adcu2;
    ADCU_2 getAdcu2();

    ADCU_3 adcu3;
    ADCU_3 getAdcu3();

    ADCU_4 adcu4;
    ADCU_4 getAdcu4();

    ADCU_5 adcu5;
    ADCU_5 getAdcu5();

    ADCU_6 adcu6;
    ADCU_6 getAdcu6();

    ADCU_7 adcu7;
    ADCU_7 getAdcu7();

    ADCU_8 adcu8;
    ADCU_8 getAdcu8();

    ADCU_9 adcu9;
    ADCU_9 getAdcu9();

    ADCU_10 adcu10;
    ADCU_10 getAdcu10();

    ADCU_11 adcu11;
    ADCU_11 getAdcu11();

    ADCU_12 adcu12;
    ADCU_12 getAdcu12();


    /* parse related */
    void throtParse(can_frm_timed_t *_can_obj);
    void gearParse(can_frm_timed_t *_can_obj);
    void epsParse(can_frm_timed_t *_can_obj);
    void ebsParse(can_frm_timed_t *_can_obj);
    void retarderParse(can_frm_timed_t *_can_obj);
    void bodyelecParse(can_frm_timed_t *_can_obj);
    void handshakeParse(can_frm_timed_t *_can_obj);
    void diagnose(can_frm_timed_t *_can_obj);

    /* package related */
    void package();
    void handshakePackage(can_frm_t *_can_frm,uint8_t counter);
    void throtPackage(can_frm_t *_can_frm,uint8_t counter);
    void ebsPackage(can_frm_t *_can_frm,uint8_t counter);
    void epbPackage(can_frm_t *_can_frm,uint8_t counter);
    void retarderPackage(can_frm_t *_can_frm,uint8_t counter);
    void epsPackage(can_frm_t *_can_frm,uint8_t counter);
    void gearPackage(can_frm_t *_can_frm,uint8_t counter);
    void liftPackage(can_frm_t *_can_frm,uint8_t counter);
    void positionPackage(can_frm_t *_can_frm,uint8_t counter);
    void elevationPackage(can_frm_t *_can_frm,uint8_t counter);
    void orientationPackage(can_frm_t *_can_frm,uint8_t counter);
    void bodyelecPackage(can_frm_t *_can_frm,uint8_t counter);
};

#endif // PRO_VEHICLE_H
