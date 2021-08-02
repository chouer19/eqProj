#ifndef DIALOG_UNLOAD_PLACE_SCAN_H
#define DIALOG_UNLOAD_PLACE_SCAN_H

#include <QDialog>
#include<mutex>
#include<QDesktopWidget>
#include"header/system/pub_types.h"

namespace Ui {
class Dialog_unload_place_scan;
}

class Dialog_unload_place_scan : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_unload_place_scan(QWidget *parent = 0);
    ~Dialog_unload_place_scan();
    bool confirmed();
    FORKLIFT_SAMPLE_GIS_PACKET getForkliftSampleGisPacket();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_end_clicked();

    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_unload_place_scan *ui;

    std::mutex m_mutex;

    bool confirm=false;
    void setConfirm(bool _confirm);

    FORKLIFT_SAMPLE_GIS_PACKET forkliftSampleGisPacket;
    void setForkliftSampleGisPacket(FORKLIFT_SAMPLE_GIS_PACKET _forkliftSampleGisPacket);
};

#endif // DIALOG_UNLOAD_PLACE_SCAN_H
