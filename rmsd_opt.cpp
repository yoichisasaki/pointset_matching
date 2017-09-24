//マッチング問題
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "rmsd.hpp"

using namespace std;
using namespace Eigen;

#undef DEBUG_PRUNE
//#define DEBUG_PRUNE

double R_EPS = 0.001 * 0.001 * 0.001; //1/1000*1000*1000

//queryの最初のk個(len)の点のみ抽出
void copy_first_k(vector<Point>& query0, vector<Point>& query, int len) {
  query0.clear();
  for (int i = 0; i < len; i++) {
    query0.push_back(query[i]);
  }
}

int find_main(vector<Point>& candi, //マッチ候補点集合
	      vector<Point>& dataseq,   //データベース点集合
	      vector<Point>& query,      //パターン点集合
	      double r)                          //閾値
{
  min_rmsd = r;
//スタートは深さ０から
  return find(candi, 0, dataseq, query, r); 
}

int find(vector<Point>& candi, 
	 int depth, 
	 vector<Point>& dataseq, 
	 vector<Point>& query, 
	 double r)
{
    num_visit++;

    if (candi.size() == query.size()) { //マッチ候補がパターン点集合と同じ点の数になった
    double rmsd = RMSD(candi, query); 
    if (rmsd <  min_rmsd) {    
      num_match++;

      if (rmsd < min_rmsd){
	min_rmsd = rmsd; 
      }
    }
  } else {
    for(size_t x = 0; x < dataseq.size(); x++) {

      //Check if x is not incluced in candi
      if (!is_contained_datapoint(x, candi, dataseq)) {
	candi.push_back(dataseq[x]);

	//上限関数を用いた枝刈り
	vector<Point> query0;  
	copy_first_k(query0, query, candi.size()); //最初のlen個だけとる
	double rmsd = RMSD(candi, query0); 
	double ratio = ((double)query.size())/((double)candi.size()); 

	if (rmsd < sqrt(ratio) * (min_rmsd)) { 
	  find(candi,  depth+1, dataseq, query, r);
	}
	candi.pop_back();
      }
    }
  }
  return num_match; //合わせるため 
}

//EOF
