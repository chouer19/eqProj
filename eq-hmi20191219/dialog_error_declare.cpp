#include "dialog_error_declare.h"
#include "ui_dialog_error_declare.h"

Dialog_error_declare::Dialog_error_declare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_error_declare)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_error_declare::~Dialog_error_declare()
{
    delete ui;
}

int Dialog_error_declare::getErrorType()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return error_type;
}

string Dialog_error_declare::getErrorDescription()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return error_description;
}

bool Dialog_error_declare::confirmed()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return confirm;
}

void Dialog_error_declare::on_pushButton_confirm_clicked()
{
    setConfirm(true);
    setErrorDescription(ui->textEdit->toPlainText().toStdString());
    setErrorType(ui->comboBox->currentIndex()+1);
    ui->textEdit->clear();
    close();
}

void Dialog_error_declare::on_pushButton_cancel_clicked()
{
    setConfirm(false);
    ui->textEdit->clear();
    close();
}

void Dialog_error_declare::setErrorType(int _error_type)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    error_type=_error_type;
}

void Dialog_error_declare::setErrorDescription(string _error_description)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    error_description=_error_description;
}

void Dialog_error_declare::setConfirm(bool _confirm)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    confirm=_confirm;
}
