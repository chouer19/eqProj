#include "dialog_confirm.h"
#include "ui_dialog_confirm.h"

Dialog_confirm::Dialog_confirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ready_confirm)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_confirm::~Dialog_confirm()
{
    delete ui;
}

void Dialog_confirm::setConfirmInfo(QString _confirmInfo)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirmInfo=_confirmInfo;
    ui->label->setText(confirmInfo);
}

bool Dialog_confirm::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

QString Dialog_confirm::getConfirmInfo()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirmInfo;
}

void Dialog_confirm::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}

void Dialog_confirm::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    close();
}

void Dialog_confirm::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    close();
}
