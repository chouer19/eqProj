#ifndef TDINSDECODE_H
#define TDINSDECODE_H

#include <QObject>
#include<QThread>
#include<QMutex>
#include"pubfunc.h"
#include "geographicLib_header/Geocentric.hpp"
#include "geographicLib_header/LocalCartesian.hpp"
using namespace GeographicLib;

class tDInsDecode:public QThread
{
    Q_OBJECT
public:
     //

    //
    tDInsDecode();
    void stopThread();
    void inputData(QList<uint8_t> dataIn);
    void inputByte(QByteArray bytesIn);
    QList<INS_OPVT2AHR_DATA> outputData();

protected slots:
    void run();


signals:
    void decodeFinish();

private:

    double lat0=39.8825;
    double lon0=110.257;
    double alt0=1376.49;

    volatile bool isThreadStop;

    const float g=9.8;

    QList<INS_OPVT2AHR_DATA> decodeData;
    QList<uint8_t> origiDataBuff;
    QByteArray origiByteBuff;
    uint8_t *Buffer;
    int dataCount;
    QMutex mutex;

    bool Ins2AHRCheckSum(byte data[], int index);
    void Ins2AHRDecode(byte data[], int index);

};

#endif // TDINSDECODE_H
