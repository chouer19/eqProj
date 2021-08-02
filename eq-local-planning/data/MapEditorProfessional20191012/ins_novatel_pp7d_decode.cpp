#include "ins_novatel_pp7d_decode.h"

ins_novatel_pp7d_decode::ins_novatel_pp7d_decode()
{

}

double ins_novatel_pp7d_decode::GetLOInDegree(double dLIn)
{
    //3°带求带号及中央子午线经度（d的形式）
    //具体公式请参阅《常用大地坐标系及其变换》朱华统，解放军出版社138页
    double L = dLIn;
    double L_ddd_Style = L;
    double ZoneNumber = (int)((L_ddd_Style - 1.5) / 3.0) + 1;
    double L0 = ZoneNumber * 3.0;//degree
    return L0;
}

POINT2D ins_novatel_pp7d_decode::BLH2XYZ(double B, double L, double H)
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

    POINT2D p;
    p.x = E - x0;
    p.y = N - y0;
    h = H;

    return p;
}

inspvax_message ins_novatel_pp7d_decode::msgDecode(QByteArray text)
{
    inspvax_message msgTemper;
    int i=0;


    while (text[i]!=';')
    {
        msgTemper.msg_header.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;

    while (text[i]!=',')
    {
        msgTemper.ins_status.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;

    while (text[i]!=',')
    {
        msgTemper.position_type.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;

    QString latitudeTemper;
    while (text[i]!=',')
    {
        latitudeTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.latitude=latitudeTemper.toDouble();

    QString longitudeTemper;
    while (text[i]!=',')
    {
        longitudeTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.longitude=longitudeTemper.toDouble();

    QString altitudeTemper;
    while (text[i]!=',')
    {
        altitudeTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.altitude=altitudeTemper.toDouble();

    QString undulationTemper;
    while (text[i]!=',')
    {
        undulationTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.undulation=undulationTemper.toDouble();

    QString north_velocityTemper;
    while (text[i]!=',')
    {
        north_velocityTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.north_velocity=north_velocityTemper.toDouble();

    QString east_velocityTemper;
    while (text[i]!=',')
    {
        east_velocityTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.east_velocity=east_velocityTemper.toDouble();

    QString up_velocityTemper;
    while (text[i]!=',')
    {
        up_velocityTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.up_velocity=up_velocityTemper.toDouble();

    QString rollTemper;
    while (text[i]!=',')
    {
        rollTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.roll=rollTemper.toDouble();

    QString pitchTemper;
    while (text[i]!=',')
    {
        pitchTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.pitch=pitchTemper.toDouble();

    QString azimuthTemper;
    while (text[i]!=',')
    {
        azimuthTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.azimuth=azimuthTemper.toDouble();

    QString latitude_stdTemper;
    while (text[i]!=',')
    {
        latitude_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.latitude_std=latitude_stdTemper.toDouble();

    QString longitude_stdTemper;
    while (text[i]!=',')
    {
        longitude_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.longitude_std=longitude_stdTemper.toDouble();

    QString altitude_stdTemper;
    while (text[i]!=',')
    {
        altitude_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.altitude_std=altitude_stdTemper.toDouble();

    QString north_velocity_stdTemper;
    while (text[i]!=',')
    {
        north_velocity_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.north_velocity_std=north_velocity_stdTemper.toDouble();

    QString east_velocity_stdTemper;
    while (text[i]!=',')
    {
        east_velocity_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.east_velocity_std=east_velocity_stdTemper.toDouble();

    QString up_velocity_stdTemper;
    while (text[i]!=',')
    {
        up_velocity_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.up_velocity_std=up_velocity_stdTemper.toDouble();

    QString roll_stdTemper;
    while (text[i]!=',')
    {
        roll_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.roll_std=roll_stdTemper.toDouble();

    QString pitch_stdTemper;
    while (text[i]!=',')
    {
        pitch_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.pitch_std=pitch_stdTemper.toDouble();

    QString azimuth_stdTemper;
    while (text[i]!=',')
    {
        azimuth_stdTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.azimuth_std=azimuth_stdTemper.toDouble();

    QString extended_statusTemper;
    while (text[i]!=',')
    {
        extended_statusTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.extended_status=extended_statusTemper.toUInt();

    QString seconds_since_updateTemper;
    while (text[i]!='*')
    {
        seconds_since_updateTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    msgTemper.seconds_since_update=seconds_since_updateTemper.toUInt();

    QString CRCTemper;
//    CRCTemper.push_back('0');
//    CRCTemper.push_back('x');
    while (text[i]!='\n')
    {
        CRCTemper.push_back(char(text[i]));
        i++;
        if(i>=text.size())
            return msgTemper;
    }
    i++;
    bool ok;
    msgTemper.CRC=CRCTemper.toUInt(&ok, 16);

    POINT2D p=BLH2XYZ(msgTemper.latitude,msgTemper.longitude,0);
    double theta= msgTemper.azimuth*PI/180;
    msgTemper.x=p.x+lonOffset*sin(theta);
    msgTemper.y=p.y+lonOffset*cos(theta);
    msgTemper.z=msgTemper.altitude;

    return msgTemper;
}
