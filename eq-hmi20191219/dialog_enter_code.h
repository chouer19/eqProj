#ifndef DIALOG_ENTER_CODE_H
#define DIALOG_ENTER_CODE_H

#include <QDialog>
#include<string>
#include<QDesktopWidget>
#include<mutex>

using namespace std;

namespace Ui {
class Dialog_enter_code;
}

class Dialog_enter_code : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_enter_code(QWidget *parent = 0);
    ~Dialog_enter_code();

    string getTerminalCode();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_num1_clicked();

    void on_pushButton_num2_clicked();

    void on_pushButton_num3_clicked();

    void on_pushButton_num4_clicked();

    void on_pushButton_num9_clicked();

    void on_pushButton_num5_clicked();

    void on_pushButton_num6_clicked();

    void on_pushButton_num7_clicked();

    void on_pushButton_num8_clicked();

    void on_pushButton_num0_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_backspace_clicked();

private:
    Ui::Dialog_enter_code *ui;
    std::mutex m_mutex;

    //
    string terminal_code="";
    void setTerminalCode(string _terminal_code);

    QString terminal_code_temp="";
    void setTerminalCodeTemp(QString _terminal_code_temp);
    QString getTerminalCodeTemp();
};

#endif // DIALOG_ENTER_CODE_H
