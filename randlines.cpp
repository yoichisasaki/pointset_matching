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

string USAGE_STRING("Usage: prog textfile patternfile "); 


//PDBデータファイルの内容を読み込む
//炭素原子CAの行のみとりだす。これは1カラム目はATOMで、3カラむ目がCA。
//PDBデータでは、行の６、７、８カラム目がX, Y, Z座標。
int pdb_dataseq_read(vector<string> &lineseq,  ifstream &ifs1) {
  string buf1, check1;
  int n = 0;
  while(getline(ifs1, buf1)) {
    lineseq.push_back(buf1);
    n++;
  }
  return n; //読んだ行数 
} //dataseq_read

int main(int argc, char** argv)
{
  ///
  string textfile = "data/testpdb1.txt"; 
  string patfile = "data/pat1L10.txt"; 
  int k; 
  double r = 0.1;  //デフォールト値

  /// 引数を読む
  if (argc < 3 || argc > 4) {
    cerr << "please input argument!!" << endl;
    cerr << USAGE_STRING << endl; 
    exit(1); 
  }
  if (argc >= 3) {
    textfile = argv[1];
    k = atoi(argv[2]);
  } 

#ifdef DEBUG
  cerr 
    << "#print args" << endl 
    << " textfile: " << textfile << endl 
    << " k: " << k << endl ; 
#endif
 
  // file open -> database
  ifstream textfs(textfile);
  if(textfs.fail()) {
    cerr << "error: text file do not exist!: " << textfile << endl; 
    cerr << USAGE_STRING << endl; 
    exit(0);
  }

  //データファイルの内容を読み込む
  vector<string> dataseq;
  string buf; 
  int n = 0;
  while(getline(textfs, buf)) {
    dataseq.push_back(buf);
    n++;
  }

    cerr <<  "@randpat: num sampled lines" << n  << "\t" << endl; 
    //    srand();
    assert(k <= n);

    ///bin/
    unsigned SEED = (unsigned) time(NULL); 
    srand(SEED); 

  //dataseqからランダムにk個の要素を出力する．
  int len = n; 
  int num_sampled = 0; 
  for (int i = 0; i < k; i++) {
    //cout << dataseq[i] << endl;  //debug

    //sample random number in [0..len-1]
//行を出力する
    int x = rand() % len;  
    string line = dataseq[x]; 
    dataseq[x] = dataseq[len-1]; //swap
    len = len - 1;
#ifdef DEBUG
    cerr <<  "@sample " << i <<": " << x  << endl;  //for debug
#endif
    cout <<  line << endl; 
    num_sampled++; 
  }
  cerr <<  "@randpat: number of sampled lines: " << num_sampled  << endl;  //for debug
}






