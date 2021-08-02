#ifndef PUBFUNC_H
#define PUBFUNC_H

#include<QList>
#include<QByteArray>
#include"typedefs.h"
#include<math.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits.h>
using namespace std;

#define PI 3.1415926535897932384626433832795
#define deltaTime 0.05
#define MaxValue numeric_limits<double>::max()

extern QList<uint8_t> reConstruct(QByteArray lineData);
extern QList<uint8_t> reConstruct1(QByteArray lineData);
extern QList<MAP_POINT> reConstruct2(QByteArray lineData);
extern uint8_t hex2dec(char hexNum);
extern double GetLOInDegree(double dLIn);
extern POINT3D BLH2XYZ(double B, double L, double H);
extern bool Ins2AHRCheckSum(byte data[], int index);
extern INS_OPVT2AHR_DATA Ins2AHRDecode(byte data[], int index);
extern QList<INS_OPVT2AHR_DATA> getInsDecodeData(QList<uint8_t> templist);
extern QList<MAP_POINT> getMapFormat(QList<INS_OPVT2AHR_DATA> srcData);
extern QList<MAP_POINT> filtratePoint(QList<MAP_POINT> srcData);
extern double get2Dis(POINT2D p0,POINT2D p1);
extern double get2Dis(POINT3D p0, POINT3D p1);
extern double get3Dis(POINT3D p0, POINT3D p1);
extern POINT2D Coordinate_TransfertoVehicle(POINT2D srcPoint,INS_OPVT2AHR_DATA insData);
extern POINT3D Coordinate_TransfertoWorld(POINT3D srcPoint,INS_OPVT2AHR_DATA insData);
extern int getNearestPointIndexInGround(QList<MAP_POINT> mapList,INS_OPVT2AHR_DATA pos,int lastIndex);
extern int getNearestPointIndexInVehicle(QList<MAP_POINT> mapList,int lastIndex);
extern int calcDisOnPath(QList<TRACK_POINT> path,int startIndex,double dis);
extern double sum(vector<double> Vnum, int n);
extern double MutilSum(vector<double> Vx, vector<double> Vy, int n);
extern double RelatePow(vector<double> Vx, int n, int ex);
extern double RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex);
extern void EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[]);
extern void CalEquation(int exp, double coefficient[]);
extern double calcCurvature(POINT3D p0, POINT3D p1, POINT3D p2);
extern double F(double c[],int l,int m);
extern double Em[10][10];
extern double Max(double a,double b);
extern double Min(double a,double b);
extern float average(QList<float> data);




#endif // PUBFUNC_H
