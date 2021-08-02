#ifndef DIALOG_CONFIRM_LOAD_H
#define DIALOG_CONFIRM_LOAD_H

#include <QDialog>
#include<QDesktopWidget>
#include<mutex>
#include<string>

namespace Ui {
class Dialog_confirm_load;
}

class Dialog_confirm_load : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_confirm_load(QWidget *parent = 0);
    ~Dialog_confirm_load();

    bool confirmed();
    bool hard();
private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_small_earth_clicked(bool checked);

    void on_pushButton_hard_stone_clicked(bool checked);

private:
    Ui::Dialog_confirm_load *ui;

    std::mutex m_mutex;

    bool confirm=false;
    void setConfirm(bool _confirm);

    bool isHard=false;
    void setHard(bool _isHard);
};

#endif // DIALOG_CONFIRM_LOAD_H
