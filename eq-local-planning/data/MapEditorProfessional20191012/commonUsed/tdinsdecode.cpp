#include "tdinsdecode.h"

#define lonoffset -4.17
#define latoffset -0.09
#define altoffset -1.39


tDInsDecode::tDInsDecode()
{
    decodeData.clear();
    Buffer= new uint8_t[4096];
    dataCount=0;
    origiDataBuff.clear();
    isThreadStop=false;
}

void tDInsDecode::stopThread()
{
    isThreadStop=true;
}

void tDInsDecode::inputData(QList<uint8_t> dataIn)
{
    mutex.lock();
    origiDataBuff.append(dataIn);
    mutex.unlock();
}

void tDInsDecode::inputByte(QByteArray bytesIn)
{
    mutex.lock();
    origiByteBuff.append(bytesIn);
    mutex.unlock();
}

QList<INS_OPVT2AHR_DATA> tDInsDecode::outputData()
{
    QList<INS_OPVT2AHR_DATA> temp;
    mutex.lock();
    temp=decodeData;
    decodeData.clear();
    mutex.unlock();
    return temp;

}

void tDInsDecode::run()
{
    while(!isThreadStop)
    {
        if(origiDataBuff.size()>0)
        {
            mutex.lock();
            int num=origiDataBuff.size()>3072?3072:origiDataBuff.size();

            for(int i=0;i<num;i++)
            {
                Buffer[dataCount++]=origiDataBuff[0];
                origiDataBuff.removeAt(0);
            }

            mutex.unlock();

            int i=0;

            while(dataCount>170)
            {
                if(Buffer[i]==0xAA&&Buffer[i+1]==0x55)
                {
                    if(Ins2AHRCheckSum(Buffer,i))
                    {
                        Ins2AHRDecode(Buffer,i);
                        i+=170;
                        dataCount-=170;
                    }
                    else
                    {
                        i++;
                        dataCount--;
                    }

                }
                else
                {
                    i++;
                    dataCount--;
                }
            }

            if(dataCount>0)
            {
                memcpy(Buffer,Buffer+i,sizeof(uint8_t)*dataCount);
            }

        }
        else if(origiByteBuff.size()>0)
        {
            mutex.lock();
            int num=origiByteBuff.size()>3072?3072:origiByteBuff.size();

            for(int i=0;i<num;i++)
            {
                Buffer[dataCount++]=origiByteBuff[0];
                origiByteBuff.remove(0,1);
            }

            mutex.unlock();

            int i=0;

            while(dataCount>170)
            {
                if(Buffer[i]==0xAA&&Buffer[i+1]==0x55)
                {
                    if(Ins2AHRCheckSum(Buffer,i))
                    {
                        Ins2AHRDecode(Buffer,i);
                        i+=170;
                        dataCount-=170;
                    }
                    else
                    {
                        i++;
                        dataCount--;
                    }

                }
                else
                {
                    i++;
                    dataCount--;
                }
            }

            if(dataCount>0)
            {
                memcpy(Buffer,Buffer+i,sizeof(uint8_t)*dataCount);
            }

        }

        msleep(1);
    }
    isThreadStop=false;
}

bool tDInsDecode::Ins2AHRCheckSum(byte data[], int index)
{
    int sum = 0;
    for (int i = 2; i <= 167;i++)
    {
        sum += data[index + i];
    }
    sum = (sum & 0xffff);

    int checksum = data[index + 168] + data[index + 169] * 256;
    if (sum == checksum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void tDInsDecode::Ins2AHRDecode(byte data[], int index)
{
    INS_OPVT2AHR_ORIGIDATA Ins2AHROrigi;

    memcpy(&Ins2AHROrigi.Heading, data + index + 30 + 0, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.Pitch, data + index + 30 + 2, sizeof(INT16));
    memcpy(&Ins2AHROrigi.Roll, data + index + 30 + 4, sizeof(INT16));

    memcpy(&Ins2AHROrigi.GyroX, data + index + 30 + 6, sizeof(int));
    memcpy(&Ins2AHROrigi.GyroY, data + index + 30 + 10, sizeof(int));
    memcpy(&Ins2AHROrigi.GyroZ, data + index + 30 + 14, sizeof(int));

    memcpy(&Ins2AHROrigi.AccX, data + index + 30 + 18, sizeof(int));
    memcpy(&Ins2AHROrigi.AccY, data + index + 30 + 22, sizeof(int));
    memcpy(&Ins2AHROrigi.AccZ, data + index + 30 + 26, sizeof(int));

    memcpy(&Ins2AHROrigi.MagX, data + index + 30 + 30, sizeof(INT16));
    memcpy(&Ins2AHROrigi.MagY, data + index + 30 + 32, sizeof(INT16));
    memcpy(&Ins2AHROrigi.MagZ, data + index + 30 + 34, sizeof(INT16));

    memcpy(&Ins2AHROrigi.USW, data + index + 30 + 36, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.Vinp, data + index + 30 + 38, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.Temper, data + index + 30 + 40, sizeof(INT16));

    memcpy(&Ins2AHROrigi.Latitude, data + index + 30 + 42, sizeof(long));
    memcpy(&Ins2AHROrigi.Longitude, data + index + 30 + 50, sizeof(long));
    memcpy(&Ins2AHROrigi.Altitude, data + index + 30 + 58, sizeof(int));

    memcpy(&Ins2AHROrigi.EastSpeed, data + index + 30 + 62, sizeof(int));
    memcpy(&Ins2AHROrigi.NorthSpeed, data + index + 30 + 66, sizeof(int));
    memcpy(&Ins2AHROrigi.VerticalSpeed, data + index + 30 + 70, sizeof(int));

    memcpy(&Ins2AHROrigi.LatitudeGNSS, data + index + 30 + 74, sizeof(long));
    memcpy(&Ins2AHROrigi.LongitudeGNSS, data + index + 30 + 82, sizeof(long));
    memcpy(&Ins2AHROrigi.AltitudeGNSS, data + index + 30 + 90, sizeof(int));

    memcpy(&Ins2AHROrigi.HorizontalSpeed, data + index + 30 + 94, sizeof(int));
    memcpy(&Ins2AHROrigi.TrackOverGround, data + index + 30 + 98, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.VerticalSpeed2, data + index + 30 + 100, sizeof(int));

    memcpy(&Ins2AHROrigi.ms_gps, data + index + 30 + 104, sizeof(UINT));

    memcpy(&Ins2AHROrigi.GNSS_info1, data + index + 30 + 108, sizeof(byte));
    memcpy(&Ins2AHROrigi.GNSS_info2, data + index + 30 + 109, sizeof(byte));

    memcpy(&Ins2AHROrigi.solnSVs,data + index + 30 + 110, sizeof(byte));

    memcpy(&Ins2AHROrigi.V_latency, data + index + 30 + 111, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.AnglesPositionType, data + index + 30 + 113, sizeof(byte));

    memcpy(&Ins2AHROrigi.HeadingGNSS, data + index + 30 + 114, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.PitchGNSS, data + index + 30 + 116, sizeof(INT16));

    memcpy(&Ins2AHROrigi._solution_status, data + index + 30 + 118, sizeof(UINT8));
    memcpy(&Ins2AHROrigi._pos_and_vel_type, data + index + 30 + 119, sizeof(UINT8));

    memcpy(&Ins2AHROrigi._gps_week, data + index + 30 + 120, sizeof(UINT16));


    memcpy(&Ins2AHROrigi.P_bar, data + index + 30 + 122, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.H_bar, data + index + 30 + 124, sizeof(int));

    memcpy(&Ins2AHROrigi.NewGPS, data + index + 30 + 128, sizeof(byte));



    INS_OPVT2AHR_DATA temp;
    temp.heading=Ins2AHROrigi.Heading*0.01;
    temp.pitch=-Ins2AHROrigi.Pitch*0.01;
    temp.roll=Ins2AHROrigi.Roll*0.01;

    temp.gyroX=Ins2AHROrigi.GyroX/100000.0;
    temp.gyroY=Ins2AHROrigi.GyroY/100000.0;
    temp.gyroZ=Ins2AHROrigi.GyroZ/100000.0;

    temp.accX=Ins2AHROrigi.AccX/g/1000000.0;
    temp.accY=Ins2AHROrigi.AccY/g/1000000.0;
    temp.accZ=Ins2AHROrigi.AccZ/g/1000000.0;

    temp.magX=Ins2AHROrigi.MagX*10;
    temp.magY=Ins2AHROrigi.MagY*10;
    temp.magZ=Ins2AHROrigi.MagZ*10;

    temp.usw=Ins2AHROrigi.USW;

    temp.vInp=Ins2AHROrigi.Vinp/100.0;

    temp.temper=Ins2AHROrigi.Temper/10.0;

    temp.latitude=Ins2AHROrigi.Latitude/1000000000.0;
    temp.longitude=Ins2AHROrigi.Longitude/1000000000.0;
    temp.altitude=Ins2AHROrigi.Altitude/1000.0;

    POINT3D _pos;
    Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());
    LocalCartesian proj(lat0, lon0, alt0, earth);
    proj.Forward(temp.latitude, temp.longitude, temp.altitude, _pos.x, _pos.y, _pos.z);

    temp.originPos=_pos;

    temp.eastSpeed=Ins2AHROrigi.EastSpeed/100.0;
    temp.northSpeed=Ins2AHROrigi.NorthSpeed/100.0;
    temp.verticalSpeed=Ins2AHROrigi.VerticalSpeed/100.0;

    temp.latitudeGNSS=Ins2AHROrigi.LatitudeGNSS/1000000000.0;
    temp.longitudeGNSS=Ins2AHROrigi.LongitudeGNSS/1000000000.0;

    temp.altitudeGNSS=Ins2AHROrigi.AltitudeGNSS/1000.0;

    temp.horizontalSpeed=Ins2AHROrigi.HorizontalSpeed/100.0;
    temp.trackOverGround=Ins2AHROrigi.TrackOverGround/100.0;
    temp.verticalSpeed2=Ins2AHROrigi.VerticalSpeed2/100.0;

    temp.msGps=Ins2AHROrigi.ms_gps;

    temp.GNSSInfo1=Ins2AHROrigi.GNSS_info1;
    temp.GNSSInfo2=Ins2AHROrigi.GNSS_info2;

    temp.solnSVs=Ins2AHROrigi.solnSVs;

    temp.vLatency=Ins2AHROrigi.V_latency/1000.0;

    temp.anglesPositionType=Ins2AHROrigi.AnglesPositionType;

    temp.headingGNSS=Ins2AHROrigi.HeadingGNSS/100.0;

    temp.latencyMsHead=Ins2AHROrigi.latencyMsHead;
    temp.latencyMsPos=Ins2AHROrigi.latencyMsPos;
    temp.latencyMsVel=Ins2AHROrigi.latencyMsVel;

    temp.pBar=Ins2AHROrigi.P_bar*2;

    temp.hBar=Ins2AHROrigi.H_bar/100.0;

    temp.newGPS=Ins2AHROrigi.NewGPS;

    _pos=Coordinate_TransfertoWorld(POINT3D(lonoffset,latoffset,altoffset),temp);

    temp.x=_pos.x;
    temp.y=_pos.y;
    temp.z=_pos.z;

    mutex.lock();
    decodeData.append(temp);
    mutex.unlock();

    emit decodeFinish();
}
