#include "hserialport.h"
#include<QDebug>
#include<unistd.h>

hSerialport::hSerialport(QObject *parent) : QObject(parent)
{
    recvBuff.clear();
    sendBuff.clear();
//    sendBuff="LOG INSPVAXA ONTIME 0.05";
    InitCom();
}

hSerialport::~hSerialport()
{

}

QByteArray hSerialport::getRecvData()
{
    QByteArray temp;
    mutex.lock();
    temp=recvBuff;
    recvBuff.clear();
    mutex.unlock();
    return temp;
}

void hSerialport::setSendData(QByteArray sendData)
{
    mutex.lock();
    sendBuff.append(sendData);
    mutex.unlock();
    emit time2Send();
}

void hSerialport::InitCom()
{
    if(getComInfo())
    {
        openCom();
//        send();
    }
}

void hSerialport::closeCom()
{
    if(m_reader.isOpen())
        m_reader.close();
}

bool hSerialport::openCom()
{
    if(!m_reader.isOpen())
    {

       if(!m_reader.open(QIODevice::ReadWrite))
           m_reader.open(QIODevice::ReadWrite);



        if(m_reader.isOpen())
        {
            m_reader.setBaudRate(config.baudRate);
            m_reader.setDataBits(config.dataBits);
            m_reader.setParity(config.parity);
            m_reader.setFlowControl(config.flowControl);
            m_reader.setStopBits(config.stopBits);
            m_reader.clearError();
            m_reader.clear();
            connect(&m_reader,SIGNAL(readyRead()),this,SLOT(recv()));
            connect(this,SIGNAL(time2Send()),this,SLOT(send()));
            qDebug()<<"Serialport opened!";
            return true;
        }
        else
            return false;
    }
    else
    {
        return false;
    }
}

void hSerialport::recv()
{

    qint64 ticks1=lastTick;
    qint64 ticks2=QDateTime::currentMSecsSinceEpoch();
    double timeUsed=ticks2-ticks1;
    lastTick=ticks2;

    qDebug()<<"trans: "<<timeUsed;

    QByteArray temp=m_reader.readAll();
//    qDebug()<<temp;
    mutex.lock();
    recvBuff.append(temp);
    mutex.unlock();

    if(recvBuff.size()>100)
        emit recvFinish();
}

void hSerialport::send()
{
    mutex.lock();
    m_reader.write(sendBuff);
    sendBuff.clear();
    mutex.unlock();
}

SERIALPORT_COM_PARA hSerialport::getConfig()
{
    return config;
}

void hSerialport::setConfig(SERIALPORT_COM_PARA para)
{
    config=para;
}

bool hSerialport::getComInfo()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
        qDebug() << "System Location: " << info.systemLocation();
        if( info.portName() == "ttyUSB0" )
        {
            com_info = info;
            break;
        }
    }
    if(!com_info.isNull())
    {
        m_reader.setPort(com_info);
        return true;
    }
    else
        return false;
}
