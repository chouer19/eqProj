#include "dialog_choose_load_mode.h"
#include "ui_dialog_choose_load_mode.h"

Dialog_choose_load_mode::Dialog_choose_load_mode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_choose_load_mode)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_choose_load_mode::~Dialog_choose_load_mode()
{
    delete ui;
}

bool Dialog_choose_load_mode::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

uint8_t Dialog_choose_load_mode::getLoadMode()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return loadMode;
}

void Dialog_choose_load_mode::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    close();
    ui->pushButton_confirm->setEnabled(false);
}

void Dialog_choose_load_mode::on_pushButton_back_load_clicked()
{
    setLoadMode(1);
    ui->pushButton_confirm->setEnabled(true);
}

void Dialog_choose_load_mode::on_pushButton_side_load_clicked()
{
    setLoadMode(2);
    ui->pushButton_confirm->setEnabled(true);
}

void Dialog_choose_load_mode::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    close();
    ui->pushButton_confirm->setEnabled(false);
}

void Dialog_choose_load_mode::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_choose_load_mode::setLoadMode(uint8_t _loadMode)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    loadMode=_loadMode;
}
