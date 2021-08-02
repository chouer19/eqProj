#include "dialog_confirm_load.h"
#include "ui_dialog_confirm_load.h"

Dialog_confirm_load::Dialog_confirm_load(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_confirm_load)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_confirm_load::~Dialog_confirm_load()
{
    delete ui;
}

bool Dialog_confirm_load::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

bool Dialog_confirm_load::hard()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return isHard;
}

void Dialog_confirm_load::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_confirm_load::setHard(bool _isHard)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    isHard=_isHard;
}

void Dialog_confirm_load::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    close();
    ui->pushButton_confirm->setEnabled(false);
}

void Dialog_confirm_load::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    close();
    ui->pushButton_confirm->setEnabled(false);
}

void Dialog_confirm_load::on_pushButton_small_earth_clicked(bool checked)
{
    setHard(false);
    ui->pushButton_confirm->setEnabled(true);
}

void Dialog_confirm_load::on_pushButton_hard_stone_clicked(bool checked)
{
    setHard(true);
    ui->pushButton_confirm->setEnabled(true);
}
