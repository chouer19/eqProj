#ifndef INS_NOVATEL_PP7D_DECODE_H
#define INS_NOVATEL_PP7D_DECODE_H

#include"commonUsed/typedefs.h"

#define lonOffset 0
#define PI 3.1415926535897932384626433832795

struct inspvax_message
{
    QString msg_header;
    QString ins_status;
    QString position_type;

    double latitude;
    double longitude;
    double altitude;
    double undulation;

    double x;
    double y;
    double z;

    double north_velocity;
    double east_velocity;
    double up_velocity;

    double roll;
    double pitch;
    double azimuth;

    double latitude_std;
    double longitude_std;
    double altitude_std;

    double north_velocity_std;
    double east_velocity_std;
    double up_velocity_std;

    double roll_std;
    double pitch_std;
    double azimuth_std;

    uint32_t extended_status;

    uint16_t seconds_since_update;

    uint32_t CRC;

};


class ins_novatel_pp7d_decode
{
public:
    ins_novatel_pp7d_decode();
    inspvax_message msgDecode(QByteArray text);

private:

    double GetLOInDegree(double dLIn);
    POINT2D BLH2XYZ(double B, double L, double H);
};

#endif // INS_NOVATEL_PP7D_DECODE_H
