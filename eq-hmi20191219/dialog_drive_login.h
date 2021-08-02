#ifndef DIALOG_DRIVE_LOGIN_H
#define DIALOG_DRIVE_LOGIN_H

#include <QDialog>
#include<string>
#include<QDesktopWidget>
#include<mutex>

using namespace std;

namespace Ui {
class Dialog_drive_login;
}

class Dialog_drive_login : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_drive_login(QWidget *parent = 0);
    ~Dialog_drive_login();

    string getDriverId();
    bool confirmed();

private slots:
    void on_pushButton_num1_clicked();

    void on_pushButton_num2_clicked();

    void on_pushButton_num3_clicked();

    void on_pushButton_num4_clicked();

    void on_pushButton_num5_clicked();

    void on_pushButton_num6_clicked();

    void on_pushButton_num7_clicked();

    void on_pushButton_num8_clicked();

    void on_pushButton_num9_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_num0_clicked();

    void on_pushButton_backspace_clicked();

    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();




private:
    Ui::Dialog_drive_login *ui;

    std::mutex m_mutex;

    //
    string driverId="";
    void setDriverId(string _driverId);

    QString driverIdTemp="";
    void setDriverIdTemp(QString _driverIdTemp);
    QString getDriverIdTemp();

    bool confirm=false;
    void setConfirm(bool _confirm);
};

#endif // DIALOG_DRIVE_LOGIN_H
