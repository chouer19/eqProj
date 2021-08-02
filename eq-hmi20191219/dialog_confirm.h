#ifndef DIALOG_CONFIRM_H
#define DIALOG_CONFIRM_H

#include <QDialog>
#include<QDesktopWidget>
#include<mutex>
#include<string>

using namespace std;

namespace Ui {
class Dialog_ready_confirm;
}

class Dialog_confirm : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_confirm(QWidget *parent = 0);
    ~Dialog_confirm();

    void setConfirmInfo(QString _confirmInfo);
    bool confirmed();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_ready_confirm *ui;

    std::mutex m_mutex;

    QString confirmInfo="";
    QString getConfirmInfo();

    bool confirm=false;
    void setConfirm(bool _confirm);

};

#endif // DIALOG_CONFIRM_H
