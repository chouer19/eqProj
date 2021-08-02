#include "header/protocol/pro_dgps.hpp"

#define pitch_offset 0
#define roll_offset 0
#define lon_offset 0
#define lat_offset 0
#define alt_offset 0

void IDGPS_INS_D::install()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    taskId = sys->taskList_p[core]->push_back(this);
    m_isAlive = true;
    m_thread = std::thread(task_thread, this);
}

void IDGPS_INS_D::parse(void *frm)
{
    serial_frm_t *sp_frm = (serial_frm_t*)frm;
    //checksum
    int sum = 0;
    for (int i = 2; i <= 134;i++)
    {
        sum += sp_frm->frm.datas[i];
    }
    sum = (sum & 0xffff);

    int checksum = sp_frm->frm.datas[135] + sp_frm->frm.datas[136] * 256;
    if (sum == checksum)
    {
        //parse data here
        uint16_t _heading_data = 0;
        int16_t _pitch_data = 0;
        int16_t _roll_data = 0;
        memcpy(&_heading_data, &sp_frm->frm.datas[6 + 0], sizeof(uint16_t));
        memcpy(&_pitch_data, &sp_frm->frm.datas[6 + 2], sizeof(int16_t));
        memcpy(&_roll_data, &sp_frm->frm.datas[6 + 4], sizeof(int16_t));

        int _gyrox_data = 0;
        int _gyroy_data = 0;
        int _gyroz_data = 0;
        memcpy(&_gyrox_data, &sp_frm->frm.datas[6 + 6], sizeof(int));
        memcpy(&_gyroy_data, &sp_frm->frm.datas[6 + 10], sizeof(int));
        memcpy(&_gyroz_data, &sp_frm->frm.datas[6 + 14], sizeof(int));


        int _accX = 0;
        int _accY = 0;
        int _accZ = 0;
        memcpy(&_accX, &sp_frm->frm.datas[6 + 18], sizeof(int));
        memcpy(&_accY, &sp_frm->frm.datas[6 + 22], sizeof(int));
        memcpy(&_accZ, &sp_frm->frm.datas[6 + 26], sizeof(int));

        int16_t _magX = 0;
        int16_t _magY = 0;
        int16_t _magZ = 0;
        memcpy(&_magX, &sp_frm->frm.datas[6 + 30], sizeof(int16_t));
        memcpy(&_magY, &sp_frm->frm.datas[6 + 32], sizeof(int16_t));
        memcpy(&_magZ, &sp_frm->frm.datas[6 + 34], sizeof(int16_t));

        uint16_t _usw = 0;
        memcpy(&_usw, &sp_frm->frm.datas[6 + 36], sizeof(uint16_t));

        uint16_t _vinp = 0;
        memcpy(&_vinp, &sp_frm->frm.datas[6 + 38], sizeof(uint16_t));

        int16_t _tmper = 0;
        memcpy(&_tmper, &sp_frm->frm.datas[6 + 40], sizeof(int16_t));

        long _lat = 0;
        long _lon = 0;
        int _att = 0;
        memcpy(&_lat, &sp_frm->frm.datas[6 + 42], sizeof(long));
        memcpy(&_lon, &sp_frm->frm.datas[6 + 50], sizeof(long));
        memcpy(&_att, &sp_frm->frm.datas[6 + 58], sizeof(int));

        int _espd = 0;
        int _nspd = 0;
        int _verspd = 0;
        memcpy(&_espd, &sp_frm->frm.datas[6 + 62], sizeof(int));
        memcpy(&_nspd, &sp_frm->frm.datas[6 + 66], sizeof(int));
        memcpy(&_verspd, &sp_frm->frm.datas[6 + 70], sizeof(int));

        long _lat_gnss = 0;
        long _lon_gnss = 0;
        int _att_gnss = 0;
        memcpy(&_lat_gnss, &sp_frm->frm.datas[6 + 74], sizeof(long));
        memcpy(&_lon_gnss, &sp_frm->frm.datas[6 + 82], sizeof(long));
        memcpy(&_att_gnss, &sp_frm->frm.datas[6 + 90], sizeof(int));

        int _hspd = 0;
        uint16_t _tr_over_grd = 0;
        int _verspd2 = 0;
        memcpy(&_hspd, &sp_frm->frm.datas[6 + 94], sizeof(int));
        memcpy(&_tr_over_grd, &sp_frm->frm.datas[6 + 98], sizeof(uint16_t));
        memcpy(&_verspd2, &sp_frm->frm.datas[6 + 100], sizeof(int));

        uint32_t _gps_ms = 0;
        memcpy(&_gps_ms, &sp_frm->frm.datas[6 + 104], sizeof(uint32_t));

        uint8_t _gnss_info1 = 0;
        uint8_t _gnss_info2 = 0;
        memcpy(&_gnss_info1, &sp_frm->frm.datas[6 + 108], sizeof(uint8_t));
        memcpy(&_gnss_info2, &sp_frm->frm.datas[6 + 109], sizeof(uint8_t));

        uint8_t _soln_svs = 0;
        memcpy(&_soln_svs, &sp_frm->frm.datas[6 + 110], sizeof(uint8_t));

        uint16_t _vlatency = 0;
        memcpy(&_vlatency, &sp_frm->frm.datas[6 + 111], sizeof(uint16_t));

        uint8_t _ang_pos_type = 0;
        memcpy(&_ang_pos_type, &sp_frm->frm.datas[6 + 113], sizeof(uint8_t));

        uint16_t _head_gnss = 0;
        uint16_t _latency_ms_head = 0;
        memcpy(&_head_gnss, &sp_frm->frm.datas[6 + 114], sizeof(uint16_t));
        memcpy(&_latency_ms_head, &sp_frm->frm.datas[6 + 116], sizeof(uint16_t));

        int16_t _latency_ms_pos = 0;
        int16_t _latency_ms_vel = 0;
        memcpy(&_latency_ms_pos, &sp_frm->frm.datas[6 + 118], sizeof(int16_t));
        memcpy(&_latency_ms_vel, &sp_frm->frm.datas[6 + 120], sizeof(int16_t));

        uint16_t _pbar = 0;
        int _hbar = 0;
        uint8_t _new_gps = 0;
        memcpy(&_pbar, &sp_frm->frm.datas[6 + 122], sizeof(uint16_t));
        memcpy(&_hbar, &sp_frm->frm.datas[6 + 124], sizeof(int));
        memcpy(&_new_gps, &sp_frm->frm.datas[6 + 128], sizeof(uint8_t));

        {
            std::unique_lock<std::mutex> lck(m_mutex);
            frm_data.time_stamp = pub_time->getTimeMs();
            frm_data.heading = _heading_data * 0.01;
            frm_data.pitch =-(_pitch_data * 0.01-pitch_offset);
            frm_data.roll = (_roll_data * 0.01-roll_offset);

            pitchlist.push_back(frm_data.pitch);
            if(pitchlist.size()>30)
                pitchlist.erase(pitchlist.begin());

            double pitch_average=0;
            for(int i=0;i<pitchlist.size();i++)
            {
                pitch_average+=pitchlist[i];
            }
            m_pitchAver=pitch_average/pitchlist.size();

            frm_data.gyroX = _gyrox_data / 100000.0;
            frm_data.gyroY = _gyroy_data / 100000.0;
            frm_data.gyroZ = _gyroz_data / 100000.0;

            frm_data.accX = _accX / 1000000.0;
            frm_data.accY = _accY / 1000000.0;
            frm_data.accZ = _accZ / 1000000.0;

            frm_data.magX = _magX * 10;
            frm_data.magY = _magY * 10;
            frm_data.magZ = _magZ * 10;

            frm_data.usw = _usw;

            frm_data.vInp = _vinp / 100.0;

            frm_data.temper = _tmper / 10.0;

            frm_data.latitude = _lat / 1000000000.0;
            frm_data.longitude = _lon / 1000000000.0;
            frm_data.altitude = _att / 1000.0;

            point_3d_t _pos = blh2xyz(frm_data.latitude,frm_data.longitude,frm_data.altitude);

            frm_data.pos=_pos;


            frm_data.eastSpeed = _espd / 100.0;
            frm_data.northSpeed = _nspd / 100.0;
            frm_data.verticalSpeed = _verspd / 100.0;

            frm_data.latitudeGNSS = _lat_gnss / 1000000000.0;
            frm_data.longitudeGNSS = _lon_gnss / 1000000000.0;

            frm_data.altitudeGNSS = _att_gnss / 1000.0;

            frm_data.horizontalSpeed = _hspd / 100.0;
            frm_data.trackOverGround = _tr_over_grd / 100.0;
            frm_data.verticalSpeed2 = _verspd2 / 100.0;

            frm_data.msGps = _gps_ms;

            frm_data.GNSSInfo1 = _gnss_info1;
            frm_data.GNSSInfo2 = _gnss_info2;

            frm_data.solnSVs = _soln_svs;

            frm_data.vLatency = _vlatency / 1000.0;

            frm_data.anglesPositionType = _ang_pos_type;

            frm_data.headingGNSS = _head_gnss / 100.0;

            frm_data.latencyMsHead = _latency_ms_head;
            frm_data.latencyMsPos = _latency_ms_pos;
            frm_data.latencyMsVel = _latency_ms_vel;

            frm_data.pBar = _pbar * 2;

            frm_data.hBar = _hbar / 100.0;

            frm_data.newGPS = _new_gps;


            //
            point_3d_t offset(lon_offset,lat_offset,alt_offset);
            frm_data.pos=Coordinate_TransfertoWorld(offset,frm_data);

            frm_data_1s.push_back(frm_data);
            if(frm_data_1s.size()>200)
                frm_data_1s.erase(frm_data_1s.begin());
        }

    }


}



point_3d_t IDGPS_INS_D::Coordinate_TransfertoWorld(point_3d_t srcPoint, nav_t insData)
{
    point_3d_t output_data;
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

    output_data.x =-y3 +insData.pos.x;//x no change
    output_data.y =x3 +insData.pos.y;
    output_data.z =z3 +insData.pos.z;

    return output_data;
}

void IDGPS_INS_D::err_check()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    if(pub_time->getDiffTimeMs(frm_data.time_stamp) > 1000)
    {
        nav_t emptyData;
        frm_data=emptyData;
        frm_data_1s.clear();
    }
}

float IDGPS_INS_D::heading()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.heading;    //deg
}

float IDGPS_INS_D::pitch()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.pitch;    //deg
}

float IDGPS_INS_D::pitchAver()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_pitchAver;    //deg
}

float IDGPS_INS_D::roll()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.roll;   //deg
}

float IDGPS_INS_D::headingSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.headingSTD;   //deg
}

float IDGPS_INS_D::pitchSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.pitchSTD;   //deg
}

float IDGPS_INS_D::rollSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.rollSTD;   //deg
}

double IDGPS_INS_D::latitude()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.latitude;   //deg
}

double IDGPS_INS_D::longitude()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.longitude;   //deg
}

double IDGPS_INS_D::altitude()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.altitude;//m
}

double IDGPS_INS_D::latitudeSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.latitudeSTD;   //deg
}

double IDGPS_INS_D::longitudeSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.longitudeSTD;   //deg
}

double IDGPS_INS_D::altitudeSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.altitudeSTD;//m
}

float IDGPS_INS_D::vsdNorth()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.northSpeed;//m/s
}

float IDGPS_INS_D::vsdEast()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.eastSpeed;//m/s
}

float IDGPS_INS_D::vsdUp()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.verticalSpeed;//m/s
}

float IDGPS_INS_D::vsdNorthSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.northSpeedSTD;//m/s
}

float IDGPS_INS_D::vsdEastSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.northSpeedSTD;//m/s
}

float IDGPS_INS_D::vsdUpSTD()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.northSpeedSTD;//m/s
}

string IDGPS_INS_D::RTKStatus()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.RTK_status;
}

string IDGPS_INS_D::AlignStatus()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.align_status;
}

uint8_t IDGPS_INS_D::anglesPositionType()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.anglesPositionType;
}


point_3d_t IDGPS_INS_D::position()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.pos;
}

float IDGPS_INS_D::yaw_rate()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.gyroZ;       //deg/s
}

float IDGPS_INS_D::roll_rate()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.gyroY;       //deg/s
}

float IDGPS_INS_D::pitch_rate()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.gyroX;       //deg/s
}

float IDGPS_INS_D::lon_accel()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.accY * _g_ ;
}

float IDGPS_INS_D::lat_accel()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.accX * _g_;//m/s^2
}

float IDGPS_INS_D::alt_accel()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.accZ * _g_;//m/s^2
}

nav_t IDGPS_INS_D::fram_data()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data;
}

uint8_t IDGPS_INS_D::imuWorkStatus()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data.imu_work_status;
}

vector<nav_t> IDGPS_INS_D::fram_data_1s()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return frm_data_1s;
}


//private functions
point_3d_t IDGPS_INS_D::blh2xyz(double B, double L, double H)
{
    double x0 = 0;
    double y0 = 0;

    double N, E, h;
    double L0 = getLOInDegree(L);//根据经度求中央子午线经度

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

    point_3d_t p;
    p.x = E - x0;
    p.y = N - y0;
    p.z = H;
    h = H;

    return p;
}

double IDGPS_INS_D::getLOInDegree(double dLIn)
{
    //3°带求带号及中央子午线经度（d的形式）
    //具体公式请参阅《常用大地坐标系及其变换》朱华统，解放军出版社138页
    double L = dLIn;
    double L_ddd_Style = L;
    double ZoneNumber = (int)((L_ddd_Style - 1.5) / 3.0) + 1;
    double L0 = ZoneNumber * 3.0;//degree
    return L0;
}

void IDGPS_INS_D::task_func()
{

    /////////////////////Error Check////////////////////////////
    err_check();

    //
    nav_t _fram_data=fram_data();

    ADCU_9 _adcu9;
    _adcu9.ADCU_latitude=_fram_data.latitude;
    _adcu9.ADCU_longitude=_fram_data.longitude;
    veh->setAdcu9(_adcu9);

    ADCU_10 _adcu10;
    _adcu10.ADCU_elevation=_fram_data.altitude;
    veh->setAdcu10(_adcu10);

    ADCU_11 _adcu11;
    _adcu11.ADCU_PitchDeg=_fram_data.pitch;
    _adcu11.ADCU_RollDeg=_fram_data.roll;
    _adcu11.ADCU_YawDeg=_fram_data.heading;
    veh->setAdcu11(_adcu11);

}


