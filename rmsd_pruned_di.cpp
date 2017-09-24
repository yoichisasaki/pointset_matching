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
#define DEBUG_DIAM

double R_EPS = 0.001 * 0.001 * 0.001; //1/1000*1000*1000

void copy_first_k(vector<Point>& query0, vector<Point>& query, int len) {
  query0.clear();
  for (int i = 0; i < len; i++) {
    query0.push_back(query[i]);
  }
}


//#define DEBUG_RADIUS
//データ点でもっとも距離が遠い対を求める．
void compute_farthest_pair(vector<Point> dataseq, 
			   int *i0, int *j0, double *diam) {
  double dist = -1;  //center
  for (int i = 0; i < dataseq.size(); i++) {
    Point p = dataseq[i]; 
    for (int j = i+1; j < dataseq.size(); j++) {
      Point q = dataseq[j]; 
      double d = dist2(p, q); 
      if (d > dist) { 
	dist = d; 
	*i0 = i; 
	*j0 = j; 
      }
    }
    *diam = dist; 
  }
#ifdef DEBUG_DIAM
    printf("debug: compute_fathest_pair: pid=%d, qid=%d, diameter=%f\n", 
	   *i0, *j0, *diam); 
#endif
}

void swap(vector<Point> &seq, int x, int y) {
  assert(x < seq.size());
  Point p = seq[x]; 
  seq[x] = seq[y]; 
  seq[y] = p; 
}

int find_main(vector<Point>& candi, 
	 vector<Point>& dataseq, 
	 vector<Point>& query, 
	 double r)
{
  int i0, j0;  //距離がもっとも遠い点の対の添え字
  double dist; 

  //互いの間の距離がもっとも遠い点の対を求める
  compute_farthest_pair(query, &i0, &j0, &dist);
  assert(i0 != j0);
  cout 
    << " i0=" << i0 
    << " j0=" << j0 
    << endl; 
  //二つの点を１番目と２番目にもってくる
  //int len = query.size();
  //cout  << " pi0=" << query[i0] << " pj0=" << query[j0]  << endl; 
  swap(query, 0, i0);
  swap(query, 1, j0);
  //  cout  << " pi0=" << query[i0] << " pj0=" << query[j0]  << endl; 
  //  cout  << " p0=" << query[0] << " p1=" << query[1]  << endl; 
  // compute_farthest_pair(query, &i0, &j0, &dist);

  return find(candi, 0, dataseq, query, r);
}

int find(vector<Point>& candi, 
	 int depth, 
	 vector<Point>& dataseq, 
	 vector<Point>& query, 
	 double r)
{
#ifdef DEBUG_PRUNE
  printf("@enter find: len(candi)=%d, len(pat)=%d, r=%f\n", 
	 (int)candi.size(), (int)query.size(), r); 
#endif 

    num_visit++;

  if (candi.size() == query.size()) {
    double rmsd = RMSD(candi, query); 
    if (rmsd <  r) {    
      //cout << "found!" << endl;    
      num_match++;

      if (rmsd < min_rmsd)
	min_rmsd = rmsd; 
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
	//double maxr = sqrt(ratio) * r; 

	if (rmsd <  sqrt(ratio) * r) { 
	  find(candi,  depth+1, dataseq, query, r);
	}
	// if (rmsd < (ratio * r)) { 
	//   find(candi,  depth+1, dataseq, query, r);
	// }
	//////////////////
	candi.pop_back();
      }//if (flag)
    }//for
  }//if

  return num_match; //合わせるため 
}

//EOF
