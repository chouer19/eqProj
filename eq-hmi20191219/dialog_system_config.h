#ifndef DIALOG_SYSTEM_CONFIG_H
#define DIALOG_SYSTEM_CONFIG_H

#include <QDialog>
#include<QDesktopWidget>
#include<mutex>
#include<string>
#include"header/system/sys_interface.hpp"

using namespace std;



namespace Ui {
class Dialog_system_config;
}

class Dialog_system_config : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_system_config(QWidget *parent = 0);
    ~Dialog_system_config();
    TERMINAL_CONFIG getTerminalConfig();
    void setTerminalConfig(TERMINAL_CONFIG _terminalConfig);


public slots:
    void showConfig();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_lineEdit_terminal_num_textChanged(const QString &arg1);

    void on_lineEdit_simcard_num_textChanged(const QString &arg1);

    void on_lineEdit_device_num_textChanged(const QString &arg1);

private:
    Ui::Dialog_system_config *ui;
    std::mutex m_mutex;
    //
    TERMINAL_CONFIG terminalConfig;

    //
    const uint8_t codeLength=8;
    const uint8_t terminalNumLength=5;
    const uint8_t simCardNumLength=11;
    const uint8_t ssidCardNumLength=20;
    const uint8_t deviceNumLength=17;
    bool isNum(QString _string);


    bool isConfigValid();

    uint8_t configvalid=0b111;
    void setConfigValid(uint8_t _configvalid);
    uint8_t getConfigValid();
};

#endif // DIALOG_SYSTEM_CONFIG_H
