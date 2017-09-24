#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <math.h>
#include "Eigen/SVD"
#include "rmsd.hpp"

using namespace std;
using namespace Eigen;

//#define DEBUG_MAIN
//#define DEBUG_RADIUS
#define REPORT_RADIUS

double gettimeofday_sec(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec+tv.tv_usec*1e-6 ; 
}

string USAGE_STRING("Usage: prog textfile "); 


//PDBデータファイルの内容を読み込む
//炭素原子CAの行のみとりだす。これは1カラム目はATOMで、3カラむ目がCA。
//PDBデータでは、行の６、７、８カラム目がX, Y, Z座標。
int pdb_dataseq_read(vector<Point> &pointseq,  ifstream &ifs1) {
  string buf1, check1;
  int n = 0;
  while(getline(ifs1, buf1)) {
    if (buf1.find("ATOM") != string::npos ) {
      // if (buf1.find("ATOM") != string::npos &&
      //     buf1.find("CA") != string::npos) {
      //CAの炭素原子の行だけ取り出す

      vector<string> splited;
      string token;
      istringstream line(buf1);
      while(line >> token) {
        splited.push_back(token);
      }      
      double x = atof(splited[6].c_str());
      double y = atof(splited[7].c_str());
      double z = atof(splited[8].c_str());
      Point p(n, x, y, z);
      pointseq.push_back(p);
      n++;
    }
  }
  return n; //読んだ行数 
} //dataseq_read



int main(int argc, char** argv)
{
  ///
  string textfile = "data/testpdb1.txt"; 
  string patfile = "data/pat1L10.txt"; 
  double r = 0.1;  //デフォールト値

  /// 引数を読む
  if (argc != 2) {
    cerr << "please input argument!!" << endl;
    cerr << USAGE_STRING << endl; 
    exit(1); 
  }
#ifdef DEBUG_MAIN
  printf("debug: argc=%d", argc);
#endif
  if (argc >= 2) {
    textfile = argv[1];
  } 

#ifdef DEBUG_MAIN
  cerr 
    << "#print args" << endl 
    << " textfile: " << textfile << endl 
    << " patfile: " << patfile << endl 
    << " r: " << r << endl; 
#endif
 
  // file open -> database
  ifstream textfs(textfile);
  if(textfs.fail()) {
    cerr << "error: text file do not exist!: " << textfile << endl; 
    cerr << USAGE_STRING << endl; 
    exit(0);
  }

  //データファイルの内容を読み込む
  vector<Point> dataseq;
  pdb_dataseq_read(dataseq, textfs); 

  //データの中心の番号を求める．
  int center;  //center
  double radius; 
  compute_radius(dataseq);

  //データの中心からの距離を測

  Point c = dataseq[center]; 
  //中心cからのデータ点の距離を求める．
  for (int j = 0; j < dataseq.size(); j++) {
    Point q = dataseq[j]; 
    double d = dist2(c, q); 
    printf("%d %f\n", j, d);
  }

  //平均原子間距離
  Point q0 = dataseq[0];
  double dsum = 0.0;
  int csum = 0; 
  for (int j = 1; j < dataseq.size(); j++) {
    Point q = dataseq[j]; 
    dsum += dist2(q, q0);
    csum += 1; 
    q0 = q; 
  }
  double dmean = dsum / (double)csum; 

#ifdef REPORT_RADIUS
  fprintf(stderr, "@datadist: @input : number of atoms        :\t%d (atoms)\n", (int)dataseq.size()); 
  fprintf(stderr, "@datadist: @result: mean dist between atoms:\t%f (A=0.1nm)\n", dmean); 
  fprintf(stderr, "@datadist: @result: radius=%f (A=0.1nm), ceter=%d\n", radius, center); 
#endif
}  









