#include"pubfunc.h"
#include<QDebug>

double Em[10][10];

QList<uint8_t> reConstruct(QByteArray lineData)
{
    QList<uint8_t> temp;

    for(int i=0;i<lineData.size();i+=0)
    {
        if(lineData[i]!=' '&&lineData[i]!='\n')
        {
            int j=0;
            QByteArray buff;
            while(lineData[i+j]!=' '&&lineData[i+j]!='\n')
            {
                buff.append(lineData[i+j]);
                j++;
            }

            if(buff.size()!=0)
            {
                uint number=0;
                for(int i=0;i<buff.size();i++)
                {
                    number+=hex2dec(buff[i])*pow(16,buff.size()-i-1);
                }
                temp.append(number);
            }
            else
                j++;

            i+=j;

        }
        else
            i++;
    }

    return temp;

}

uint8_t hex2dec(char hexNum)
{
    uint8_t decNum;
    if(hexNum=='A'||hexNum=='a')
        decNum=10;
    else if(hexNum=='B'||hexNum=='b')
        decNum=11;
    else if(hexNum=='C'||hexNum=='c')
        decNum=12;
    else if(hexNum=='D'||hexNum=='d')
        decNum=13;
    else if(hexNum=='E'||hexNum=='e')
        decNum=14;
    else if(hexNum=='F'||hexNum=='f')
        decNum=15;
    else
        decNum=hexNum-'0';

    return decNum;

}

QList<uint8_t> reConstruct1(QByteArray lineData)
{
    QList<uint8_t> temp;

    for(int i=0;i<lineData.size();i+=0)
    {
        if(lineData[i]!=' '&&lineData[i]!='\n')
        {
            int j=0;
            QByteArray buff;
            while(lineData[i+j]!=' '&&lineData[i+j]!='\n')
            {
                buff.append(lineData[i+j]);
                j++;
            }

            if(buff.size()!=0)
            {
                uint number=0;
                for(int i=0;i<buff.size();i++)
                {
                    number+=hex2dec(buff[i])*pow(16,buff.size()-i-1);
                }
                temp.append(number);
            }
            else
                j++;

            i+=j;

        }
        else
            i++;
    }

    return temp;
}

double GetLOInDegree(double dLIn)
{
    //3°带求带号及中央子午线经度（d的形式）
    //具体公式请参阅《常用大地坐标系及其变换》朱华统，解放军出版社138页
    double L = dLIn;
    double L_ddd_Style = L;
    double ZoneNumber = (int)((L_ddd_Style - 1.5) / 3.0) + 1;
    double L0 = ZoneNumber * 3.0;//degree
    return L0;
}

POINT3D BLH2XYZ(double B, double L, double H)
{
    double x0 = 0;
    double y0 = 0;

    double N, E, h;
    double L0 = GetLOInDegree(L);//根据经度求中央子午线经度

    double a = 6378245.0;//地球半径 北京6378245
    double F = 298.257223563;//地球扁率
    double iPI = 0.0174532925199433;//2pi除以360，用于角度转换

    double f = 1 / F;
    double b = a * (1 - f);
    double ee = (a * a - b * b) / (a * a);
    double e2 = (a * a - b * b) / (b * b);
    double n = (a - b) / (a + b), n2 = (n * n), n3 = (n2 * n), n4 = (n2 * n2), n5 = (n4 * n);
    double al = (a + b) * (1 + n2 / 4 + n4 / 64) / 2;
    double bt = -3 * n / 2 + 9 * n3 / 16 - 3 * n5 / 32;
    double gm = 15 * n2 / 16 - 15 * n4 / 32;
    double dt = -35 * n3 / 48 + 105 * n5 / 256;
    double ep = 315 * n4 / 512;

    B = B * iPI;
    L = L * iPI;
    L0 = L0 * iPI;

    double l = L - L0, cl = (cos(B) * l), cl2 = (cl * cl), cl3 = (cl2 * cl), cl4 = (cl2 * cl2), cl5 = (cl4 * cl), cl6 = (cl5 * cl), cl7 = (cl6 * cl), cl8 = (cl4 * cl4);
    double lB = al * (B + bt * sin(2 * B) + gm * sin(4 * B) + dt * sin(6 * B) + ep * sin(8 * B));
    double t = tan(B), t2 = (t * t), t4 = (t2 * t2), t6 = (t4 * t2);
    double Nn = a / sqrt(1 - ee * sin(B) * sin(B));
    double yt = e2 * cos(B) * cos(B);
    N = lB;
    N += t * Nn * cl2 / 2;
    N += t * Nn * cl4 * (5 - t2 + 9 * yt + 4 * yt * yt) / 24;
    N += t * Nn * cl6 * (61 - 58 * t2 + t4 + 270 * yt - 330 * t2 * yt) / 720;
    N += t * Nn * cl8 * (1385 - 3111 * t2 + 543 * t4 - t6) / 40320;

    E = Nn * cl;
    E += Nn * cl3 * (1 - t2 + yt) / 6;
    E += Nn * cl5 * (5 - 18 * t2 + t4 + 14 * yt - 58 * t2 * yt) / 120;
    E += Nn * cl7 * (61 - 479 * t2 + 179 * t4 - t6) / 5040;

    E += 500000;

    N = 0.9999 * N;
    E = 0.9999 * (E - 500000.0) + 250000.0;

    POINT3D p;
    p.x = E - x0;
    p.y = N - y0;
    p.z= H;

    return p;
}

bool Ins2AHRCheckSum(byte data[], int index)
{
    int sum = 0;
    for (int i = 2; i <= 134;i++)
    {
        sum += data[index + i];
    }
    sum = (sum & 0xffff);

    int checksum = data[index + 135] + data[index + 136] * 256;
    if (sum == checksum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

INS_OPVT2AHR_DATA Ins2AHRDecode(byte data[], int index)
{
    INS_OPVT2AHR_ORIGIDATA Ins2AHROrigi;

    double g=9.8;

    memcpy(&Ins2AHROrigi.Heading, data + index + 6 + 0, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.Pitch, data + index + 6 + 2, sizeof(INT16));
    memcpy(&Ins2AHROrigi.Roll, data + index + 6 + 4, sizeof(INT16));

    memcpy(&Ins2AHROrigi.GyroX, data + index + 6 + 6, sizeof(int));
    memcpy(&Ins2AHROrigi.GyroY, data + index + 6 + 10, sizeof(int));
    memcpy(&Ins2AHROrigi.GyroZ, data + index + 6 + 14, sizeof(int));

    memcpy(&Ins2AHROrigi.AccX, data + index + 6 + 18, sizeof(int));
    memcpy(&Ins2AHROrigi.AccY, data + index + 6 + 22, sizeof(int));
    memcpy(&Ins2AHROrigi.AccZ, data + index + 6 + 26, sizeof(int));

    memcpy(&Ins2AHROrigi.MagX, data + index + 6 + 30, sizeof(INT16));
    memcpy(&Ins2AHROrigi.MagY, data + index + 6 + 32, sizeof(INT16));
    memcpy(&Ins2AHROrigi.MagZ, data + index + 6 + 34, sizeof(INT16));

    memcpy(&Ins2AHROrigi.USW, data + index + 6 + 36, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.Vinp, data + index + 6 + 38, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.Temper, data + index + 6 + 40, sizeof(INT16));

    memcpy(&Ins2AHROrigi.Latitude, data + index + 6 + 42, sizeof(long));
    memcpy(&Ins2AHROrigi.Longitude, data + index + 6 + 50, sizeof(long));
    memcpy(&Ins2AHROrigi.Altitude, data + index + 6 + 58, sizeof(int));

    memcpy(&Ins2AHROrigi.EastSpeed, data + index + 6 + 62, sizeof(int));
    memcpy(&Ins2AHROrigi.NorthSpeed, data + index + 6 + 66, sizeof(int));
    memcpy(&Ins2AHROrigi.VerticalSpeed, data + index + 6 + 70, sizeof(int));

    memcpy(&Ins2AHROrigi.LatitudeGNSS, data + index + 6 + 74, sizeof(long));
    memcpy(&Ins2AHROrigi.LongitudeGNSS, data + index + 6 + 82, sizeof(long));
    memcpy(&Ins2AHROrigi.AltitudeGNSS, data + index + 6 + 90, sizeof(int));

    memcpy(&Ins2AHROrigi.HorizontalSpeed, data + index + 6 + 94, sizeof(int));
    memcpy(&Ins2AHROrigi.TrackOverGround, data + index + 6 + 98, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.VerticalSpeed2, data + index + 6 + 100, sizeof(int));

    memcpy(&Ins2AHROrigi.ms_gps, data + index + 6 + 104, sizeof(UINT));

    memcpy(&Ins2AHROrigi.GNSS_info1, data + index + 6 + 108, sizeof(byte));
    memcpy(&Ins2AHROrigi.GNSS_info2, data + index + 6 + 109, sizeof(byte));

    memcpy(&Ins2AHROrigi.solnSVs, data + index + 6 + 110, sizeof(byte));

    memcpy(&Ins2AHROrigi.V_latency, data + index + 6 + 111, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.AnglesPositionType, data + index + 6 + 113, sizeof(byte));

    memcpy(&Ins2AHROrigi.HeadingGNSS, data + index + 6 + 114, sizeof(UINT16));
    memcpy(&Ins2AHROrigi.latencyMsHead, data + index + 6 + 116, sizeof(INT16));

    memcpy(&Ins2AHROrigi.latencyMsPos, data + index + 6 + 118, sizeof(INT16));
    memcpy(&Ins2AHROrigi.latencyMsVel, data + index + 6 + 120, sizeof(INT16));

    memcpy(&Ins2AHROrigi.P_bar, data + index + 6 + 122, sizeof(UINT16));

    memcpy(&Ins2AHROrigi.H_bar, data + index + 6 + 124, sizeof(int));

    memcpy(&Ins2AHROrigi.NewGPS, data + index + 6 + 128, sizeof(byte));

    INS_OPVT2AHR_DATA temp;
    temp.heading=Ins2AHROrigi.Heading*0.01;
    temp.pitch=Ins2AHROrigi.Pitch*0.01;
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

    POINT3D p=BLH2XYZ(temp.latitude,temp.longitude,0);
    temp.x=p.x;
    temp.y=p.y;

    temp.altitude=Ins2AHROrigi.Altitude/1000.0;

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

    return temp;

}

QList<INS_OPVT2AHR_DATA> getInsDecodeData(QList<uint8_t> templist)
{
    int size=templist.size();
    uint8_t *Buffer=new uint8_t[size];
    int dataCount=0;
    for(int i=0;i<size;i++)
    {
        Buffer[dataCount++]=templist[i];
    }


    int i=0;
    QList<INS_OPVT2AHR_DATA> srcData;
    while(dataCount>137)
    {
        if(Buffer[i]==0xAA&&Buffer[i+1]==0x55)
        {
            if(Ins2AHRCheckSum(Buffer,i))
            {
                srcData.append(Ins2AHRDecode(Buffer,i));
                i+=137;
                dataCount-=137;
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

    delete Buffer;

    return srcData;
}

QList<MAP_POINT> getMapFormat(QList<INS_OPVT2AHR_DATA> srcData)
{
    QList<MAP_POINT> tempMap;
    for(int i=0;i<srcData.size();i++)
    {
        MAP_POINT tempPoint;
        tempPoint.roadIndex=0;
        tempPoint.point.x=srcData[i].x;
        tempPoint.point.y=srcData[i].y;
        tempPoint.type=TRACK_POINT_TYPE::none;
        tempMap.append(tempPoint);
    }
    return tempMap;
}

QList<MAP_POINT> filtratePoint(QList<MAP_POINT> srcData)
{
    QList<MAP_POINT> tempList;
    if(srcData.size()>0)
    {
        tempList.append(srcData[0]);
        for(int i=1;i<srcData.size();i++)
        {
            double dist= get2Dis(tempList.last().point,srcData[i].point);
            if(dist<1&&dist>=0.1)
                tempList.append(srcData[i]);
        }

    }
    qDebug()<<tempList.size();
    return tempList;

}

double get2Dis(POINT3D p0, POINT3D p1)
{
    return sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y));
}

double get2Dis(POINT2D p0, POINT2D p1)
{
    return sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y));
}

POINT2D Coordinate_TransfertoVehicle(POINT2D srcPoint,INS_OPVT2AHR_DATA insData)
{
    double x_vehicle,y_vehicle;
    double theta=atan2(srcPoint.x-insData.x,srcPoint.y-insData.y);
    x_vehicle=cos(theta-insData.heading/180*PI)*sqrt((srcPoint.x-insData.x)*(srcPoint.x-insData.x)+(srcPoint.y-insData.y)*(srcPoint.y-insData.y));
    y_vehicle=sin(theta-insData.heading/180*PI)*sqrt((srcPoint.x-insData.x)*(srcPoint.x-insData.x)+(srcPoint.y-insData.y)*(srcPoint.y-insData.y));
    POINT2D P;
    P.y=x_vehicle;
    P.x=-y_vehicle;
    return P;
}

QList<MAP_POINT> reConstruct2(QByteArray lineData)
{
    QList<MAP_POINT> tempList;
    int i=0;
    while(i<lineData.size())
    {
        int j=0;
        QByteArray temp;
        while(lineData[i+j]!='\n')
        {
            temp.append(lineData[i+j]);
            j++;
        }
        j++;
        i+=j;

        int k=0;
        QList<double> temp1;
        while(k<temp.size())
        {

            QByteArray temp2;
            while(temp[k]!='\t')
            {
                temp2.append(temp[k]);
                k++;
            }
            k++;
            temp1.append(temp2.toDouble());
        }

        if(temp1.size()>3)
        {
            MAP_POINT tempPoint;
            tempPoint.roadIndex=temp1[0];
            tempPoint.type=(TRACK_POINT_TYPE)temp1[1];
            tempPoint.point.x=temp1[2];
            tempPoint.point.y=temp1[3];
            tempList.append(tempPoint);
        }

    }
    return tempList;
}

int getNearestPointIndexInGround(QList<MAP_POINT> mapList, INS_OPVT2AHR_DATA pos, int lastIndex)
{
    int SearchCNT=300;
    int minIndex=0;
    double minDis=99999999;
    if(lastIndex==0)
    {
        for(int i=0;i<mapList.size();i++)
        {
            double dis=get2Dis(mapList[i].point,POINT3D(pos.x,pos.y,pos.z));
            if(dis<minDis)
            {
                minDis=dis;
                minIndex=i;
            }
        }

    }
    else
    {

        for(int i=lastIndex-SearchCNT;i<lastIndex+SearchCNT;i++)
        {
            int j=(i+mapList.size())%mapList.size();
            double dis=get2Dis(mapList[j].point,POINT3D(pos.x,pos.y,pos.z));
            if(dis<minDis)
            {
                minDis=dis;
                minIndex=j;
            }

        }
    }
    return minIndex;
}

//累加
double sum(vector<double> Vnum, int n)
{
    double dsum=0;
    for (int i=0; i<n; i++)
    {
        dsum+=Vnum[i];
    }
    return dsum;
}
//乘积和
double MutilSum(vector<double> Vx, vector<double> Vy, int n)
{
    double dMultiSum=0;
    for (int i=0; i<n; i++)
    {
        dMultiSum+=Vx[i]*Vy[i];
    }
    return dMultiSum;
}
//ex次方和
double RelatePow(vector<double> Vx, int n, int ex)
{
    double ReSum=0;
    for (int i=0; i<n; i++)
    {
        ReSum+=pow(Vx[i],ex);
    }
    return ReSum;
}
//x的ex次方与y的乘积的累加
double RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex)
{
    double dReMultiSum=0;
    for (int i=0; i<n; i++)
    {
        dReMultiSum+=pow(Vx[i],ex)*Vy[i];
    }
    return dReMultiSum;
}
//计算方程组的增广矩阵
void EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[])
{
    for (int i=1; i<=ex; i++)
    {
        for (int j=1; j<=ex; j++)
        {
            Em[i][j]=RelatePow(Vx,n,i+j-2);
        }
        Em[i][ex+1]=RelateMutiXY(Vx,Vy,n,i-1);
    }
    Em[1][1]=n;
    CalEquation(ex,coefficient);
}
//求解方程
void CalEquation(int exp, double coefficient[])
{
    for(int k=1;k<exp;k++) //消元过程
    {
        for(int i=k+1;i<exp+1;i++)
        {
            double p1=0;

            if(Em[k][k]!=0)
                p1=Em[i][k]/Em[k][k];

            for(int j=k;j<exp+2;j++)
                Em[i][j]=Em[i][j]-Em[k][j]*p1;
        }
    }
    coefficient[exp]=Em[exp][exp+1]/Em[exp][exp];
    for(int l=exp-1;l>=1;l--)   //回代求解
        coefficient[l]=(Em[l][exp+1]-F(coefficient,l+1,exp))/Em[l][l];
}
//供CalEquation函数调用
double F(double c[],int l,int m)
{
    double sum=0;
    for(int i=l;i<=m;i++)
        sum+=Em[l-1][i]*c[i];
    return sum;
}

double Max(double a, double b)
{
    return a>b?a:b;
}

double Min(double a, double b)
{
    return a<b?a:b;
}

int getNearestPointIndexInVehicle(QList<MAP_POINT> mapList, int lastIndex)
{
    int SearchCNT=300;
    int minIndex=0;
    double minDis=99999999;
    if(lastIndex==0)
    {
        for(int i=0;i<mapList.size();i++)
        {
            double dis=get2Dis(mapList[i].point,POINT3D(0,0,0));
            if(dis<minDis)
            {
                minDis=dis;
                minIndex=i;
            }
        }

    }
    else
    {

        for(int i=lastIndex-SearchCNT;i<lastIndex+SearchCNT;i++)
        {
            int j=(i+mapList.size())%mapList.size();
            double dis=get2Dis(mapList[j].point,POINT3D(0,0,0));
            if(dis<minDis)
            {
                minDis=dis;
                minIndex=j;
            }

        }
    }
    return minIndex;
}

POINT3D Coordinate_TransfertoWorld(POINT3D srcPoint, INS_OPVT2AHR_DATA insData)
{
    POINT3D output_data;
    //
    double x=srcPoint.x;
    double y=srcPoint.y;
    double z=srcPoint.z;

    //绕y轴旋转
    double x2,y2,z2;
    double pitch_com=(insData.pitch)*M_PI/180.0;
    z2=z*cos(pitch_com)-x*sin(pitch_com);
    x2=z*sin(pitch_com)+x*cos(pitch_com);
    y2=y;

    //绕x轴旋转
    double x1,y1,z1;
    double roll_com=(insData.roll)*M_PI/180.0;
    y1 = y2*cos(roll_com)-z2*sin(roll_com);
    z1 = y2*sin(roll_com)+z2*cos(roll_com);
    x1 = x2;

    //绕z轴旋转
    double x3,y3,z3;
    double yaw_com=(-(insData.heading)+90)*M_PI/180.0;
    x3=x1*cos(yaw_com)-y1*sin(yaw_com);
    y3=x1*sin(yaw_com)+y1*cos(yaw_com);
    z3=z1;



    output_data.x =x3 +insData.originPos.x ;//x no change
    output_data.y =y3 +insData.originPos.y;
    output_data.z =z3 +insData.originPos.z;



    return output_data;
}

int calcDisOnPath(QList<TRACK_POINT> path, int startIndex, double dis)
{
    int endIndex=-1;
    double calcDis=0;
    for(int i=startIndex;i<path.size()-1;i++)
    {
        calcDis+=get2Dis(path[i].point,path[i+1].point);
        if(calcDis>=dis)
        {
            endIndex=i+1;
            break;
        }
    }
    return endIndex;
}

double calcCurvature(POINT3D p0, POINT3D p1, POINT3D p2)
{
    double curvature=0;

   if(p0.x==p1.x)
   {
       if(p1.x==p2.x)
           curvature=0;
       else
       {
           double midy=0.5*(p0.y+p1.y);
           if(p1.y==p2.y)
           {
               double midx=0.5*(p1.x+p2.x);
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
           else
           {
               double k=-(p2.x-p1.x)/(p2.y-p1.y);
               double b=0.5*(p2.y+p1.y)-k*0.5*(p2.x+p1.x);
               double midx=(midy-b)/k;
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
       }
   }
   else if(p1.x==p2.x)
   {
       if(p1.x==p0.x)
           curvature=0;
       else
       {
           double midy=0.5*(p2.y+p1.y);
           if(p1.y==p0.y)
           {
               double midx=0.5*(p1.x+p0.x);
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
           else
           {
               double k=-(p0.x-p1.x)/(p0.y-p1.y);
               double b=0.5*(p0.y+p1.y)-k*0.5*(p0.x+p1.x);
               double midx=(midy-b)/k;
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
       }
   }
   else if(p0.x==p2.x)
   {
       if(p1.x==p0.x)
           curvature=0;
       else
       {
           double midy=0.5*(p2.y+p0.y);
           if(p1.y==p0.y||p0.y==p2.y)
           {
               double midx=0.5*(p1.x+p0.x);
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
           else
           {
               double k=-(p0.x-p1.x)/(p0.y-p1.y);
               double b=0.5*(p0.y+p1.y)-k*0.5*(p0.x+p1.x);
               double midx=(midy-b)/k;
               POINT3D circlePoint(midx,midy,0);
               curvature=1.0/get2Dis(p0,circlePoint);
           }
       }
   }
   else
   {
       double k0=(p1.y-p0.y)/(p1.x-p0.x);
       double k1=(p2.y-p0.y)/(p2.x-p0.x);
       double k2=(p2.y-p1.y)/(p2.x-p1.x);

       if(k0==k2)
           curvature=0;
       else
       {
           curvature=abs((k2-k0)/(p2.x-p0.x))/sqrt(pow(1+k1*k1,3));
       }

   }

   if(curvature<=pow(10,-6))
       curvature=0;

    return curvature;
}

float average(QList<float> data)
{
    float result=0;
    float sum=0;
    for(int i=0;i<data.size();i++)
    {
        sum+=data[i];
    }
    result=sum/data.size();
    return result;
}

double get3Dis(POINT3D p0, POINT3D p1)
{
    return sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)+(p0.z-p1.z)*(p0.z-p1.z));
}
