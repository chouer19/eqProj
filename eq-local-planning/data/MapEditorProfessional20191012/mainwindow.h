#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"commonUsed/hserialport.h"
#include"commonUsed/tdinsdecode.h"
#include"commonUsed/qdisplaywidget1.h"
#include"commonUsed/qdisplaywidget2.h"
#include"commonUsed/database.h"
#include<QFileDialog>
#include<QMutex>
#include<QTimer>
#include"ins_novatel_pp7d_decode.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMapMainData(QList<MAP_POINT> valueList);
    QList<MAP_POINT> getMapMainData();

    void paintEvent(QPaintEvent *event);

private slots:
    void on_action_Open_triggered();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void on_pushButtonSetType_clicked();

    void on_pushButtondelete_clicked();

    void on_pushButtonInterpolation_clicked();

    void on_action_Save_as_triggered();

    void tranforSerialData();

    void tranforInsData();


    void on_actionOpen_File_triggered();

    void on_pushButton_insert_point_clicked();

    void on_pushButton_up_clicked();

    void on_checkBox_enable_move_clicked(bool checked);

    void on_pushButton_down_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_Bspline_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_setmap_clicked();

    void on_checkBox_showtext_clicked(bool checked);

    void on_pushButton_calcHeading_clicked();

    void on_checkBox_heading_clicked(bool checked);

    void on_checkBox_showSpeedLimit_clicked(bool checked);

    void on_checkBox_showIndex_clicked(bool checked);

    void on_checkBox_showZ_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    database myDatabase;
    QList<MAP_POINT> mapMainData;

    QList<MAP_POINT> getBSplineData();
    void setBSplineData(QList<MAP_POINT> data);
    QList<MAP_POINT> bSplineData;

    QList<MAP_POINT> calcBSpline(QList<MAP_POINT> srcdata);
    double calcHeading(POINT3D p0,POINT3D p1);
    QList<MAP_POINT> filtPoint(QList<MAP_POINT> srcdata,double interval);
    QList<MAP_POINT> addCtrlPoint(QList<MAP_POINT> srcdata);

    //
    QList<MAP_POINT> mapPointInterpolation(QList<MAP_POINT> origiData,int start,int end);
    POINT3D Coordinate_TransfertoWorld(POINT3D srcPoint, MAP_POINT insData);
    //
    QMutex mutex;
    bool isMouseDown=false;
    int originalfx;
    int originalfy;
    int originalx;
    int originaly;

     QTimer *timerPaint;

     int indexStart=0;
     int indexEnd=0;
     int type=0;

     hSerialport mySerialport;
     tDInsDecode myIns;

     INS_OPVT2AHR_DATA pos;

     vector<inspvax_message> msgVector;
};

#endif // MAINWINDOW_H
