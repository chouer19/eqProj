#ifndef DIALOG_DELAY_REQUEST_H
#define DIALOG_DELAY_REQUEST_H

#include <QDialog>
#include<string>
#include<QDesktopWidget>
#include<mutex>

using namespace std;

namespace Ui {
class Dialog_delay_request;
}

class Dialog_delay_request : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_delay_request(QWidget *parent = 0);
    ~Dialog_delay_request();

    uint8_t getDelayMinutes();
    int getDelayType();
    bool confirmed();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_delay_request *ui;

    std::mutex m_mutex;

    uint8_t delay_minutes;
    void setDelayMinutes(uint8_t _delay_minutes);

    int delayType=0;
    void setDelayType(int _delayType);

    bool confirm=false;
    void setConfirm(bool _confirm);
};

#endif // DIALOG_DELAY_REQUEST_H
