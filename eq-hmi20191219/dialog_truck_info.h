#ifndef DIALOG_TRUCK_INFO_H
#define DIALOG_TRUCK_INFO_H

#include <QDialog>
#include"dialog_confirm.h"
#include<QDesktopWidget>
#include<mutex>
#include<string>
#include<QTimerEvent>
#include "header/system/pub_globals.h"


namespace Ui {
class Dialog_truck_info;
}

class Dialog_truck_info : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_truck_info(QWidget *parent = 0);
    ~Dialog_truck_info();


    void setSurrdTruckInfoReq(SURRD_TRUCK_INFO_REQ _surrdTruckInfoReq);

private slots:
    void on_pushButton_stop_clicked();

    void on_pushButton_close_clicked();

    void timerEvent(QTimerEvent *event);




private:
    Ui::Dialog_truck_info *ui;

    std::mutex m_mutex;

    int timerId=0;

    SURRD_TRUCK_INFO_REQ surrdTruckInfoReq;
    SURRD_TRUCK_INFO_REQ getSurrdTruckInfoReq();

    SURRD_TRUCK_CTRL_REQ surrdTruckCtrlReq;
    SURRD_TRUCK_CTRL_REQ getSurrdTruckCtrlReq();
    void setSurrdTruckCtrlReq(SURRD_TRUCK_CTRL_REQ _surrdTruckCtrlReq);

};

#endif // DIALOG_TRUCK_INFO_H
