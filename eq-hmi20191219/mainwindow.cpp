#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDateTime>


extern bool main_p0_start;
extern bool main_p1_start;
extern bool main_p2_start;

std::thread t0;
std::thread t1;
std::thread t2;


void thread0(void)
{
    main_p0();
}

void thread1(void)
{
    main_p1();
}

void thread2(void)
{
    main_p2();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //-----------------------------------------------------------------------------

    if(false == initPublic())
        while(true){}     //force to reset

    if(false == initSystem())
        while(true){}     //force to reset

    if(false==initDrive())
        while(true){}     //force to reset

    if(false==initProtocol())
        while(true){}     //force to reset

//    if(false==initAlgorithm())
//        while(true){}     //force to reset

    t0 = std::thread(thread0);
    main_p0_start=true;
    t1 = std::thread(thread1);
    main_p1_start=true;
    t2 = std::thread(thread2);
    main_p2_start=true;


    //----------------------------------------------------------------------
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);

    timeRefresh=new QTimer(this);
    connect(timeRefresh,SIGNAL(timeout()),this,SLOT(dataRefresh()));
    timeRefresh->start(100);

    //
    ui->label_button_status_1_1->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_1_2->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_2_1->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_2_2->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_3_1->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_3_2->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_3_3->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_3_4->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_3_5->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_4_1->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_4_2->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_4_3->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_5_1->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_button_status_5_2->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->widget_status->setVisible(false);
    ui->widget_function->setVisible(false);
    ui->widget_calibration->setVisible(false);

    login=new Dialog_drive_login();
    error_declare=new Dialog_error_declare();
    delay_request=new Dialog_delay_request();
    enter_code =new Dialog_enter_code();
    sysconfig=new Dialog_system_config();

    connect(this,SIGNAL(sigConfig()),sysconfig,SLOT(showConfig()));
    connect(timeRefresh,SIGNAL(timeout()),ui->widget,SLOT(update()));
    connect(this,SIGNAL(sigShowMessage(string)),this,SLOT(show_message_box(string)));

}

MainWindow::~MainWindow()
{

    main_p0_start=false;
    t0.join();

    main_p1_start=false;
    t1.join();

    main_p2_start=false;
    t2.join();

    //------------------

    timeRefresh->stop();
    delete timeRefresh;

    login->close();
    delete login;

    error_declare->close();
    delete error_declare;

    delay_request->close();
    delete delay_request;

    enter_code->close();
    delete enter_code;

    sysconfig->close();
    delete sysconfig;

    delete ui;
}

//获取中文字段的GBK编码值
vector<uint8_t> encode(const string s)
{
    string str = s;
    vector<uint8_t> buff;

    int len = str.length();
    buff.resize(len);
    memcpy(&buff[0], str.c_str(), len);

    return buff;
}

//把GBK编码值解码成中文字段
string decode(vector<uint8_t> buff)
{
    vector<uint8_t>debuf = buff;
    debuf.resize(buff.size() + 1);
    string str = (char *)&debuf[0];
    return str;
}


void MainWindow::dataRefresh()
{
    setConfig(sys->telecom->getTerminalConfig());
    setInfo(sys->telecom->getTerminalInfo());
    setStatus(DEVICE_STATUS(sys->telecom->getTerminalInfo().device_status));

    TERMINAL_CONFIG _config=getConfig();
    TERMINAL_INFO _info=getInfo();
//    _info.ground_origin=point_3d_t(110.257,39.8825,1376.49);
    protocol->gnss_receiver->setTerminalInfo(_info);

    DEVICE_STATUS _device_status=getStatus();
    nav_t basePoint=protocol->gnss_receiver->getBasePoint();


//    geographicLib_coordinate_transfer gct;
    QFont font;



    //ui display
    if(_info.device_type==Excavator)
    {
        static size_t s_count=0;
        if(s_count<=59)
            s_count++;
        else
            s_count=0;

        ui->label_button_status_1_1->setVisible(true);
        ui->label_button_status_1_2->setVisible(true);
        ui->label_button_status_2_1->setVisible(true);
        ui->label_button_status_2_2->setVisible(true);
        ui->label_button_status_3_1->setVisible(true);
        ui->label_button_status_3_2->setVisible(true);
        ui->label_button_status_3_3->setVisible(true);

        ui->label_button_status_3_5->setVisible(false);
        ui->label_button_status_4_1->setVisible(true);
        ui->label_button_status_4_2->setVisible(true);
        ui->label_button_status_4_3->setVisible(true);
        ui->label_button_status_5_1->setVisible(true);
        ui->label_button_status_5_2->setVisible(true);


        ui->pushButton_send_truck->setVisible(true);
        ui->pushButton_load_finish->setText("装完");
        font.setPointSize(23);
        ui->pushButton_load_finish->setFont(font);
        ui->pushButton_calibrate->setText("人工\n标定");
        if(!ui->pushButton_calibrate_auto->isChecked())
            ui->pushButton_calibrate_auto->setText("自动\n标定");
        else
        {
            if(s_count%20==0)
                ui->pushButton_calibrate_auto->setText("自动\n标定\n中");
            else if(s_count%20==1)
                ui->pushButton_calibrate_auto->setText("自动\n标定\n中。");
            else if(s_count%20==2)
                ui->pushButton_calibrate_auto->setText("自动\n标定\n中。。");
        }

        ui->pushButton_calibrate_auto->setVisible(true);
//        ui->pushButton_calibrate->setFont(font);


        ui->label_button_status_1_1->setText("挖机:"+QString::fromStdString(_info.device_num));
        ui->label_button_status_1_2->setText(QString::fromStdString(_info.driver_name));
        ui->label_button_status_2_1->setText("设备状态");
        if(_info.device_status==1)
            ui->label_button_status_2_2->setText("作业");
        else if(_info.device_status==2)
            ui->label_button_status_2_2->setText("备用");
        else if(_info.device_status==3)
            ui->label_button_status_2_2->setText("故障");
        else if(_info.device_status==4)
            ui->label_button_status_2_2->setText("延时");
        else if(_info.device_status==5)
            ui->label_button_status_2_2->setText("收车");
        else
            ui->label_button_status_2_2->setText("");


        ui->label_button_status_3_1->setText("作业状态");
        ui->label_button_status_3_2->setText(QString::fromStdString(_info.es.work_status));
        ui->label_button_status_3_3->setText("组内卡车");
        if(_info.es.group_truck.size()>0)
        {
            ui->label_button_status_3_4->setVisible(true);
            ui->label_button_status_3_4->setText(QString::fromStdString(_info.es.group_truck.front()));
        }
        else
        {
            ui->label_button_status_3_4->setVisible(false);
        }

        ui->label_button_status_4_1->setText("作业平台");
        ui->label_button_status_4_2->setText(QString::fromStdString(_info.es.work_platform));
        ui->label_button_status_4_3->setText(QString::fromStdString(_info.es.material_type));
        ui->label_button_status_5_1->setText("产量");
        ui->label_button_status_5_2->setText(QString::number(_info.es.output));


        //load point
        //bucket
        nav_t _reachPoint=protocol->gnss_receiver->getReferencePos();
        setReachPoint(_reachPoint);
        DIRC_POINT bucketPos;
        bucketPos.heading=_reachPoint.heading;
        bucketPos.pos_xy.x=_reachPoint.pos.x-basePoint.pos.x;
        bucketPos.pos_xy.y=_reachPoint.pos.y-basePoint.pos.y;
        ui->widget->setReachPoint(bucketPos);


        LOAD_POS _autoLoadPos=sys->telecom->getAutoLoadposition();
//        if(_autoLoadPos.isValid)
        {
            _autoLoadPos.basePoint.pos_xy.x-=basePoint.pos.x;
            _autoLoadPos.basePoint.pos_xy.y-=basePoint.pos.y;

            for(int i=0;i<_autoLoadPos.endpoint_xy.size();i++)
            {
                _autoLoadPos.endpoint_xy[i].x-=basePoint.pos.x;
                _autoLoadPos.endpoint_xy[i].y-=basePoint.pos.y;
            }
            ui->widget->setAutoLoadPoint(_autoLoadPos);

        }


        //last load position
        LOAD_POS _lastLoadPos=sys->telecom->getLoadposition();
        if(_lastLoadPos.isValid)
        {
            _lastLoadPos.basePoint.pos_xy.x-=basePoint.pos.x;
            _lastLoadPos.basePoint.pos_xy.y-=basePoint.pos.y;

            for(int i=0;i<_lastLoadPos.endpoint_xy.size();i++)
            {
                _lastLoadPos.endpoint_xy[i].x-=basePoint.pos.x;
                _lastLoadPos.endpoint_xy[i].y-=basePoint.pos.y;
            }
            ui->widget->setLastLoadPoint(_lastLoadPos);

        }

        //load position
        LOAD_POS _loadPos=getLoadPoint();
        if(_loadPos.isValid)
        {
            _loadPos.basePoint.pos_xy.x-=basePoint.pos.x;
            _loadPos.basePoint.pos_xy.y-=basePoint.pos.y;

            for(int i=0;i<_loadPos.endpoint_xy.size();i++)
            {
                _loadPos.endpoint_xy[i].x-=basePoint.pos.x;
                _loadPos.endpoint_xy[i].y-=basePoint.pos.y;
            }
            ui->widget->setLoadPoint(_loadPos);
        }

    }
    else
    {
        ui->label_button_status_1_1->setVisible(true);
        ui->label_button_status_1_2->setVisible(true);
        ui->label_button_status_2_1->setVisible(true);
        ui->label_button_status_2_2->setVisible(true);
        ui->label_button_status_3_1->setVisible(false);
        ui->label_button_status_3_2->setVisible(true);
        ui->label_button_status_3_3->setVisible(false);
        ui->label_button_status_3_4->setVisible(true);
        ui->label_button_status_3_5->setVisible(false);
        ui->label_button_status_4_1->setVisible(true);
        ui->label_button_status_4_2->setVisible(true);
        ui->label_button_status_4_3->setVisible(false);
        ui->label_button_status_5_1->setVisible(true);
        ui->label_button_status_5_2->setVisible(false);

        ui->pushButton_send_truck->setVisible(false);
        ui->pushButton_load_finish->setText("卸载位扫描");
        font.setPointSize(15);
        ui->pushButton_load_finish->setFont(font);
        ui->pushButton_calibrate->setText("卸车指派");
        ui->pushButton_calibrate->setFont(font);
        ui->pushButton_calibrate_auto->setVisible(false);




        ui->label_button_status_1_1->setText("铲车:"+QString::fromStdString(_info.device_num));
        ui->label_button_status_1_2->setText(QString::fromStdString(_info.driver_name));
        ui->label_button_status_2_1->setText("设备状态");
        if(_info.device_status==1)
            ui->label_button_status_2_2->setText("作业");
        else if(_info.device_status==2)
            ui->label_button_status_2_2->setText("备用");
        else if(_info.device_status==3)
            ui->label_button_status_2_2->setText("故障");
        else if(_info.device_status==4)
            ui->label_button_status_2_2->setText("延时");
        else if(_info.device_status==5)
            ui->label_button_status_2_2->setText("收车");
        else
            ui->label_button_status_2_2->setText("");

        ui->label_button_status_3_2->setText(QString::fromStdString("作业状态"));
        ui->label_button_status_3_4->setText(QString::fromStdString(_info.fs.work_status));
        ui->label_button_status_4_1->setText("调度指令");
        ui->label_button_status_4_2->setText(QString::fromStdString(_info.fs.dispatch_order));
        ui->label_button_status_5_1->setText(QString::fromStdString("主动任务预留"));
    }

    //surrd vehicle
    surrd_vehicle_packet_t _surrdVeh=sys->telecom->getSurrdVehicleInfo();

    for(size_t i=0;i<_surrdVeh.number;i++)
    {
        _surrdVeh.vehicles[i].x-=basePoint.pos.x;
        _surrdVeh.vehicles[i].y-=basePoint.pos.y;
    }
    ui->widget->setSurrdTrucks(_surrdVeh);

    if(_surrdVeh.number>0)
    {


    }
    else
    {
        ui->pushButton_truck0->setVisible(true);
        ui->pushButton_truck1->setVisible(true);
        ui->pushButton_truck2->setVisible(false);
        ui->pushButton_truck3->setVisible(false);
    }


    //message
    cloud_info _cloudInfo=sys->telecom->getNewMessage();
    if(_cloudInfo.new_message_flag)
    {
        QByteArray str;
        string k2((char*)_cloudInfo.message_packet.content.data());
        QString s = QString::fromStdString(k2);
        for(int i=0;i<(s.length()/2);i++)
        {
            uchar b=s.mid(i*2,2).toInt(nullptr,16);
            str.push_back(b);
        }


        string result =QTextCodec::codecForName("GBK")->toUnicode(str).toStdString();
        emit sigShowMessage(result);
    }

    //warning
    INTERNAL_WARNING _internalWarning=sys->telecom->getInternalWarning();
    if(_internalWarning.newWarning)
    {
        emit sigShowMessage(_internalWarning.warningMessage);
    }

    ui->label_time->setText(QTime::currentTime().toString());
    if(basePoint.align_status=="NARROW_INT")
    {
        ui->label_signal_good->setVisible(true);
        ui->label_signal_poor->setVisible(false);
    }
    else
    {
        ui->label_signal_good->setVisible(false);
        ui->label_signal_poor->setVisible(true);
    }

    //map
    vector<point_lidarmap_t> mapdataLeftBoundary=sys->telecom->getLeftBoundary();
    vector<point_lidarmap_t> mapdataRightBoundary=sys->telecom->getRightBoundary();
    for(int i=0;i<mapdataLeftBoundary.size();i++)
    {
        mapdataLeftBoundary[i].p.x-=basePoint.pos.x;
        mapdataLeftBoundary[i].p.y-=basePoint.pos.y;
        mapdataLeftBoundary[i].p.z-=basePoint.pos.z;

    }

    for(int i=0;i<mapdataRightBoundary.size();i++)
    {
        mapdataRightBoundary[i].p.x-=basePoint.pos.x;
        mapdataRightBoundary[i].p.y-=basePoint.pos.y;
        mapdataRightBoundary[i].p.z-=basePoint.pos.z;

    }

    ui->widget->setLeftBoundary(mapdataLeftBoundary);
    ui->widget->setRightBoundary(mapdataRightBoundary);





    //ui action
    uint8_t _log_status=_info.driver_log_status;
    ui->pushButton_function_1->setEnabled(_log_status==0);
    ui->pushButton_function_2->setEnabled((_log_status==1)&&(_device_status!=Fault));
    ui->pushButton_function_3->setEnabled((_log_status==1)&&(_device_status!=Delay));
    ui->pushButton_ready->setEnabled((_log_status==1)&&(_device_status!=Ready));
    ui->pushButton_function_4->setEnabled(true);
    ui->pushButton_logout->setEnabled(_log_status==1);

    if(_info.device_type==Excavator)
    {
        LOAD_POS _autoLoadPos=sys->telecom->getAutoLoadposition();
        ui->pushButton_calibrate->setEnabled((_log_status==1)&&(_device_status==Ready)&&(!ui->widget_calibration->isVisible()));
        ui->pushButton_calibrate_auto->setEnabled(allowAutoCalibrate()&&(_log_status==1));

        ui->pushButton_send_truck->setEnabled(((_log_status==1)&&(_device_status==Ready)&&sys->telecom->getLoadPosCalibrated()&&!truckSend()));

//        ui->pushButton_send_truck->setEnabled(((_log_status==1)&&(_device_status==Ready)&&getLoadPosCalibrated())||ui->pushButton_calibrate_auto->isChecked());

        ui->pushButton_load_finish->setEnabled(((_log_status==1)&&(_device_status==Ready)&&sys->telecom->getLoadPosCalibrated()&&truckSend()));
        if(_log_status!=1)
            ui->pushButton_calibrate_auto->setChecked(false);

        ui->pushButton_clean_loadarea->setEnabled((_log_status==1)&&(_device_status==Ready));
    }
    else
    {
        ui->pushButton_load_finish->setEnabled((_log_status==1)&&(_device_status==Ready));
        ui->pushButton_calibrate->setEnabled((_log_status==1)&&(_device_status==Ready));
    }
}

void MainWindow::on_pushButton_status_clicked()
{
    ui->widget_status->setVisible(true);
    ui->pushButton_status->setVisible(false);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(ui->widget_status->outThis())
    {
        ui->widget_status->setVisible(false);
        ui->pushButton_status->setVisible(true);
    }

    if(ui->widget_function->outThis())
    {
        ui->widget_function->setVisible(false);
        ui->pushButton_function->setVisible(true);
    }


    if(ui->pushButton_hand->isChecked())
        ui->widget->mouseDown();



}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(ui->pushButton_hand->isChecked())
        ui->widget->mouseUp();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(ui->pushButton_hand->isChecked())
        ui->widget->mouseMove();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void MainWindow::enterEvent(QEvent *event)
{

}

void MainWindow::on_pushButton_function_clicked()
{
    ui->pushButton_function->setVisible(false);
    ui->widget_function->setVisible(true);
}

void MainWindow::on_pushButton_function_1_clicked()
{
    login->exec();

    if(login->confirmed())
    {
        sys->telecom->setDriverNum(login->getDriverId());
        sys->telecom->setlogin(false);
    }
    else
    {

    }

}

void MainWindow::on_pushButton_function_2_clicked()
{
    error_declare->exec();

    if(error_declare->confirmed())
    {
        sys->telecom->setErrorType(error_declare->getErrorType());
        sys->telecom->setErrorDescription(error_declare->getErrorDescription());
        sys->telecom->setErrorDeclare(false);
//        sys->telecom->setDeviceStatus(3);
    }
    else
    {

    }
}

void MainWindow::on_pushButton_function_3_clicked()
{
    delay_request->exec();

    if(delay_request->confirmed())
    {
        DELAY_REQUEST _delayRequest;
        _delayRequest.isDelayRequestOk=false;
        _delayRequest.delayType=delay_request->getDelayType();
        _delayRequest.delayMinutes=delay_request->getDelayMinutes();
        sys->telecom->setDelayRequest(_delayRequest);
//        sys->telecom->setDeviceStatus(4);
    }
}

void MainWindow::on_pushButton_function_4_clicked()
{

    if(enter_code->getTerminalCode()!=getConfig().terminal_code)
    {
        enter_code->exec();

        if(enter_code->getTerminalCode()==getConfig().terminal_code)
        {
            sysconfig->setTerminalConfig(getConfig());
            emit sigConfig();
            sysconfig->exec();
            sys->telecom->setTerminalConfig(sysconfig->getTerminalConfig());
            sys->telecom->setWriteConfig(true);
        }
    }
    else
    {
        sysconfig->setTerminalConfig(getConfig());
        emit sigConfig();
        sysconfig->exec();
        sys->telecom->setTerminalConfig(sysconfig->getTerminalConfig());
        sys->telecom->setWriteConfig(true);
    }
}

DEVICE_STATUS MainWindow::getStatus()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return status;
}

void MainWindow::setStatus(DEVICE_STATUS _status)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    status=_status;
}

LOAD_POS MainWindow::getLoadPoint()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadPoint;
}

void MainWindow::setLoadPoint(LOAD_POS _loadPoint)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadPoint=_loadPoint;
}

LOAD_POS MainWindow::getUnloadPoint()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return unloadPoint;
}

void MainWindow::setUnloadPoint(LOAD_POS _unloadPoint)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    unloadPoint=_unloadPoint;
}

LOAD_POS MainWindow::getFinalLoadPoint()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return finalLoadPoint;
}

void MainWindow::setFinalLoadPoint(LOAD_POS _finalLoadPoint)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    finalLoadPoint=_finalLoadPoint;
}

nav_t MainWindow::getReachPoint()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return reachPoint;
}

void MainWindow::setReachPoint(nav_t _reachPoint)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    reachPoint=_reachPoint;
}

LOAD_POS MainWindow::getLoadPos()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadPos;
}

void MainWindow::setLoadPos(LOAD_POS _loadPos)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadPos=_loadPos;
}

LOAD_POS MainWindow::getLastLoadPos()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return lastloadPos;
}

void MainWindow::setLastLoadPos(LOAD_POS _lastloadPos)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    lastloadPos=_lastloadPos;
}

void MainWindow::calcAreaEndPoints(LOAD_POS &_loadPos)
{
    geographicLib_coordinate_transfer gct;
    gct.setGroundOrigin(getInfo().ground_origin);
    nav_t basePoint;
    basePoint.pos.x=_loadPos.basePoint.pos_xy.x;
    basePoint.pos.y=_loadPos.basePoint.pos_xy.y;
    basePoint.heading=_loadPos.basePoint.heading;

    _loadPos.endpoint_xy.clear();

    point_3d_t offsetfl(gct.lonF,gct.lat/2,0);
    point_3d_t _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetfl,basePoint);
    _loadPos.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

    point_3d_t offsetfr(gct.lonF,-gct.lat/2,0);
    _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetfr,basePoint);
    _loadPos.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

    point_3d_t offsetrr(-gct.lonR,-gct.lat/2,0);
    _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetrr,basePoint);
    _loadPos.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));

    point_3d_t offsetrl(-gct.lonR,gct.lat/2,0);
    _endpoint_xy=gct.Coordinate_TransfertoWorld(offsetrl,basePoint);
    _loadPos.endpoint_xy.push_back(point_2d_t(_endpoint_xy.x,_endpoint_xy.y));
}

bool MainWindow::truckSend()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return isTruckSend;
}

void MainWindow::setTruckSend(bool _isTruckSend)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    isTruckSend=_isTruckSend;
}

bool MainWindow::getLoadPosCalibrated()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadPosCalibrated;
}

void MainWindow::setLoadPosCalibrated(bool _loadPosCalibrated)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadPosCalibrated=_loadPosCalibrated;
}

bool MainWindow::allowAutoCalibrate()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return isAllowAutoCalibrate;
}

void MainWindow::setAllowAutoCalibrate(bool _isAllowAutoCalibrate)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    isAllowAutoCalibrate=_isAllowAutoCalibrate;
}

void MainWindow::on_pushButton_ready_clicked()
{
    Dialog_confirm *confirm =new Dialog_confirm();
    confirm->setConfirmInfo("就绪确认");
    confirm->exec();

    if(confirm->confirmed())
    {
        sys->telecom->setDeviceStatus(1);
    }
    else
    {

    }

    delete confirm;
}

void MainWindow::on_pushButton_logout_clicked()
{
    Dialog_confirm *confirm =new Dialog_confirm();
    confirm->setConfirmInfo("登出确认");
    confirm->exec();

    if(confirm->confirmed())
    {
        sys->telecom->setlogout(false);
        sys->telecom->setDeviceStatus(5);
        setLoadPosCalibrated(false);
    }
    else
    {

    }

    delete confirm;
}

void MainWindow::on_pushButton_load_finish_clicked()
{
    TERMINAL_INFO _info=getInfo();
    if(_info.device_type==Excavator)
    {

        Dialog_confirm_load *confirm =new Dialog_confirm_load();
        confirm->exec();

        if(confirm->confirmed())
        {
            TERMINAL_REQUEST _terminalRequest;
            _terminalRequest.isHttpTerminalRequestOk=false;
            _terminalRequest.isTcpTerminalRequestOk=false;
            _terminalRequest.request_type=3;
            _terminalRequest.isHard=confirm->hard();
            sys->telecom->setTerminalRequest(_terminalRequest);
            setLoadPosCalibrated(false);
            setTruckSend(false);
        }
        else
        {

        }

        delete confirm;
    }
    else
    {
        Dialog_unload_place_scan *unload_place_scan=new Dialog_unload_place_scan();
        unload_place_scan->exec();
        if(unload_place_scan->confirmed())
        {
            FORKLIFT_SAMPLE_GIS_PACKET _packet=unload_place_scan->getForkliftSampleGisPacket();
            _packet.isNewMessage=true;
            sys->telecom->setForkliftSampleGisPacket(_packet);
        }

        delete unload_place_scan;
    }
}

void MainWindow::on_pushButton_calibrate_clicked()
{
    TERMINAL_INFO _info=getInfo();
    if(_info.device_type==Excavator)
    {
        Dialog_choose_load_mode *confirm =new Dialog_choose_load_mode();
        confirm->exec();

        if(confirm->confirmed())
        {
            LOAD_POS _lastLoadPos=sys->telecom->getLoadposition();
            ui->widget_calibration->setVisible(true);

            nav_t _reacharea=getReachPoint();
            nav_t basePoint=protocol->gnss_receiver->getBasePoint();
            uint8_t _loadMode=confirm->getLoadMode();

            LOAD_POS _loadposition=getLoadPoint();
            _loadposition.load_mode=_loadMode;
            _loadposition.calibrate_mode=1;
            _loadposition.isValid=true;

            geographicLib_coordinate_transfer gct;
            gct.setGroundOrigin(getInfo().ground_origin);
            point_3d_t _pos;

            switch (_loadMode)
            {
                case 1:
                {


                    _pos=gct.Coordinate_TransfertoWorld(point_3d_t(0,0,0),_reacharea);
                    _loadposition.basePoint.heading=_reacharea.heading;


                    break;
                }
                case 2:
                {

                     _pos=gct.Coordinate_TransfertoWorld(point_3d_t(1,0,0),_reacharea);
                    _loadposition.basePoint.heading=_reacharea.heading-90;

                    break;
                }
            }
            _loadposition.basePoint.pos_xy.x=_pos.x;
            _loadposition.basePoint.pos_xy.y=_pos.y;


            while(_loadposition.basePoint.heading<0)
                _loadposition.basePoint.heading+=360;

            while(_loadposition.basePoint.heading>=360)
                _loadposition.basePoint.heading-=360;



            calcAreaEndPoints(_loadposition);

            setLoadPoint(_loadposition);
            ui->widget->setShowLoadArea(true);
            ui->pushButton_calibrate->setEnabled(false);
        }
        else
        {

        }

        delete confirm;

//        LOAD_POS _lastLoadPos=sys->telecom->getLoadposition();
//        if(_lastLoadPos.isValid)
//        {

//            Dialog_confirm *confirm =new Dialog_confirm();
//            confirm->setConfirmInfo("是否沿用上次装载位");
//            confirm->exec();

//            if(confirm->confirmed())
//            {
//                LOAD_POS_UPDATE _loadPosUpdate;
//                _loadPosUpdate.isNewLoadPos=true;
//                _loadPosUpdate.updateType=0;
//                sys->telecom->setNewLoadPos(_loadPosUpdate);
//                setLoadPosCalibrated(true);
//            }
//            else
//            {
//                ui->widget_calibration->setVisible(true);
//                LOAD_POS _loadposition=getLoadPoint();
//                nav_t _reacharea=getReachPoint();
//                _loadposition.isValid=true;
//                _loadposition.basePoint.pos_xy.x=_reacharea.pos.x;
//                _loadposition.basePoint.pos_xy.y=_reacharea.pos.y;
//                _loadposition.basePoint.heading=_lastLoadPos.basePoint.heading;

//                calcAreaEndPoints(_loadposition);

//                setLoadPoint(_loadposition);
//                ui->widget->setShowLoadArea(true);
//                ui->pushButton_calibrate->setEnabled(false);
//            }

//            delete confirm;
//        }
//        else
//        {
//            ui->widget_calibration->setVisible(true);
//            LOAD_POS _loadposition=getLoadPoint();
//            nav_t _reacharea=getReachPoint();
//            _loadposition.isValid=true;
//            _loadposition.basePoint.pos_xy.x=_reacharea.pos.x;
//            _loadposition.basePoint.pos_xy.y=_reacharea.pos.y;
//            _loadposition.basePoint.heading=_reacharea.heading;
//            calcAreaEndPoints(_loadposition);
//            setLoadPoint(_loadposition);
//            ui->widget->setShowLoadArea(true);
//            ui->pushButton_calibrate->setEnabled(false);
//        }
    }
    else
    {
        ui->widget_calibration->setVisible(true);
        LOAD_POS _unloadplace;
//        nav_t _basepoint=();
//        _loadposition.isValid=true;
//        _loadposition.basePoint.pos_xy.x=_reacharea.pos.x;
//        _loadposition.basePoint.pos_xy.y=_reacharea.pos.y;
//        _loadposition.basePoint.heading=_lastLoadPos.basePoint.heading;

//        calcAreaEndPoints(_loadposition);

//        setLoadPoint(_loadposition);
//        ui->widget->setShowLoadArea(true);
//        ui->pushButton_calibrate->setEnabled(false);
    }
}

void MainWindow::on_pushButton_calibrate_confirm_clicked()
{
    ui->widget_calibration->setVisible(false);
    ui->widget->setShowLoadArea(false);
    LOAD_POS _loadPoint=getLoadPoint();
    nav_t basePoint=protocol->gnss_receiver->getBasePoint();
    _loadPoint.coordinate=point_3d_t(basePoint.longitude,basePoint.latitude,basePoint.altitude);

    LOAD_POS_UPDATE _loadPosUpdate;
    _loadPosUpdate.isNewLoadPos=true;
    _loadPosUpdate.updateType=1;
    sys->telecom->setLoadposition(_loadPoint);
    sys->telecom->setNewLoadPos(_loadPosUpdate);
    ui->pushButton_calibrate->setEnabled(true);
    setLoadPosCalibrated(true);
    setAllowAutoCalibrate(true);
    ui->pushButton_calibrate_auto->setChecked(false);
}

void MainWindow::on_pushButton__calibrate_cancel_clicked()
{
    ui->widget_calibration->setVisible(false);
    ui->widget->setShowLoadArea(false);
    ui->pushButton_calibrate->setEnabled(true);
}

TERMINAL_CONFIG MainWindow::getConfig()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return config;
}

void MainWindow::setConfig(TERMINAL_CONFIG _config)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    config=_config;
}

bool MainWindow::readConfigFile()
{
    QByteArray _configArray=drive->txt->read(configFileName);
    if(_configArray.size()>0)
    {
        char _terminal_code[50];
        char _terminal_num[50];
        char _sim_card_num[50];
        char _binding_device_num[50];
        int _type;

        uint byteoffset=0;
        memcpy(&_type,_configArray.data()+byteoffset,4);
        byteoffset+=4;
        memcpy(_terminal_code,_configArray.data()+byteoffset,50);
        byteoffset+=50;
        memcpy(_terminal_num,_configArray.data()+byteoffset,50);
        byteoffset+=50;
        memcpy(_sim_card_num,_configArray.data()+byteoffset,50);
        byteoffset+=50;
        memcpy(_binding_device_num,_configArray.data()+byteoffset,50);
        byteoffset+=50;


        TERMINAL_CONFIG _config;
        _config.terminal_type=(TERMINAL_TYPE)(_type);
        _config.terminal_code=_terminal_code;
        _config.terminal_num=_terminal_num;
        _config.sim_card_num=_sim_card_num;
        _config.binding_device_num=_binding_device_num;
        setConfig(_config);


        return true;
    }
    else
        return false;
}

bool MainWindow::writeConfigFile()
{
    QString fileName=QString::fromStdString(configFileName);
    QFile file(fileName);
    if(file.exists())
    {
        QFile::remove(fileName);
    }

    QByteArray _configArray;
    _configArray.resize(205);

    const char* _terminal_code=getConfig().terminal_code.data();
    const char* _terminal_num=getConfig().terminal_num.data();
    const char* _sim_card_num=getConfig().sim_card_num.data();
    const char* _binding_device_num=getConfig().binding_device_num.data();
    int _type=getConfig().terminal_type;


    uint byteoffset=0;
    memcpy(_configArray.data()+byteoffset,&_type,4);
    byteoffset+=4;
    memcpy(_configArray.data()+byteoffset,_terminal_code,50);
    byteoffset+=50;
    memcpy(_configArray.data()+byteoffset,_terminal_num,50);
    byteoffset+=50;
    memcpy(_configArray.data()+byteoffset,_sim_card_num,50);
    byteoffset+=50;
    memcpy(_configArray.data()+byteoffset,_binding_device_num,50);
    byteoffset+=50;
    drive->txt->write(configFileName,_configArray);


}

TERMINAL_INFO MainWindow::getInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalInfo;
}

void MainWindow::setInfo(TERMINAL_INFO _info)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalInfo=_info;
}

void MainWindow::on_pushButton_roll_clockwise_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.heading+=rollAngleInterval;
    while(_loadarea.basePoint.heading<0)
        _loadarea.basePoint.heading+=360;

    while(_loadarea.basePoint.heading>=360)
        _loadarea.basePoint.heading-=360;

    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_up_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.pos_xy.y+=moveDistInterval;
    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_roll_countorclockwise_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.heading-=rollAngleInterval;
    while(_loadarea.basePoint.heading<0)
        _loadarea.basePoint.heading+=360;

    while(_loadarea.basePoint.heading>=360)
        _loadarea.basePoint.heading-=360;

    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_left_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.pos_xy.x-=moveDistInterval;
    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_down_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.pos_xy.y-=moveDistInterval;
    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_right_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    _loadarea.basePoint.pos_xy.x+=moveDistInterval;
    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_send_truck_clicked()
{
    Dialog_confirm *confirm =new Dialog_confirm();
    confirm->setConfirmInfo("派车确认");
    confirm->exec();

    if(confirm->confirmed())
    {
        TERMINAL_REQUEST _terminalRequest;
        _terminalRequest.isTcpTerminalRequestOk=false;
        _terminalRequest.request_type=2;
        sys->telecom->setTerminalRequest(_terminalRequest);
        setTruckSend(true);
    }
    else
    {

    }

    delete confirm;
}

void MainWindow::on_pushButton_roll_plus_clicked()
{
    ui->widget->ZoomIn();
}

void MainWindow::on_pushButton_roll_minus_clicked()
{
    ui->widget->ZoomOut();
}

void MainWindow::on_pushButton_return_center_clicked()
{
    ui->widget->mouseDoubleClick();
}

void MainWindow::on_pushButton_v_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    double _reachareaHeading=getReachPoint().heading;
    double _reachareaHeading1=_reachareaHeading+90;
    double _reachareaHeading2=_reachareaHeading-90;

    while(_reachareaHeading1<0)
        _reachareaHeading1+=360;

    while(_reachareaHeading1>=360)
        _reachareaHeading1-=360;

    while(_reachareaHeading2<0)
        _reachareaHeading2+=360;

    while(_reachareaHeading2>=360)
        _reachareaHeading2-=360;

    if(_loadarea.basePoint.heading==_reachareaHeading1)
        _loadarea.basePoint.heading=_reachareaHeading2;
    else
        _loadarea.basePoint.heading=_reachareaHeading1;




    while(_loadarea.basePoint.heading<0)
        _loadarea.basePoint.heading+=360;

    while(_loadarea.basePoint.heading>=360)
        _loadarea.basePoint.heading-=360;

    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::on_pushButton_h_clicked()
{
    LOAD_POS _loadarea=getLoadPoint();
    double _reachareaHeading=getReachPoint().heading;
    _loadarea.basePoint.heading=_reachareaHeading;
    calcAreaEndPoints(_loadarea);
    setLoadPoint(_loadarea);
}

void MainWindow::show_message_box(string _messageContent)
{
    Dialog_confirm *confirm =new Dialog_confirm();
    confirm->setConfirmInfo(QString::fromStdString(_messageContent));
    confirm->exec();

    if(confirm->confirmed())
    {

    }
    else
    {

    }

    delete confirm;
}

void MainWindow::on_pushButton_clean_loadarea_clicked()
{
    Dialog_confirm *confirm =new Dialog_confirm();
    confirm->setConfirmInfo("确认清理装载位？");
    confirm->exec();

    if(confirm->confirmed())
    {
        nav_t _basePoint=protocol->gnss_receiver->getBasePoint();
        CMD_INFO _cmdInfo;
        _cmdInfo.isNewCmd=true;
        _cmdInfo.cmd_type=4;
        _cmdInfo.cmd_destination=point_3d_t(_basePoint.longitude,_basePoint.latitude,_basePoint.altitude);
        sys->telecom->setCmdInfo(_cmdInfo);
    }
    else
    {

    }

    delete confirm;
}

void MainWindow::on_pushButton_calibrate_auto_clicked()
{
    setAllowAutoCalibrate(false);

    LOAD_POS _loadPoint=getLoadPoint();
    nav_t basePoint=protocol->gnss_receiver->getBasePoint();
    _loadPoint.coordinate=point_3d_t(basePoint.longitude,basePoint.latitude,basePoint.altitude);
    _loadPoint.calibrate_mode=2;

    LOAD_POS_UPDATE _loadPosUpdate;
    _loadPosUpdate.isNewLoadPos=true;
    _loadPosUpdate.updateType=1;
    sys->telecom->setLoadposition(_loadPoint);
    sys->telecom->setNewLoadPos(_loadPosUpdate);
    setLoadPosCalibrated(true);



}

void MainWindow::on_pushButton_truck0_clicked()
{
    showTruckInfo(0);
}

void MainWindow::on_pushButton_truck1_clicked()
{
    showTruckInfo(1);
}

void MainWindow::showTruckInfo(int _truckNum)
{
    Dialog_truck_info *truck_info =new Dialog_truck_info();
    SURRD_TRUCK_INFO_REQ _stiq;
    _stiq.isNew=true;
//    string id (reinterpret_cast<char const*>(sys->telecom->getSurrdVehicleInfo().vehicles[_truckNum].id), 10);
    //    _stiq.vehicleNum=id;
    switch(_truckNum)
    {
    case 0:
    {
        _stiq.vehicleNum="10001";
        break;
    }
    case 1:
    {
        _stiq.vehicleNum="10002";
        break;
    }

    }

//    _stiq.vehicleNum= "TL850430CK";
    truck_info->setSurrdTruckInfoReq(_stiq);
    truck_info->exec();


    delete truck_info;
}
