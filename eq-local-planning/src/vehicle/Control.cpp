#include "eqDriving/vehicle/Control.hpp"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace eqDriving;
using namespace vehicle;

namespace eqDriving{
namespace vehicle{
Vector3d purepursuit(const State &_state, const vector<Pose3d> &_poses, double(*pre_func)(double) ){
    Vector3d cmder;
    double l_d = pre_func(_state.Velocity().X());
    int target_point = target(_state, _poses, l_d);
    double alpha = atan2(_poses[target_point].Pos().X() - _state.X(), 
                         _poses[target_point].Pos().Y() - _state.Y())
                   - _state.Yaw();

    double delta = atan2(2.0 * _state.Wheelbase() * sin(alpha) / l_d, 1.0);
    cmder.X(-delta);
    return cmder;
}

int nearest(const State &_state, const vector<Pose3d> &_poses){
    int index = -1;
    double max_dis = 999999;
    for(int i=0;i<_poses.size();i++){
        double tmp = sqrt(pow(_poses[i].Pos().X() - _state.X(),2) + 
                          pow(_poses[i].Pos().Y() - _state.Y(),2));
        if(tmp < max_dis){max_dis = tmp; index = i;}
    }
    return index;
}

int target(const State &_state, const vector<Pose3d> &_poses, const double &_forsee){
    int index = -1;
    double max_dis = 999999;
    for(int i=0;i<_poses.size();i++){
        double tmp = sqrt(pow(_poses[i].Pos().X() - _state.X(),2) + 
                          pow(_poses[i].Pos().Y() - _state.Y(),2));
        if(tmp < max_dis){max_dis = tmp; index = i;}
    }
    double sum_dis = 0;
    for(int i=index+1; index<_poses.size() && index >0 && sum_dis < _forsee;i++){
        sum_dis+=sqrt(pow(_poses[i].Pos().X() - _poses[i-1].Pos().X(),2) + 
                      pow(_poses[i].Pos().Y() - _poses[i-1].Pos().Y(),2));
    }
    return index;
}

Vector3d purepursuit(const State &_state, const vector<double> &_xs, const vector<double> &_ys,  double(*pre_func)(double) ){
    Vector3d cmder;
    double l_d = pre_func(_state.Velocity().X());
    int target_point = target(_state, _xs, _ys, l_d);
    //cout << "target point is : " << target_point << endl;;
    double alpha = _state.Yaw() - atan2(_xs[target_point] - _state.X(), 
                         _ys[target_point] - _state.Y());
    //cout << "target y : " << _ys[target_point] << endl;
    //cout << "state y : " << _state.Y() << endl;
    //cout << _ys[target_point] - _state.Y() << endl;
    //cout << _ys[target_point] - _state.Y() << endl;
    //cout << "atan2 is : " << atan2(_xs[target_point] - _state.X(),_ys[target_point] - _state.Y()) * 90 / asin(1) << endl;
    //cout << "yaw is : " << _state.Yaw() * 90/asin(1) << endl;
    //cout << "alpha is :" << alpha<< endl;

    double delta = atan2(2.0 * _state.Wheelbase() * sin(alpha) / l_d, 1.0);
    //cout << "delta is :" << delta << endl;
    //delta *= 3.0;
    cmder.X(delta);
    return cmder;
}

int nearest(const State &_state,const vector<double> &_xs, const vector<double> &_ys){
    int index = -1;
    double max_dis = 999999;
    for(int i=0;i<_xs.size();i++){
        double tmp = sqrt(pow(_xs[i] - _state.X(),2) + 
                          pow(_ys[i] - _state.Y(),2));
        if(tmp < max_dis){max_dis = tmp; index = i;}
    }
    return index;
}

int target(const State &_state,const vector<double> &_xs, const vector<double> &_ys, const double &_forsee){
    int index = -1;
    double max_dis = 999999;
    for(int i=0;i<_xs.size();i++){
        double tmp = sqrt(pow(_xs[i]- _state.X(),2) + 
                          pow(_ys[i] - _state.Y(),2));
        if(tmp < max_dis){max_dis = tmp; index = i;}
    }
    //cout << "nearest index is : " << index << endl;
    //cout << "nearest dis is : " << max_dis << endl;
    //cout << "forsee dis is : " << _forsee << endl;
    double sum_dis = 0;
    for(index++; index<_xs.size() && index > 0 && sum_dis < _forsee;index++){
        sum_dis+=sqrt(pow(_xs[index] - _xs[index-1],2) + 
                      pow(_ys[index] - _ys[index-1],2));

    }
    //cout << "sum dis is : " << sum_dis << endl;
    return index;
}

}
}
