#include "eqDriving/vehicle/State.hpp"
using namespace vehicle;

#include <deque>
#include <vector>
#include <iostream>
using namespace std;

#pragma once
namespace eqDriving{
  namespace planning{

    void calcPath(
         const vector<double> &_base_xs, 
         const vector<double> &_base_ys, 
         const vector<double> &_base_yaws,

         const double &_cur_x, const double &_cur_y, const double &_cur_yaw,
         const double &_offset, const double &_baseline,
         vector<double> &_path_xs,
         vector<double> &_path_ys
                  );

    class FrenetPrivate;
    class Frenet{
        public: Frenet();
        public: Frenet(string mapfile);
        public: ~Frenet();

        public: bool done();

        public: void calcPaths(const State &_state);
        public: void calcPath(const double &_offset, const double &_baseline, vector<double> &_xs, vector<double> &_ys);
        public: void calcXYs(const vector<double> &_ss, const vector<double> &_ds, vector<double> &_xs, vector<double> &_ys);

        public: void updatePos(const double &_x, const double &_y, const double &_z);
        public: void updateState(const State &_value);
        public: State vehicleState() const;

        public: vector<double> mapX() const;
        public: vector<double> mapY() const;
        public: vector<double> mapYaw() const;
        public: vector<double> mapS() const;

        public: deque<double> oldD() const;
        public: deque<double> oldS() const;
        public: vector<double> A() const;

        private: void appendX(const double &_value);
        private: void appendY(const double &_value);
        private: void appendZ(const double &_value);
        private: void appendS(const double &_value);
        private: void appendYaw(const double &_value);

        private: void xy2sd();

        private: vector<double> calcD(const int &_T);
        private: vector<double> calcD(const int &_T, const int &_num);
        private: vector<double> calcS(const vector<double> &_ds);
        private: void calcS();
        private: static int select_callback(void *pVoid, int argc, char **argv, char **azColName);

        private: FrenetPrivate *dataPtr = nullptr;
        public: Frenet&operator=(const Frenet &_frenet);
    };
  }
}
