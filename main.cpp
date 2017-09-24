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

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec+tv.tv_usec*1e-6 ; 
};

string USAGE_STRING("Usage: prog textfile patternfile r"); 


//PDBデータファイルの内容を読み込む
//炭素原子CAの行のみとりだす。これは1カラム目はATOMで、3カラむ目がCA。
//PDBデータでは、行の６、７、８カラム目がX, Y, Z座標。
int pdb_dataseq_read(vector<Point> &pointseq,  ifstream &ifs1) {
  string buf1, check1;
  int n = 0;
  while(getline(ifs1, buf1)) {
    if (buf1.find("ATOM") != string::npos &&
        buf1.find("CA") != string::npos) {
      //CAの炭素原子の行だけ取り出す

      vector<string> splited;
      string token;
      istringstream line(buf1);
      while(line >> token) {
        splited.push_back(token);
      }      
      //PDBデータでは、行の６、７、８カラム目がX, Y, Z座標。
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
  double r = 0.1;  //デフォルト値

  /// 引数を読む
  if (argc < 3 || argc > 4) {
    cerr << "@apsmatch: please input argument!!" << endl;
    cerr << USAGE_STRING << endl; 
    exit(1); 
  }
  if (argc >= 3) {
    textfile = argv[1];
    patfile = argv[2];
  } 
  if (argc >= 4) {
    r = atof(argv[3]);
  } 

  cout 
    << "@apsmatch: "
    << " textfile: " << textfile 
    << "; " << " patfile: " << patfile 
    << "; " << " r: " << r << endl; 
  // file open textfile(dataseq)
  ifstream textfs(textfile);
  if(textfs.fail()) {
    cerr << "@apsmatch: error: text file do not exist!: " << textfile << endl; 
    cerr << USAGE_STRING << endl; 
    exit(0);
  }

  //データファイルの内容を読み込む
  vector<Point> dataseq;
  pdb_dataseq_read(dataseq, textfs); 
  cout << "@apsmatch: num data points: " << dataseq.size() << endl; 


  // file open patfile(query)
  ifstream patfs(patfile);
  if(patfs.fail()) {
    cerr << "@apsmatch: error: pattern file do not exist!: " << patfile << endl; 
    cerr << USAGE_STRING << endl; 
    exit(0);
  }

  //パターンファイルの内容を読み込む
  vector<Point> query;
  pdb_dataseq_read(query, patfs); 
  cout << "@apsmatch: num patt points: " << query.size() << endl; 


  //パターンマッチを行う
  double t1 = gettimeofday_sec(); 
  vector<Point> candi; //作業用の点列
  int count = find_main(candi, dataseq, query, r); //見つけたマッチ数

  cout 
     << "@apsmatch: @stat running time: "
     << (gettimeofday_sec() - t1) 
     << " (sec)"
     << endl;
  cout 
       << "@apsmatch: @stat #match=" << num_match 
       << "\t#visit=" 
       <<  num_visit
       << endl;
cout 
     << "@apsmatch: @stat min rmsd=" 
    << min_rmsd 
    << "\tr=" 
    << r 
    << endl;
}






