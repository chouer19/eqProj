#include "eqDriving/planning/frenet.hpp"
using namespace eqDriving;
using namespace planning;

#include "eqDriving/vehicle/State.hpp"
#include "eqDriving/vehicle/Control.hpp"
using namespace vehicle;

#include "thirdParty/Eigen/Dense"

#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
using namespace std;

#include "thirdParty/sqlite/sqlite3.h"

void eqDriving::planning::calcPath(
    const vector<double> &_base_xs, 
    const vector<double> &_base_ys, 
    const vector<double> &_base_yaws,

    const double &_cur_x, const double &_cur_y, const double &_cur_yaw,
    const double &_offset, const double &_baseline,
    vector<double> &_path_xs,
    vector<double> &_path_ys
              ){
    // current s,d
    double min_d = 0; double min_s = 0;double sum_s = 0;double min_yaw = 0;
    double nearest = 9999;
    vector<double> _base_ss;
    _base_ss.push_back(0);
    for(int i=0;i<_base_xs.size()-1;i++){
        Eigen::Vector3d v(_base_xs[i+1]-_base_xs[i],_base_ys[i+1]-_base_ys[i],0);
        Eigen::Vector3d w(_cur_x-_base_xs[i], _cur_y-_base_ys[i],0);
        double nearest_tmp = w.norm();
        if(nearest > nearest_tmp){
            nearest = nearest_tmp;
            min_d = w.cross(v)[2]/v.norm();
            min_s = sum_s + w.dot(v)/v.norm();
            min_yaw = _cur_yaw - _base_yaws[i];
        }
        sum_s += v.norm();
        _base_ss.push_back(sum_s);
    }
    cout << min_d <<"," << min_s << endl;
    // ds
    auto AA = [](int TT){
        Eigen::Matrix<double,3,3> A;
        A << pow(TT,3) , pow(TT,4) , pow(TT,5),
             3*pow(TT,2), 4*pow(TT,3), 5*pow(TT,4),
             6*pow(TT,1), 12*pow(TT,2), 20*pow(TT,3);
        return A;
    };
    auto bb = [](double a_0, double a_1, double a_2, int TT, double offset){
        Eigen::Matrix<double,3,1> b;
        b << offset - a_0 - a_1*TT - a_2/2*pow(TT,2),
             0 - a_1 - a_2*TT,
             0 - a_2;
        return b;
    };
    int T = 5;
    double a_0 = min_d;
    double a_1 = _baseline/T*tan(min_yaw);
    double a_2 = 0;
    Eigen::Matrix<double,3,3> A = AA(T);
    Eigen::Matrix<double,3,1> b = bb(a_0, a_1, a_2, T, _offset);
    auto aa = A.householderQr().solve(b);
    double a_3 = aa[0]; double a_4 = aa[1]; double a_5=aa[2];
    vector<double> _ds;
    for(int i=0;i<_baseline*10;i++){
        double x = i * 0.1 * 5 / _baseline;
        _ds.push_back(a_0+a_1*x+a_2*x*x+a_3*x*x*x+a_4*x*x*x*x+a_5*x*x*x*x*x);
    }
    // ss
    vector<double> _ss;
    for(int i=0;i<_baseline*10;i++){
        _ss.push_back(min_s+i*0.1);
    }
    //xys
    _path_xs.clear(); _path_ys.clear();
    size_t keypoint = 0;
    for(int i=0; i<_ss.size(); i++){
        while(keypoint < _base_xs.size()-1 && _base_ss[keypoint+1]<_ss[i]){keypoint++;}
        double forward = _ss[i] - _base_ss[keypoint];
        double target_x = _base_xs[keypoint] + forward*sin(_base_yaws[keypoint]);
        double target_y = _base_ys[keypoint] + forward*cos(_base_yaws[keypoint]);
        double offset_x = _ds[i]*sin(_base_yaws[keypoint]+asin(1));
        double offset_y = _ds[i]*cos(_base_yaws[keypoint]+asin(1));
        _path_xs.push_back(target_x+offset_x);
        _path_ys.push_back(target_y+offset_y);
    }
}

// Private data for the Frenet class
class eqDriving::planning::FrenetPrivate
{
  public: vector<double> map_x;
  public: vector<double> map_y;
  public: vector<double> map_z;
  public: vector<double> map_yaw;
  public: vector<double> map_s;

  public: deque<double> old_d;
  public: deque<double> old_s;

  public: string table_name = "";

  public: vector<double> a{0,0,0,0,0,0};

  public: State state;
  public: int nearest_index = 0;
};

void Frenet::calcPaths(const State &_state){

}

void Frenet::calcPath(const double &_offset, const double &_baseline,vector<double> &_xs, vector<double> &_ys){
    _xs.clear(); _ys.clear();
    auto AA = [](int TT){
        Eigen::Matrix<double,3,3> A;
        A << pow(TT,3) , pow(TT,4) , pow(TT,5),
             3*pow(TT,2), 4*pow(TT,3), 5*pow(TT,4),
             6*pow(TT,1), 12*pow(TT,2), 20*pow(TT,3);
        return A;
    };
    auto bb = [](double a_0, double a_1, double a_2, int TT, double offset){
        Eigen::Matrix<double,3,1> b;
        b << offset - a_0 - a_1*TT - a_2/2*pow(TT,2),
             0 - a_1 - a_2*TT,
             0 - a_2;
        return b;
    };
    //int T = _baseline * 10;
    //int T = 250;
    int T = 5;
    double a_0 = this->dataPtr->old_d[5];
    double a_1 = this->dataPtr->old_d[5] - this->dataPtr->old_d[4];
    int index = this->dataPtr->nearest_index;
    State state = this->vehicleState();
    //cout << _baseline/T << endl;
    a_1 = _baseline/T*tan(state.Yaw() - this->dataPtr->map_yaw[index]);
    //cout << "a_1 : " << a_1 << endl;
    //a_1 = 0;
    double a_2 = this->dataPtr->old_d[5]/2 + this->dataPtr->old_d[3]/2 - this->dataPtr->old_d[4];
    a_2 *= 0.01;
    //cout << "a_2 : " << a_2 << endl;
    //a_2 = 0.00001;
    a_2 = 0;
    Eigen::Matrix<double,3,3> A = AA(T);
    Eigen::Matrix<double,3,1> b = bb(a_0, a_1, a_2, T, _offset);
    auto aa = A.householderQr().solve(b);
    double a_3 = aa[0]; double a_4 = aa[1]; double a_5=aa[2];
    this->dataPtr->a[0] = a_0;
    this->dataPtr->a[1] = a_1;
    this->dataPtr->a[2] = a_2;
    this->dataPtr->a[3] = a_3;
    this->dataPtr->a[4] = a_4;
    this->dataPtr->a[5] = a_5;
    //vector<double> ds = this->calcD(T);
    //vector<double> ds = this->calcD(T,250);
    vector<double> ds = this->calcD(T,_baseline * 10);
    vector<double> ss = this->calcS(ds);
    calcXYs(ss,ds,_xs,_ys);
}

bool Frenet::done(){
    if(this->dataPtr->map_x.size() - this->dataPtr->nearest_index < 10){
        return true;
    }
    return false;
}

vector<double> Frenet::calcD(const int &_T, const int &_num){
    vector<double> ds; //ds.push_back(this->dataPtr->old_d[-1]);
    double a_0 = this->dataPtr->a[0];
    double a_1 = this->dataPtr->a[1];
    double a_2 = this->dataPtr->a[2];
    double a_3 = this->dataPtr->a[3];
    double a_4 = this->dataPtr->a[4];
    double a_5 = this->dataPtr->a[5];
    double j = _T*1.0/_num;
    for(int i=0; i<_num; i++){
        ds.push_back(a_0+a_1*i*j+a_2*i*i*j+a_3*pow(i*j,3)+a_4*pow(i*j,4)+a_5*pow(i*j,5));
    }
    return ds;
}


void Frenet::calcXYs(const vector<double> &_ss, const vector<double> &_ds, vector<double> &_xs, vector<double> &_ys){
    int keypoint = 0;
    for(int i=0; i<_ss.size(); i++){
        while(keypoint < this->dataPtr->map_s.size()-1 && this->dataPtr->map_s[keypoint+1]<_ss[i]){keypoint++;}
        double forward = _ss[i] - this->dataPtr->map_s[keypoint];
        double target_x = this->dataPtr->map_x[keypoint] + forward*sin(this->dataPtr->map_yaw[keypoint]);
        double target_y = this->dataPtr->map_y[keypoint] + forward*cos(this->dataPtr->map_yaw[keypoint]);
        double offset_x = _ds[i] * sin(this->dataPtr->map_yaw[keypoint]+asin(1));
        double offset_y = _ds[i] * cos(this->dataPtr->map_yaw[keypoint]+asin(1));
        _xs.push_back(target_x+offset_x);
        _ys.push_back(target_y+offset_y);
    }
}

vector<double> Frenet::calcS(const vector<double> &_ds){
    vector<double> ss; ss.push_back(this->dataPtr->old_s.back());
    for(int i=1; i<_ds.size(); i++){
        //ss.push_back(sqrt(0.01-pow(_ds[i]-_ds[i-1],2)) + ss.back());
        ss.push_back(0.1 + ss.back());
    }
    return ss;
}

vector<double> Frenet::calcD(const int &_T){
    vector<double> ds; //ds.push_back(this->dataPtr->old_d[-1]);
    double a_0 = this->dataPtr->a[0];
    double a_1 = this->dataPtr->a[1];
    double a_2 = this->dataPtr->a[2];
    double a_3 = this->dataPtr->a[3];
    double a_4 = this->dataPtr->a[4];
    double a_5 = this->dataPtr->a[5];
    
    for(int i=0; i<_T; i++){
        ds.push_back(a_0+a_1*i+a_2*i*i+a_3*pow(i,3)+a_4*pow(i,4)+a_5*pow(i,5));
    }
    return ds;
}

void Frenet::updateState(const State &_state){
    this->dataPtr->state = _state;
    this->xy2sd();
    //this->dataPtr->a[0] = this->dataPtr->old_d[5];
    //this->dataPtr->a[1] = this->dataPtr->old_d[5] - this->dataPtr->old_d[4];
    //this->dataPtr->a[2] = (this->dataPtr->old_d[5] - this->dataPtr->old_d[3])/2;
}

State Frenet::vehicleState() const{
    return this->dataPtr->state;
}

void Frenet::xy2sd(){
    State state = this->vehicleState();
    int index = -1; double dis = 999;
    for(int i=0; i<this->dataPtr->map_x.size();i++){
        double tmp = sqrt(pow(this->dataPtr->map_x[i]-state.X(),2) + 
                          pow(this->dataPtr->map_y[i]-state.Y(),2));
        if(dis > tmp){
            dis = tmp; index = i;
        }
    }
    this->dataPtr->nearest_index = index;
    double s = this->dataPtr->map_s[index];
    Eigen::Vector3d v(this->dataPtr->map_x[index]-this->dataPtr->map_x[index-1], this->dataPtr->map_y[index]-this->dataPtr->map_y[index-1],0);
    Eigen::Vector3d w(state.X()-this->dataPtr->map_x[index-1], state.Y()-this->dataPtr->map_y[index-1],0);
    double d = w.cross(v)[2]/v.norm();
    this->dataPtr->old_s.push_back(s);
    this->dataPtr->old_d.push_back(d);
    while(this->dataPtr->old_s.size()<6){this->dataPtr->old_s.push_back(s);}
    while(this->dataPtr->old_d.size()<6){this->dataPtr->old_d.push_back(d);}
    while(this->dataPtr->old_s.size()>6){this->dataPtr->old_s.pop_front();}
    while(this->dataPtr->old_d.size()>6){this->dataPtr->old_d.pop_front();}
}

void Frenet::updatePos(const double &_x, const double &_y, const double &_z){
}

Frenet::Frenet():dataPtr(new FrenetPrivate){
}

Frenet::Frenet(string mapfile):dataPtr(new FrenetPrivate){
    sqlite3 *db; char *zErrMsg = 0; int  rc; char *sql;
    rc = sqlite3_open(mapfile.c_str(), &db);
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return ;
    }else{
        fprintf(stdout, "Opened database successfully!\n");
    }

    sql = "SELECT name FROM sqlite_master WHERE type=\'table\'";
    rc = sqlite3_exec(db, sql, select_callback, this, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg); sqlite3_close(db);
        return ;
    }
    string tmpstr("SELECT * from " + this->dataPtr->table_name + " order by roadIndex");
    rc = sqlite3_exec(db, tmpstr.c_str(), select_callback, this, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg); sqlite3_close(db);
        return ;
    }
    sqlite3_close(db);
    this->calcS();
}

Frenet::~Frenet(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

void Frenet::calcS(){
    vector<double> x = this->dataPtr->map_x;
    vector<double> y = this->dataPtr->map_y;
    double sum = 0.0;
    this->appendS(sum);   
    for(int i=1; i<x.size() ;i++){
        sum += sqrt(pow(y[i]-y[i-1],2) + pow(x[i]-x[i-1],2));
        this->appendS(sum);   
    }
}

void Frenet::appendX(const double &_value){this->dataPtr->map_x.push_back(_value);}
void Frenet::appendY(const double &_value){this->dataPtr->map_y.push_back(_value);}
void Frenet::appendZ(const double &_value){this->dataPtr->map_z.push_back(_value);}
void Frenet::appendS(const double &_value){this->dataPtr->map_s.push_back(_value);}
void Frenet::appendYaw(const double &_value){this->dataPtr->map_yaw.push_back(_value);}

int Frenet::select_callback(void *pVoid, int argc, char **argv, char **azColName){
   Frenet* p = (Frenet*) pVoid;
   for(int i=0; i<argc; i++){
       if(string(azColName[i]) == "name"){
           p->dataPtr->table_name = argv[i];
           return 0;
       }
       if(string(azColName[i]) == "x"){ p->appendX(atof(argv[i])); continue; }
       if(string(azColName[i]) == "y"){ p->appendY(atof(argv[i])); continue; }
       if(string(azColName[i]) == "z"){ p->appendZ(atof(argv[i])); continue; }
       if(string(azColName[i]) == "heading"){ p->appendYaw(atof(argv[i]) * asin(1.0)/90); continue; }
   }
   return 0;
}

Frenet &Frenet::operator=(const Frenet &_frenet)
{
    this->dataPtr->map_x = _frenet.mapX() ;
    this->dataPtr->map_y = _frenet.mapY() ;
    this->dataPtr->map_yaw = _frenet.mapYaw();
    this->dataPtr->map_s = _frenet.mapS();

    this->dataPtr->state = _frenet.vehicleState(); 

    this->dataPtr->old_s = _frenet.oldS();
    this->dataPtr->old_d = _frenet.oldD();

    this->dataPtr->a = _frenet.A();
    this->dataPtr->nearest_index  = _frenet.dataPtr->nearest_index;

    return *this;
}

vector<double> Frenet::mapX() const{
    return this->dataPtr->map_x;
}
vector<double> Frenet::mapY() const{
    return this->dataPtr->map_y;
}
vector<double> Frenet::mapS() const{
    return this->dataPtr->map_s;
}
deque<double> Frenet::oldS() const{
    return this->dataPtr->old_s;
}
deque<double> Frenet::oldD() const{
    return this->dataPtr->old_d;
}
vector<double> Frenet::A() const{
    return this->dataPtr->a;
}
vector<double> Frenet::mapYaw() const{
    return this->dataPtr->map_yaw;
}

