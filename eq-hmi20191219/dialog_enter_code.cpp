#include "dialog_enter_code.h"
#include "ui_dialog_enter_code.h"

Dialog_enter_code::Dialog_enter_code(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_enter_code)
{
    ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->availableGeometry();
    move (screenRect.width()/2-this->width()/2, screenRect.height()/2-this->height()/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Dialog_enter_code::~Dialog_enter_code()
{
    delete ui;
}

string Dialog_enter_code::getTerminalCode()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminal_code;
}

void Dialog_enter_code::on_pushButton_confirm_clicked()
{
    setTerminalCode(getTerminalCodeTemp().toStdString());
    setTerminalCodeTemp("");
    ui->label_code->setText("");
    close();
}

void Dialog_enter_code::on_pushButton_cancel_clicked()
{
    setTerminalCodeTemp("");
    ui->label_code->setText("");
    close();
}

void Dialog_enter_code::on_pushButton_num1_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('1');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num2_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('2');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num3_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('3');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num4_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('4');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num5_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('5');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num6_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('6');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num7_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('7');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num8_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('8');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_num9_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('9');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::setTerminalCode(string _terminal_code)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminal_code=_terminal_code;
}

void Dialog_enter_code::setTerminalCodeTemp(QString _terminal_code_temp)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    terminal_code_temp=_terminal_code_temp;
}

QString Dialog_enter_code::getTerminalCodeTemp()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return terminal_code_temp;
}

void Dialog_enter_code::on_pushButton_num0_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.push_back('0');
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}

void Dialog_enter_code::on_pushButton_clear_clicked()
{
    setTerminalCodeTemp("");
    ui->label_code->setText("");
}

void Dialog_enter_code::on_pushButton_backspace_clicked()
{
    QString _terminal_code_temp=getTerminalCodeTemp();
    _terminal_code_temp.remove(_terminal_code_temp.size()-1,1);
    setTerminalCodeTemp(_terminal_code_temp);
    ui->label_code->setText(_terminal_code_temp);
}
