#include "dialog_drive_login.h"
#include "ui_dialog_drive_login.h"

Dialog_drive_login::Dialog_drive_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_drive_login)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);
}

Dialog_drive_login::~Dialog_drive_login()
{
    delete ui;
}

string Dialog_drive_login::getDriverId()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return driverId;
}

bool Dialog_drive_login::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

void Dialog_drive_login::on_pushButton_num1_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('1');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num2_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('2');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num3_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('3');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num4_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('4');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num5_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('5');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num6_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('6');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num7_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('7');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num8_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('8');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_num9_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('9');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_clear_clicked()
{
    setDriverIdTemp("");
    ui->label_driverId->setText(getDriverIdTemp());
}

void Dialog_drive_login::on_pushButton_num0_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.push_back('0');
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_backspace_clicked()
{
    QString _driverIdTemp=getDriverIdTemp();
    _driverIdTemp.remove(_driverIdTemp.size()-1,1);
    setDriverIdTemp(_driverIdTemp);
    ui->label_driverId->setText(_driverIdTemp);
}

void Dialog_drive_login::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    setDriverId(getDriverIdTemp().toStdString());
    setDriverIdTemp("");
    ui->label_driverId->setText("");
    close();
}

void Dialog_drive_login::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    setDriverIdTemp("");
    ui->label_driverId->setText("");
    close();

}



void Dialog_drive_login::setDriverIdTemp(QString _driverIdTemp)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    driverIdTemp=_driverIdTemp;
}

QString Dialog_drive_login::getDriverIdTemp()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return driverIdTemp;
}

void Dialog_drive_login::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_drive_login::setDriverId(string _driverId)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    driverId=_driverId;
}
