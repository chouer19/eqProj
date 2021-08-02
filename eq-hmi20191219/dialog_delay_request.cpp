#include "dialog_delay_request.h"
#include "ui_dialog_delay_request.h"

Dialog_delay_request::Dialog_delay_request(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_delay_request)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_delay_request::~Dialog_delay_request()
{
    delete ui;
}

uint8_t Dialog_delay_request::getDelayMinutes()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return delay_minutes;
}

int Dialog_delay_request::getDelayType()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return delayType;
}

bool Dialog_delay_request::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

void Dialog_delay_request::setDelayMinutes(uint8_t _delay_minutes)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    delay_minutes=_delay_minutes;
}

void Dialog_delay_request::setDelayType(int _delayType)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    delayType=_delayType;
}

void Dialog_delay_request::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_delay_request::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    setDelayType(ui->comboBox->currentIndex()+5);
    setDelayMinutes((ui->comboBox_2->currentIndex()+1)*5);
    close();
}

void Dialog_delay_request::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    close();
}
