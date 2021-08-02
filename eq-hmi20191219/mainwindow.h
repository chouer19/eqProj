#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<thread>
#include<QTimer>
#include<QTime>
#include<QTextCodec>
#include <codecvt>
#include"header/system/main_linux.h"
#include "header/system/pub_globals.h"
#include"header/system/sys_abstract.hpp"
#include "header/drive/dri_abstract.hpp"
#include "header/protocol/pro_abstract.hpp"
#include"header/algorithm/alg_abstract.hpp"
#include"dialog_drive_login.h"
#include"dialog_error_declare.h"
#include"dialog_delay_request.h"
#include"dialog_enter_code.h"
#include"dialog_confirm.h"
#include"dialog_system_config.h"
#include"qdisplaywidget2.h"
#include"dialog_unload_place_scan.h"
#include"dialog_confirm_load.h"
#include"dialog_choose_load_mode.h"
#include"dialog_truck_info.h"

enum DEVICE_STATUS
{
    Idle=0,
    Ready=1,
    reserve=2,
    Fault=3,
    Delay=4,
    workout=5
};


const string configFileName="./config";


//enum CONFIG_BIT
//{
//    Terminal_type=0,
//    Terminal_code,
//    Reserve=Terminal_code+codeLength
//};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_pushButton_status_clicked();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void enterEvent(QEvent *event);

    void on_pushButton_function_clicked();

    void on_pushButton_function_1_clicked();

    void dataRefresh();

    void on_pushButton_function_2_clicked();

    void on_pushButton_function_3_clicked();

    void on_pushButton_function_4_clicked();

    void on_pushButton_ready_clicked();

    void on_pushButton_logout_clicked();

    void on_pushButton_load_finish_clicked();

    void on_pushButton_calibrate_clicked();

    void on_pushButton_calibrate_confirm_clicked();

    void on_pushButton__calibrate_cancel_clicked();

    void on_pushButton_roll_clockwise_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_roll_countorclockwise_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_send_truck_clicked();

    void on_pushButton_roll_plus_clicked();

    void on_pushButton_roll_minus_clicked();

    void on_pushButton_return_center_clicked();

    void on_pushButton_v_clicked();

    void on_pushButton_h_clicked();

    void show_message_box(string _messageContent);

    void on_pushButton_clean_loadarea_clicked();

    void on_pushButton_calibrate_auto_clicked();

    void on_pushButton_truck0_clicked();

    void on_pushButton_truck1_clicked();

signals:
    void sigConfig();
    void sigShowMessage(string _messageContent);

private:
    Ui::MainWindow *ui;
    Dialog_drive_login *login;
    Dialog_error_declare *error_declare;
    Dialog_delay_request *delay_request;
    Dialog_enter_code *enter_code;
    Dialog_system_config *sysconfig;

    void showTruckInfo(int _truckNum);

    //config
    TERMINAL_CONFIG config;
    TERMINAL_CONFIG getConfig();
    void setConfig(TERMINAL_CONFIG _config);
    bool readConfigFile();
    bool writeConfigFile();

    //info
    TERMINAL_INFO terminalInfo;
    TERMINAL_INFO getInfo();
    void setInfo(TERMINAL_INFO _info);


    std::mutex m_mutex;
    QTimer *timeRefresh;


    DEVICE_STATUS status=Idle;
    DEVICE_STATUS getStatus();
    void setStatus(DEVICE_STATUS _status);

    LOAD_POS loadPoint;
    LOAD_POS getLoadPoint();
    void setLoadPoint(LOAD_POS _loadPoint);

    LOAD_POS unloadPoint;
    LOAD_POS getUnloadPoint();
    void setUnloadPoint(LOAD_POS _unloadPoint);

    LOAD_POS finalLoadPoint;
    LOAD_POS getFinalLoadPoint();
    void setFinalLoadPoint(LOAD_POS _finalLoadPoint);

    nav_t reachPoint;
    nav_t getReachPoint();
    void setReachPoint(nav_t _reachPoint);

    LOAD_POS loadPos;
    LOAD_POS getLoadPos();
    void setLoadPos(LOAD_POS _loadPos);

    LOAD_POS lastloadPos;
    LOAD_POS getLastLoadPos();
    void setLastLoadPos(LOAD_POS _lastloadPos);

    void calcAreaEndPoints(LOAD_POS &_loadPos);

    bool isTruckSend=false;
    bool truckSend();
    void setTruckSend(bool _isTruckSend);

    double moveDistInterval=0.25;
    double rollAngleInterval=5;

    bool loadPosCalibrated=false;
    bool getLoadPosCalibrated();
    void setLoadPosCalibrated(bool _loadPosCalibrated);

    bool isAllowAutoCalibrate=false;
    bool allowAutoCalibrate();
    void setAllowAutoCalibrate(bool _isAllowAutoCalibrate);


};

#endif // MAINWINDOW_H
