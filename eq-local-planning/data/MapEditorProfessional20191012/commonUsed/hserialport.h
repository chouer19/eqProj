#ifndef HSERIALPORT_H
#define HSERIALPORT_H

#include <QObject>
#include<QDateTime>
#include<QList>
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QMutex>


struct SERIAL_DATA_WITH_Time
{
    qint64 timeticks;
    QByteArray serialData;

    SERIAL_DATA_WITH_Time()
    {
        timeticks=0;
        serialData.clear();
    }
};

struct SERIALPORT_COM_PARA
{
    qint64 baudRate;
    QSerialPort::Parity parity;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;

    SERIALPORT_COM_PARA()
    {
        baudRate=460800;
        parity=QSerialPort::NoParity;
        dataBits =QSerialPort::Data8;
        stopBits=QSerialPort::OneStop;
        flowControl=QSerialPort::NoFlowControl;
    }


};

class hSerialport : public QObject
{
    Q_OBJECT
public:
    explicit hSerialport(QObject *parent = nullptr);
    ~hSerialport();

    bool openCom();
    void closeCom();

private:
    QByteArray recvBuff;
    QByteArray sendBuff;
    SERIALPORT_COM_PARA config;

    //inner field and func
    QMutex mutex;
    QSerialPort m_reader;
    QSerialPortInfo com_info;

    bool getComInfo();
    void InitCom();





    //thread
    qint64 lastTick;



signals:
    void recvFinish();

    void time2Send();


public slots:
    void recv();
    void send();

    QByteArray getRecvData();
    void setSendData(QByteArray sendData);
    SERIALPORT_COM_PARA getConfig();
    void setConfig(SERIALPORT_COM_PARA para);


};

#endif // HSERIALPORT_H
