#include "dialog_unload_place_scan.h"
#include "ui_dialog_unload_place_scan.h"

Dialog_unload_place_scan::Dialog_unload_place_scan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_unload_place_scan)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_unload_place_scan::~Dialog_unload_place_scan()
{
    delete ui;
}

bool Dialog_unload_place_scan::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

FORKLIFT_SAMPLE_GIS_PACKET Dialog_unload_place_scan::getForkliftSampleGisPacket()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return forkliftSampleGisPacket;
}

void Dialog_unload_place_scan::on_pushButton_start_clicked()
{
    ui->pushButton_end->setEnabled(true);
    ui->pushButton_start->setEnabled(false);

    QDate date=QDate::currentDate();
    QTime time=  QTime::currentTime();
    utc_time _utcTime;
    _utcTime.year=date.year();
    _utcTime.month=date.month();
    _utcTime.day=date.day();
    _utcTime.hour=time.hour();
    _utcTime.minute=time.minute();
    _utcTime.second=time.second();
    _utcTime.millisecond=time.msec();

    FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket=getForkliftSampleGisPacket();
    _forkliftSampleGisPacket.start_time=_utcTime;
    setForkliftSampleGisPacket(_forkliftSampleGisPacket);

}

void Dialog_unload_place_scan::on_pushButton_end_clicked()
{
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_confirm->setEnabled(true);

    QDate date=QDate::currentDate();
    QTime time=  QTime::currentTime();
    utc_time _utcTime;
    _utcTime.year=date.year();
    _utcTime.month=date.month();
    _utcTime.day=date.day();
    _utcTime.hour=time.hour();
    _utcTime.minute=time.minute();
    _utcTime.second=time.second();
    _utcTime.millisecond=time.msec();

    FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket=getForkliftSampleGisPacket();
    _forkliftSampleGisPacket.end_time=_utcTime;
    setForkliftSampleGisPacket(_forkliftSampleGisPacket);

}

void Dialog_unload_place_scan::on_pushButton_confirm_clicked()
{
    ui->pushButton_confirm->setEnabled(false);
    setConfirm(true);
    close();
}

void Dialog_unload_place_scan::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    close();
}

void Dialog_unload_place_scan::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_unload_place_scan::setForkliftSampleGisPacket(FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    forkliftSampleGisPacket=_forkliftSampleGisPacket;
}
