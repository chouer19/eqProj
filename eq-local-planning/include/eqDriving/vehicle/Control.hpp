#pragma once

#include <vector>
#include <iostream>
#include "eqDriving/math/Vector3.hpp"
#include "eqDriving/math/Quaternion.hpp"
#include "eqDriving/math/Pose3.hpp"
#include "eqDriving/vehicle/State.hpp"

using namespace std;
using namespace eqDriving;
using namespace math;
namespace eqDriving
{
  namespace vehicle
  {
      Vector3d purepursuit(const State &_state, const vector<Pose3d> &_poses, double(*pre_func)(double) );
      int nearest(const State &_state, const vector<Pose3d> &_poses);
      int target(const State &_state, const vector<Pose3d> &_poses, const double &_forsee);
      Vector3d purepursuit(const State &_state, const vector<double> &_xs, const vector<double> &_ys, double(*pre_func)(double) );
      int nearest(const State &_state,const vector<double> &_xs, const vector<double> &_ys);
      int target(const State &_state,const vector<double> &_xs, const vector<double> &_ys, const double &_forsee);
  }
}
