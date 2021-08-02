#include "eqDriving/vehicle/State.hpp"
#include <vector>

using namespace eqDriving;
using namespace vehicle;
// Private data for the Material class
class eqDriving::vehicle::StatePrivate
{
  public: Vector3d llh_p;// latitude, longitude, height
  public: Vector3d xyz_p;// x, y, z
  public: Quaterniond rot;
  public: double yaw;
  public: int zone;
  public: bool north;//
  public: Vector3d velocity;// 
  public: Vector3d acceleration;// 
  public: double wheelbase = 6;// zhouju
  public: double track = 3; // lunju
};

State::State():dataPtr(new StatePrivate){
}

State::State(const double &_latitude, const double &_longitude, const double &_height, const double &_yaw, const double &_roll, const double &_pitch):dataPtr(new StatePrivate){

} 

State::State(const Vector3d &_xyzp, const int &_zone, const bool &_north):dataPtr(new StatePrivate){
}

State::State(const Vector3d &_xyzp, const Vector3d &_origin):dataPtr(new StatePrivate){
}

State::~State(){
  delete this->dataPtr;
  this->dataPtr = nullptr;
}

State::State(const State &_point):dataPtr(new StatePrivate)
{
    this->dataPtr->llh_p = _point.LLHPos();
    this->dataPtr->xyz_p = _point.XYZPos();
    this->dataPtr->rot = _point.Rot();
    this->dataPtr->zone = _point.Zone();
    this->dataPtr->north = _point.IsNorth();
    this->dataPtr->yaw = _point.Yaw();
}

void State::Update(const Vector3d &_cmder){
    this->X(this->X() + this->Velocity().X() * sin(this->Yaw()) * 0.1);
    this->Y(this->Y() + this->Velocity().X() * cos(this->Yaw()) * 0.1);
    //cout <<  "yaw co : " << this->Velocity().X() / this->Wheelbase() << endl;
    //cout <<  "cmd : " << _cmder.X() / this->Wheelbase() << endl;
    //cout << "tan delta : " << tan(_cmder.X()) << endl;
    this->Yaw(this->Yaw() - this->Velocity().X() / this->Wheelbase() * tan(_cmder.X()) * 0.1);
    //cout << this->Yaw() << endl;
    //state.v = state.v + a * dt
    //state.rear_x = state.x - ((L / 2) * math.cos(state.yaw))
    //state.rear_y = state.y - ((L / 2) * math.sin(state.yaw))
}

////////////////////////////////////////////
// get methods
double State::Latitude() const{ return this->dataPtr->llh_p.X();}
double State::Longitude() const{ return this->dataPtr->llh_p.Y();}
double State::Height() const{ return this->dataPtr->llh_p.Z();}
Vector3d State::LLHPos() const{ return this->dataPtr->llh_p;}
double State::X() const{ return this->dataPtr->xyz_p.X();}
double State::Y() const{ return this->dataPtr->xyz_p.Y();}
double State::Z() const{ return this->dataPtr->xyz_p.Z();}
Vector3d State::XYZPos() const{ return this->dataPtr->xyz_p;}
int State::Zone() const {return this->dataPtr->zone;}
bool State::IsNorth() const{return this->dataPtr->north;}
//double State::Yaw() const{return this->dataPtr->rot.Yaw();}
double State::Yaw() const{return this->dataPtr->yaw;}
double State::Roll() const{return this->dataPtr->rot.Roll();}
double State::Pitch() const{return this->dataPtr->rot.Pitch();}
Quaterniond State::Rot() const{return this->dataPtr->rot;}
Vector3d State::Velocity() const{ return this->dataPtr->velocity;}
Vector3d State::Acceleration() const{ return this->dataPtr->acceleration;}
double State::Wheelbase() const{return this->dataPtr->wheelbase;}
double State::Track() const{return this->dataPtr->track;}

void State::X(const double &_value) { this->dataPtr->xyz_p.X(_value);}
void State::Y(const double &_value) { this->dataPtr->xyz_p.Y(_value);}
void State::Z(const double &_value) { this->dataPtr->xyz_p.Z(_value);}
//void State::Yaw(const double &_value) { this->dataPtr->rot.Euler(0,0,_value);}
void State::Yaw(const double &_value) { this->dataPtr->yaw = _value;}
void State::Latitude(const double &_lat) { this->dataPtr->llh_p.X(_lat);}
void State::Longitude(const double &_lon){ this->dataPtr->llh_p.Y(_lon);}
void State::Height(const double &_height){ this->dataPtr->llh_p.Z(_height);}
void State::LLHPos(const double &_lat, const double &_lon, const double &_height){}
void State::XYZPos(const double &_x, const double &_y, const double &_z){
    this->dataPtr->xyz_p.Set(_x,_y,_z);
}
void State::Velocity(const double &_x, const double &_y, const double &_z){
    this->dataPtr->velocity.Set(_x,_y,_z);
}
void State::Acceleration(const double &_x, const double &_y, const double &_z){
    this->dataPtr->acceleration.Set(_x, _y, _z);
}
void State::Rot(const double &_roll, const double &_pitch, const double &_yaw){
    this->dataPtr->rot.Euler(_roll, _pitch, _yaw);
}

////////////////////////////////////////////
// set methods


///////////////////////////////
// clear methods
///////////////////////////////
State &State::operator=(const State &_point)
{
    this->dataPtr->llh_p = _point.LLHPos();
    this->dataPtr->xyz_p = _point.XYZPos();
    this->dataPtr->rot = _point.Rot();
    this->dataPtr->zone = _point.Zone();
    this->dataPtr->north = _point.IsNorth();
    this->dataPtr->velocity = _point.Velocity();// 
    this->dataPtr->acceleration = _point.Acceleration();// 
    this->dataPtr->wheelbase = _point.Wheelbase();// zhouju
    this->dataPtr->track = _point.Track(); // lunju
    this->dataPtr->yaw = _point.Yaw(); // lunju
    return *this;
}

