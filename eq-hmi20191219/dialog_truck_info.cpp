#include "dialog_truck_info.h"
#include "ui_dialog_truck_info.h"

Dialog_truck_info::Dialog_truck_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_truck_info)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);


    timerId=startTimer(100);
}

Dialog_truck_info::~Dialog_truck_info()
{
    delete ui;
}

void Dialog_truck_info::on_pushButton_stop_clicked()
{
    Dialog_confirm *confirm =new Dialog_confirm();
    SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq=getSurrdTruckInfoReq();

    QString _string =QString::fromStdString(_surrdTruckInfoReq.vehicleNum)+ "卡车紧急停车确认";
    confirm->setConfirmInfo(_string);
    confirm->exec();

    if(confirm->confirmed())
    {

        SURRD_TRUCK_CTRL_REQ _surrTruckCtrlReq;
        _surrTruckCtrlReq.isNew=true;
        _surrTruckCtrlReq.vehicleNum=_surrdTruckInfoReq.vehicleNum;
        _surrTruckCtrlReq.ctrlType=0;
        sys->telecom->setSurrdTruckCtrlReq(_surrTruckCtrlReq);
    }
    else
    {

    }

    delete confirm;

}

void Dialog_truck_info::on_pushButton_close_clicked()
{
    close();
}

void Dialog_truck_info::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timerId)
    {
        SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq=getSurrdTruckInfoReq();
        ui->label_vehicle_number->setText(QString::fromStdString(_surrdTruckInfoReq.vehicleNum));
        getVehicleTask_respond_t _surrdTruckInfo=sys->telecom->getSurrdTruckInfo();
        if(_surrdTruckInfo.code=="200")
        {
            ui->label_vehicle_task->setText(QString::fromStdString(_surrdTruckInfo.task_type));
            ui->label_vehicle_load->setText(QString::fromStdString(_surrdTruckInfo.load_state));
        }
        else
        {


            sys->telecom->setSurrdTruckInfoReq(_surrdTruckInfoReq);


            ui->label_vehicle_task->setText("获取中。。");
            ui->label_vehicle_load->setText("获取中。。");
        }
    }
}

void Dialog_truck_info::setSurrdTruckInfoReq(SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrdTruckInfoReq=_surrdTruckInfoReq;
}

SURRD_TRUCK_INFO_REQ Dialog_truck_info::getSurrdTruckInfoReq()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrdTruckInfoReq;
}

SURRD_TRUCK_CTRL_REQ Dialog_truck_info::getSurrdTruckCtrlReq()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return surrdTruckCtrlReq;
}

void Dialog_truck_info::setSurrdTruckCtrlReq(SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    surrdTruckCtrlReq=_surrdTruckCtrlReq;
}
