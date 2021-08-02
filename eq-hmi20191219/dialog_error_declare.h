#ifndef DIALOG_ERROR_DECLARE_H
#define DIALOG_ERROR_DECLARE_H

#include <QDialog>
#include<QDesktopWidget>
#include<mutex>
#include<string>

using namespace std;

namespace Ui {
class Dialog_error_declare;
}

class Dialog_error_declare : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_error_declare(QWidget *parent = 0);
    ~Dialog_error_declare();

    int getErrorType();
    string getErrorDescription();
    bool confirmed();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_error_declare *ui;

    std::mutex m_mutex;

    int error_type=0;
    void setErrorType(int _error_type);

    string error_description="";
    void setErrorDescription(string _error_description);

    bool confirm=false;
    void setConfirm(bool _confirm);

};

#endif // DIALOG_ERROR_DECLARE_H
