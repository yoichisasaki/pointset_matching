#ifndef ___RMSD_NAIVE___
#define ___RMSD_NAIVE___

#include <iostream>
#include <vector>
#include <math.h>
#include <assert.h>
#include <limits>
#include "Eigen/SVD"

using namespace std;

enum COOR{
  X=0,
  Y,
  Z
}; //class COOR

class Point{
  int id;
  double x, y, z;
public:
  Point(int idt, double xt, double yt, double zt)
    :id(idt),x(xt),y(yt),z(zt){};
  int getid(){return id;}
  double getx(){return x;}
  double gety(){return y;}
  double getz(){return z;}
  void print(){cout << "("
                    << x << ",  "
                    << y << ",  "
                    << z
                    << ") " << endl;}

  bool equals(const Point &a) const {
    if(x == a.x && y == a.y && z == a.z)
      return true ;
    else
      return false;
  }

  bool operator==(const Point &a)const{
    if(x == a.x && y == a.y && z == a.z)
      return true ;
    else
      return false;
  }
}; //class Point
  
extern int num_match; //見つけたマッチの回数 
extern int num_visit; //候補の訪問回数
extern double min_rmsd; //候補の訪問回数

extern bool is_contained_datapoint(int x, vector<Point>& candi, vector<Point>& dataseq); 

extern double RMSD(vector<Point>& s, vector<Point>& t);  

extern  int find_main(vector<Point>& candi, 
		 vector<Point>& dataseq, 
		 vector<Point>& query, 
		 double r); 

extern  int find(vector<Point>& candi, 
		 int depth, 
		 vector<Point>& dataseq, 
		 vector<Point>& query, 
		 double r); 

extern  int find_FPT(vector<Point>& candi, 
		 int depth, 
		 vector<Point>& dataseq, 
		 vector<Point>& query, 
		 double r,
		 double L); 
// extern void find2(vector<Point>& candi,
// 	  int i, 
// 	  vector<Point>& dataseq,
// 	  vector<Point>& query, double r);

extern double dist2(Point p, Point q);

extern double dist3(Point p, Point q);

//extern  vector<Point> bucket[int xid][int yid][int zid];

extern double compute_radius(vector<Point> dataseq);

extern Point compute_centroid(vector<Point> dataseq);
#endif







