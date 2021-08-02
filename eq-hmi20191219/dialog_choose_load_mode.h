#ifndef DIALOG_CHOOSE_LOAD_MODE_H
#define DIALOG_CHOOSE_LOAD_MODE_H

#include <QDialog>
#include<QDesktopWidget>
#include<mutex>
#include<string>

namespace Ui {
class Dialog_choose_load_mode;
}

class Dialog_choose_load_mode : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_choose_load_mode(QWidget *parent = 0);
    ~Dialog_choose_load_mode();

    bool confirmed();
    uint8_t getLoadMode();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_back_load_clicked();

    void on_pushButton_side_load_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_choose_load_mode *ui;


    std::mutex m_mutex;

    bool confirm=false;
    void setConfirm(bool _confirm);

    uint8_t loadMode=0;
    void setLoadMode(uint8_t _loadMode);

};

#endif // DIALOG_CHOOSE_LOAD_MODE_H
