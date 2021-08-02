#include "dialog_system_config.h"
#include "ui_dialog_system_config.h"

Dialog_system_config::Dialog_system_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_system_config)
{
    ui->setupUi(this);

    ui->label_invalid1->setVisible(false);
}

Dialog_system_config::~Dialog_system_config()
{
    delete ui;
}

TERMINAL_CONFIG Dialog_system_config::getTerminalConfig()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminalConfig;
}

void Dialog_system_config::setTerminalConfig(TERMINAL_CONFIG _terminalConfig)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminalConfig=_terminalConfig;
}

void Dialog_system_config::showConfig()
{
    ui->comboBox_terminal_type->setCurrentIndex(getTerminalConfig().terminal_type);
    ui->lineEdit_device_num->setText(QString::fromStdString(getTerminalConfig().binding_device_num));
    ui->lineEdit_simcard_num->setText(QString::fromStdString(getTerminalConfig().sim_card_num));
    ui->lineEdit_terminal_num->setText(QString::fromStdString(getTerminalConfig().terminal_num));
}

void Dialog_system_config::on_pushButton_confirm_clicked()
{
    if(isConfigValid())
    {
        TERMINAL_CONFIG _config=getTerminalConfig();
        _config.terminal_type=(TERMINAL_TYPE)(ui->comboBox_terminal_type->currentIndex());
        _config.sim_card_num=ui->lineEdit_simcard_num->text().toStdString();
        _config.binding_device_num=ui->lineEdit_device_num->text().toStdString();
        _config.terminal_num=ui->lineEdit_terminal_num->text().toStdString();
        setTerminalConfig(_config);
        close();
    }
}

void Dialog_system_config::on_pushButton_cancel_clicked()
{
    close();
}


void Dialog_system_config::on_lineEdit_terminal_num_textChanged(const QString &arg1)
{
    if(isNum(ui->lineEdit_terminal_num->text())&&ui->lineEdit_terminal_num->text().size()==terminalNumLength)
    {
        ui->label_invalid1->setVisible(false);
        setConfigValid(getConfigValid()|0x01);
    }
    else
    {
        ui->label_invalid1->setVisible(true);
        setConfigValid(getConfigValid()&0xfe);
    }
}

bool Dialog_system_config::isNum(QString _string)
{
    if(_string.size()==0)
        return false;

    for(int i=0;i<_string.size();i++)
    {
        if(_string[i]<'0'||_string[i]>'9')
            return false;
    }
    return true;

}

bool Dialog_system_config::isConfigValid()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return configvalid==0b111;
}

uint8_t Dialog_system_config::getConfigValid()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return configvalid;
}

void Dialog_system_config::setConfigValid(uint8_t _configvalid)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    configvalid=_configvalid;
}

void Dialog_system_config::on_lineEdit_simcard_num_textChanged(const QString &arg1)
{
    if(isNum(ui->lineEdit_simcard_num->text())&&(ui->lineEdit_simcard_num->text().size()==simCardNumLength||ui->lineEdit_simcard_num->text().size()==ssidCardNumLength))
    {
        ui->label_invalid2->setVisible(false);
        setConfigValid(getConfigValid()|0x02);
    }
    else
    {
        ui->label_invalid2->setVisible(true);
        setConfigValid(getConfigValid()&0xfd);
    }
}

void Dialog_system_config::on_lineEdit_device_num_textChanged(const QString &arg1)
{
    if(/*isNum(ui->lineEdit_device_num->text())&&*/ui->lineEdit_device_num->text().size()==deviceNumLength)
    {
        ui->label_invalid3->setVisible(false);
        setConfigValid(getConfigValid()|0x04);
    }
    else
    {
        ui->label_invalid3->setVisible(true);
        setConfigValid(getConfigValid()&0xfb);
        setConfigValid(false);
    }
}
