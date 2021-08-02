#include "mainwindow.h"
#include "ui_mainwindow.h"

const double lon_offset=0;
const double lat_offset=0;
const double alt_offset=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerPaint =new QTimer();
    connect(timerPaint,SIGNAL(timeout()),this,SLOT(update()));
    timerPaint->start(100);
//    connect(&mySerialport,SIGNAL(recvFinish()),this,SLOT(tranforSerialData()));
//    connect(&myIns,SIGNAL(decodeFinish()),this,SLOT(tranforInsData()));
    connect(timerPaint,SIGNAL(timeout()),this,SLOT(tranforSerialData()));
    connect(timerPaint,SIGNAL(timeout()),this,SLOT(tranforInsData()));
    myIns.start();
}

MainWindow::~MainWindow()
{
    if(myIns.isRunning())
        myIns.stopThread();

    while(myIns.isRunning());

    timerPaint->stop();
    delete timerPaint;
    delete ui;
}

void MainWindow::setMapMainData(QList<MAP_POINT> valueList)
{
    mutex.lock();
    mapMainData=valueList;
    mutex.unlock();
}

QList<MAP_POINT> MainWindow::getMapMainData()
{
    QList<MAP_POINT> tempList;
    mutex.lock();
    tempList=mapMainData;
    mutex.unlock();
    return tempList;
}

void MainWindow::on_action_Open_triggered()
{
    QString openFileName=QFileDialog::getOpenFileName(this,tr("open map"),"\.","*map");
    if(openFileName!="")
    {
        myDatabase.creatConnection(openFileName);

        QList<MAP_POINT> map=myDatabase.getData();
        myDatabase.close();
        if(myDatabase.isReadComlete)
        {
            for(int i=0;i<map.size();i++)
            {
                map[i].point=Coordinate_TransfertoWorld(POINT3D(lon_offset,lat_offset,alt_offset),map[i]);
            }

            setMapMainData(map);
        }

    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(ui->mapDisplay1->isMouseInWidget)
        ui->mapDisplay1->mouseDown();
    else
    {
        isMouseDown=true;
        originalfx=this->x();
        originalfy=this->y();
        originalx=cursor().pos().x();
        originaly=cursor().pos().y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

    if(ui->mapDisplay1->isMouseInWidget)
        ui->mapDisplay1->mouseUp();
    else
        isMouseDown=false;

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(ui->mapDisplay1->isMouseInWidget)
        ui->mapDisplay1->mouseMove();
    else
    {
        if(isMouseDown)
        {
            move(originalfx+cursor().pos().x()-originalx,originalfy+cursor().pos().y()-originaly);
        }
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(ui->mapDisplay1->isMouseInWidget)
        ui->mapDisplay1->mouseDoubleClick();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(ui->mapDisplay1->isMouseInWidget)
        ui->mapDisplay1->wheelRoll(event);
}

QList<MAP_POINT> MainWindow::mapPointInterpolation(QList<MAP_POINT> origiData,int start,int end)
{
    QList<MAP_POINT> result;
    if(origiData.size()<=1)
        result=origiData;
    else
    {
        for(int i=0;i<start;i++)
        {
            result.push_back(origiData[i]);
        }

        for(int i=start;i<min(end,origiData.size()-1);i++)
        {
            double dist=get2Dis(origiData[i].point,origiData[i+1].point);
            if(dist>=0.2)
            {
                int count=(int)(dist/0.1);
                for(int j=0;j<count;j++)
                {
                    MAP_POINT p;
                    double x=(double)j/count*(origiData[i+1].point.y-origiData[i].point.y);
                    p.point.x=origiData[i].point.x+(double)j/count*(origiData[i+1].point.x-origiData[i].point.x);
                    p.point.y=origiData[i].point.y+(double)j/count*(origiData[i+1].point.y-origiData[i].point.y);
                    p.point.z=origiData[i].point.z+(double)j/count*(origiData[i+1].point.z-origiData[i].point.z);
                    p.heading=origiData[i].heading+(double)j/count*(origiData[i+1].heading-origiData[i].heading);
                    p.heading=(p.heading<0)?(p.heading+360):p.heading;
                    p.roadIndex=origiData[i].roadIndex;
                    p.type=origiData[i].type;

                    result.append(p);
                }
            }
            else
                result.append(origiData[i]);
        }



        for(int i=end;i<origiData.size();i++)
        {
            result.push_back(origiData[i]);
        }



    }

    return result;


}

POINT3D MainWindow::Coordinate_TransfertoWorld(POINT3D srcPoint, MAP_POINT insData)
{
    POINT3D output_data;
    //
    double x=srcPoint.x;
    double y=srcPoint.y;
    double z=srcPoint.z;

    //绕y轴旋转
    double x2,y2,z2;
    double roll_com=-(insData.roll)*M_PI/180.0;
    z2=z*cos(roll_com)-x*sin(roll_com);
    x2=z*sin(roll_com)+x*cos(roll_com);
    y2=y;

    //绕x轴旋转
    double x1,y1,z1;
    double pitch_com=(insData.pitch)*M_PI/180.0;
    y1 = y2*cos(pitch_com)-z2*sin(pitch_com);
    z1 = y2*sin(pitch_com)+z2*cos(pitch_com);
    x1 = x2;

    //绕z轴旋转
    double x3,y3,z3;
    double yaw_com=-(insData.heading)*M_PI/180.0;
    x3=x1*cos(yaw_com)-y1*sin(yaw_com);
    y3=x1*sin(yaw_com)+y1*cos(yaw_com);
    z3=z1;

    output_data.x =-y3 +insData.point.x;//x no change
    output_data.y =x3 +insData.point.y;
    output_data.z =z3 +insData.point.z;

    return output_data;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QList<MAP_POINT> map=getMapMainData();
    QList<MAP_POINT> _bspline=getBSplineData();

    mutex.lock();
    INS_OPVT2AHR_DATA loc=pos;
    mutex.unlock();

    ui->label_heading->setText(QString::number(loc.heading));
    if(map.size()>0)
    {
        //display
        MAP_POINT basePoint=map[0];
        QList<MAP_POINT> data;
        for(int i=0;i<map.size();i++)
        {
            MAP_POINT tempPoint=map[i];
            tempPoint.point.x-=basePoint.point.x;
            tempPoint.point.y-=basePoint.point.y;
            data.append(tempPoint);
        }
        ui->mapDisplay1->setMapData(data);

        pos.x-=basePoint.point.x;
        pos.y-=basePoint.point.y;

        ui->mapDisplay1->setLocateData(pos);

        //set index
        indexStart=ui->spinBoxStart->value();
        indexEnd=ui->spinBoxEnd->value();
        ui->spinBoxStart->setMaximum(indexEnd);
        ui->spinBoxEnd->setMaximum(map.size()-1);
        ui->spinBoxEnd->setMinimum(indexStart);
        ui->mapDisplay1->indexStart=indexStart;
        ui->mapDisplay1->indexEnd=indexEnd;

        //
        type=ui->spinBoxType->value();
        ui->labelPointCount->setText("count: "+QString::number(map.size()));

        double totalLength=0;
//        for(int i=indexStart;i<indexEnd;i++)
//        {
//            totalLength+=get3Dis(map[i].point,map[i+1].point);
//        }
        ui->label_sum->setText("sum: "+QString::number(totalLength));

    }

    if(_bspline.size()>0)
    {
        MAP_POINT basePoint=_bspline[0];
        QList<MAP_POINT> data;
        for(int i=0;i<_bspline.size();i++)
        {
            MAP_POINT tempPoint=_bspline[i];
            tempPoint.point.x-=basePoint.point.x;
            tempPoint.point.y-=basePoint.point.y;
            data.append(tempPoint);
        }
         ui->mapDisplay1->setBsplineData(data);
    }
    else
        ui->mapDisplay1->setBsplineData(_bspline);




    if(msgVector.size()>0)
    {
        //display
        MAP_POINT originPoint;
        originPoint.point =POINT3D(msgVector[0].x,msgVector[0].y,msgVector[0].z);
        QList<MAP_POINT> data;
        for(int i=0;i<msgVector.size();i++)
        {
            MAP_POINT tempPoint;
            tempPoint.point=POINT3D(msgVector[0].x,msgVector[0].y,msgVector[0].z);
            tempPoint.point.x-=originPoint.point.x;
            tempPoint.point.y-=originPoint.point.y;
            tempPoint.point.z-=originPoint.point.z;
            tempPoint.type=TRACK_POINT_TYPE::speedLevel1;
            data.append(tempPoint);
        }
        ui->mapDisplay1->setMapData(data);
    }


}

void MainWindow::on_pushButtonSetType_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    for(int i=indexStart;i<=indexEnd;i++)
    {
        mapdata[i].type=(TRACK_POINT_TYPE)type;
    }
    setMapMainData(mapdata);
}

void MainWindow::on_pushButtondelete_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_pushButtonInterpolation_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    mapdata=mapPointInterpolation(mapdata,indexStart,indexEnd);
    setMapMainData(mapdata);
}

void MainWindow::on_action_Save_as_triggered()
{
    QList<MAP_POINT> mapSave=getMapMainData();
    QString saveFileName=QFileDialog::getSaveFileName(this,tr("保存地图"),"\.","*map")+".map";
    if(saveFileName!="")
    {
        myDatabase.creatSaveConnection(saveFileName);

        if(mapSave.size()>0)
        {
//            mapSave=mapPointInterpolation(mapSave);

//            int num=min(mapSave.size(),80);
//            for(int i=0;i<num;i++)
//            {
//                int j=mapSave.size()-1-i;
//                mapSave[j].type=TRACK_POINT_TYPE::stop;
//            }

        }
        myDatabase.addData(mapSave);


    }
}

void MainWindow::tranforSerialData()
{
    myIns.inputByte(mySerialport.getRecvData());
}

void MainWindow::tranforInsData()
{
    QList<INS_OPVT2AHR_DATA> temp=myIns.outputData();
    if(temp.size()>0)
    {
        mutex.lock();
        pos=temp.last();
        mutex.unlock();
    }

}

void MainWindow::on_actionOpen_File_triggered()
{
    QString openFileName=QFileDialog::getOpenFileName(this,tr("open txt"),"\.","*dat");
    if(openFileName!="")
    {
        QFile file(openFileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        ins_novatel_pp7d_decode decode;

        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            msgVector.push_back(decode.msgDecode(line));

        }

    }

}

void MainWindow::on_pushButton_insert_point_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart;i<=indexEnd;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_pushButton_up_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart;i<=indexEnd;i++)
    {
        mapdata[i].point.y+=ui->doubleSpinBox_move_step->value();
        temp.append(mapdata[i]);
    }
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_checkBox_enable_move_clicked(bool checked)
{
    ui->doubleSpinBox_move_step->setEnabled(checked);
    ui->pushButton_up->setEnabled(checked);
    ui->pushButton_down->setEnabled(checked);
    ui->pushButton_left->setEnabled(checked);
    ui->pushButton_right->setEnabled(checked);
}

void MainWindow::on_pushButton_down_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart;i<=indexEnd;i++)
    {
        mapdata[i].point.y-=ui->doubleSpinBox_move_step->value();
        temp.append(mapdata[i]);
    }
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_pushButton_left_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart;i<=indexEnd;i++)
    {
        mapdata[i].point.x-=ui->doubleSpinBox_move_step->value();
        temp.append(mapdata[i]);
    }
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_pushButton_right_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    QList<MAP_POINT> temp;
    for(int i=0;i<indexStart;i++)
    {
        temp.append(mapdata[i]);
    }
    for(int i=indexStart;i<=indexEnd;i++)
    {
        mapdata[i].point.x+=ui->doubleSpinBox_move_step->value();
        temp.append(mapdata[i]);
    }
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        temp.append(mapdata[i]);
    }
    setMapMainData(temp);
}

void MainWindow::on_pushButton_Bspline_clicked()
{
    QList<MAP_POINT> mapdata=getMapMainData();
    double interval=ui->doubleSpinBox_bSplineInterval->value();

    QList<MAP_POINT> _bsplinedata;
    for(int i=0;i<indexStart;i++)
    {
        _bsplinedata.append(mapdata[i]);
    }
    QList<MAP_POINT> temp;
    for(int i=indexStart;i<=indexEnd;i++)
    {
        temp.append(mapdata[i]);
    }
    temp=filtPoint(temp,interval);
    temp=addCtrlPoint(temp);
    _bsplinedata.append(calcBSpline(temp));


//    _bsplinedata.append(temp);
    for(int i=indexEnd+1;i<mapdata.size();i++)
    {
        _bsplinedata.append(mapdata[i]);
    }
    setBSplineData(_bsplinedata);
}

QList<MAP_POINT> MainWindow::getBSplineData()
{
    QList<MAP_POINT> temp;
    mutex.lock();
    temp=bSplineData;
    mutex.unlock();
    return temp;
}

void MainWindow::setBSplineData(QList<MAP_POINT> data)
{
    mutex.lock();
    bSplineData=data;
    mutex.unlock();
}

QList<MAP_POINT> MainWindow::calcBSpline(QList<MAP_POINT> srcdata)
{
    QList<MAP_POINT> dstdata;
    if(srcdata.size()>=4)
    {
        dstdata.push_back(srcdata[1]);
        for (int n = 0; n < srcdata.size(); n+=1)
        {
            if (n <= srcdata.size() - 4)
            {
                for (double t = 0.0; t <= 1.0; t += 0.01)
                {
                    double a1 =  pow((1 - t), 3) / 6;
                    double a2 =  (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6;
                    double a3 = (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6;
                    double a4 =  pow(t, 3) / 6;

                    POINT3D P0=srcdata[n].point;
                    POINT3D P1=srcdata[n+1].point;
                    POINT3D P2=srcdata[n+2].point;
                    POINT3D P3=srcdata[n+3].point;

                    double x = a1*P0.x + a2*P1.x + a3*P2.x + a4*P3.x;
                    double y = a1*P0.y + a2*P1.y + a3*P2.y + a4*P3.y;
                    double z = a1*P0.z + a2*P1.z + a3*P2.z + a4*P3.z;

                    MAP_POINT p;
                    p.point=POINT3D(x,y,z);
                    p.type=none;
                    p.heading=0;
                    p.curvature=0;
                    dstdata.push_back(p);
                }
            }
        }
        dstdata.push_back(srcdata[srcdata.size()-2]);


        dstdata=filtPoint(dstdata,0.01);

    }
    return dstdata;
}

double MainWindow::calcHeading(POINT3D p0, POINT3D p1)
{
    if(p0.y==p1.y)
    {
        if(p0.x>p1.x)
            return 270;
        else
            return 90;
    }
    else
    {
        double heading= atan((p1.x-p0.x)/(p1.y-p0.y))*180/PI;

        if(p1.y<p0.y)
            heading+=180;

        while(heading>=360)
            heading-=360;

        while(heading<0)
            heading+=360;
        return heading;

    }
}

QList<MAP_POINT> MainWindow::filtPoint(QList<MAP_POINT> srcdata, double interval)
{
    QList<MAP_POINT> _dst;
    if(srcdata.size()>0)
    {
        _dst.push_back(srcdata.front());
        for(int i=1;i<srcdata.size()-1;i++)
        {
            if(get2Dis(srcdata[i].point,_dst.back().point)>=interval)
                _dst.push_back(srcdata[i]);
        }
        _dst.push_back(srcdata.back());
    }
    return _dst;
}

QList<MAP_POINT> MainWindow::addCtrlPoint(QList<MAP_POINT> srcdata)
{
    QList<MAP_POINT> dstdata;
    if(srcdata.size()>=4)
    {
        MAP_POINT ps;
        MAP_POINT p0=srcdata[0];
        MAP_POINT p1=srcdata[1];
        MAP_POINT pn_2=srcdata[srcdata.size()-2];
        MAP_POINT pn_1=srcdata[srcdata.size()-1];
        MAP_POINT pe;

        ps.point.x=2*p0.point.x-p1.point.x;
        ps.point.y=2*p0.point.y-p1.point.y;
        ps.point.z=2*p0.point.z-p1.point.z;

        pe.point.x=2*pn_1.point.x-pn_2.point.x;
        pe.point.y=2*pn_1.point.y-pn_2.point.y;
        pe.point.z=2*pn_1.point.z-pn_2.point.z;

        dstdata.append(ps);
        dstdata.append(srcdata);
        dstdata.append(pe);

        return dstdata;
    }
    else
        return srcdata;
}

void MainWindow::on_pushButton_clear_clicked()
{
    QList<MAP_POINT> temp;
    temp.clear();
    setBSplineData(temp);
}

void MainWindow::on_pushButton_setmap_clicked()
{
    QList<MAP_POINT> temp=getBSplineData();
    temp=filtPoint(temp,0.1);
    setMapMainData(temp);
    temp.clear();
    setBSplineData(temp);
}

void MainWindow::on_checkBox_showtext_clicked(bool checked)
{
    ui->mapDisplay1->setShowType(checked);
}

void MainWindow::on_pushButton_calcHeading_clicked()
{
    if(indexStart!=indexEnd)
    {
        QList<MAP_POINT> mapdata=getMapMainData();
        QList<MAP_POINT> temp;
        for(int i=0;i<indexStart;i++)
        {
            temp.append(mapdata[i]);
        }
        for(int i=indexStart;i<=indexEnd;i++)
        {
            if(i<indexEnd)
                mapdata[i].heading=calcHeading(mapdata[i].point,mapdata[i+1].point);
            else
                mapdata[i].heading=mapdata[i-1].heading;

            if(ui->checkBox_rearmove->isChecked())
            {
                mapdata[i].heading+=180;
                while(mapdata[i].heading>=360)
                    mapdata[i].heading-=360;

                while(mapdata[i].heading<0)
                    mapdata[i].heading+=360;
            }


            temp.append(mapdata[i]);
        }
        for(int i=indexEnd+1;i<mapdata.size();i++)
        {
            temp.append(mapdata[i]);
        }
        setMapMainData(temp);
    }

    if(indexEnd>=indexStart+2)
    {
        QList<MAP_POINT> mapdata=getMapMainData();
        QList<MAP_POINT> temp;
        for(int i=0;i<indexStart;i++)
        {
            temp.append(mapdata[i]);
        }
        for(int i=indexStart+1;i<=indexEnd;i++)
        {
            POINT3D p0;
            POINT3D p1=mapdata[i].point;
            POINT3D p2;

            for(int j=i;j>=indexStart+1;j--)
            {
                if(get2Dis(mapdata[j].point,p1)>=4.35)
                {
                    p0=mapdata[j].point;
                    break;
                }

                if(j==indexStart+1)
                    p0=mapdata[j].point;
            }

            for(int j=i;j<=indexEnd;j++)
            {
                if(get2Dis(mapdata[j].point,p1)>=4.35)
                {
                    p2=mapdata[j].point;
                    break;
                }

                if(j==indexEnd)
                    p2=mapdata[j].point;
            }

            if(i==indexEnd)
                mapdata[i].curvature= mapdata[i-1].curvature;
            else
                mapdata[i].curvature=calcCurvature(mapdata[i-1].point,mapdata[i].point,mapdata[i+1].point);

            temp.append(mapdata[i]);
        }
        for(int i=indexEnd+1;i<mapdata.size();i++)
        {
            temp.append(mapdata[i]);
        }
        setMapMainData(temp);
    }

}

void MainWindow::on_checkBox_heading_clicked(bool checked)
{
    ui->mapDisplay1->setShowHeading(checked);
}

void MainWindow::on_checkBox_showSpeedLimit_clicked(bool checked)
{
    ui->mapDisplay1->setShowSpeedLimit(checked);
}

void MainWindow::on_checkBox_showIndex_clicked(bool checked)
{
    ui->mapDisplay1->setShowIndex(checked);
}

void MainWindow::on_checkBox_showZ_clicked(bool checked)
{
    ui->mapDisplay1->setShowZ(checked);
}
