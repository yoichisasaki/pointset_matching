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

void copy_first_k(vector<Point>& query0, vector<Point>& query, int len) {
  query0.clear();
  for (int i = 0; i < len; i++) {
    query0.push_back(query[i]);
  }
}


/*** リストにデータを登録 ***/
void registrate(vector<Point> dataseq){
  double l = compute_radius(vector<Point> dataseq);
  Point c = compute_centroid(vector<Point>& dataseq, int dataseq.size);
  double xs = dataseq.getx() - l;
  double xg = dataseq.getx() + l ;
  double ys = dataseq.gety() - l;
  double yg = dataseq.gety() + l;
  double zs = dataseq.getz() - l;
  double zg = dataseq.getz() + l;
  double delta = (2 * l) / 100
	/* リストにデータを登録 */
  for(int i = 0; i < dataseq.size; i++){
    double x = detaseq[i].getx;
    double y = detaseq[i].gety;
    double z = detaseq[i].getz;

    int xid = (int) (x - xs)/delta ;
    int yid = (int) (y - ys)/delta ;
    int zid = (int) (z - zs)/delta ;
    vector<Point>bucket[xid][yid][zid].push_back(dataseq[i]) ;
  }
}



int find_main(vector<Point>& candi, vector<Point>& dataseq, vector<Point>& query, double r){
  double L = 2* (sqrt((double)query.size()) * r + compute_radius(query));
  L = L * L;
  printf("L=%f\n", L);

  double l = compute_radius(vector<Point> dataseq);
  double xs = dataseq.getx() - l;
  double ys = dataseq.gety() - l;
  double zs = dataseq.getz() - l;
  double delta = (2 * l) / 100

  for (int i = 0; i < dataseq.size; i++) {
    double xmax = dataseq[i].getx + L;
    double xmin = dataseq[i].getx - L;
    double ymax = dataseq[i].gety + L;
    double ymin = dataseq[i].gety - L;
    double zmax = dataseq[i].getz + L;
    double zmin = dataseq[i].getz - L;
    int xmaxid = (int) (xmax - xs)/delta ;
    int xminid = (int) (xmin - xs)/delta ;
    int ymaxid = (int) (ymax - ys)/delta ;
    int yminid = (int) (ymin - ys)/delta ;
    int zmaxid = (int) (zmax - zs)/delta ;
    int xmaxid = (int) (zmin - zs)/delta ;
    
    for(int x = xminid; x < xmaxid; x++){
      for(int y = yminid; y < ymaxid; y++){
	for(int z = zminid; z < zmaxid; z++){
	  for(int j = 0; j < backet[x][y][z].size; j++){
	    vector<Point> D.push_back(bucket[x][y][z][j])
	      }
	}
      }
    }
 find_main2(vector<Point>& candi, vector<Point>& D, vector<Point>& query, double r)
    D.clear(); 
}
}

int find_main2(vector<Point>& candi, 
	 vector<Point>& dataseq, 
	 vector<Point>& query, 
	 double r)
{
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

	if (rmsd < sqrt(ratio) * r) { 
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





//予備
/*


int find_main(vector<Point>& candi, 
	      vector<Point>& dataseq, 
	      vector<Point>& query, 
	      double r)
{
  double L = 2* (sqrt((double)query.size()) * r + compute_radius(query));
  L = L * L;
  printf("L=%f\n", L);
  return find_FPT(candi, 0, dataseq, query, r, L);
}

int find_FPT(vector<Point>& candi, 
	     int depth, 
	     vector<Point>& dataseq, 
	     vector<Point>& query, 
	     double r,
	     double L)
{   
num_visit++;
#ifdef DEBUG_PRUNE
  printf("@enter find: len(candi)=%d, len(pat)=%d, r=%f\n", 
	 (int)candi.size(), (int)query.size(), r); 
#endif 
  //  printf("depth:%d\n", depth);
  if (candi.size() == query.size()) {
    double rmsd = RMSD(candi, query); 
    if (rmsd <  r) {    
      //cout << "found!" << endl;    
      num_match++;
      if (rmsd < min_rmsd) min_rmsd = rmsd; 
    }
    return num_match;
  } else {
    for(int x = 0; x < dataseq.size(); x++) {
      //Check if x is not incluced in candi
      if (!is_contained_datapoint(x, candi, dataseq)) {
     	//new 範囲内か?

	candi.push_back(dataseq[x]);  
	//  printf("x:%d\n", x);
	//上限関数を用いた枝刈り
	if (dist3(candi[0], dataseq[x]) <= L) {
	  vector<Point> query0;  
	  copy_first_k(query0, query, candi.size()); //最初のlen個だけとる
	  double rmsd = RMSD(candi, query0); 
	  double ratio = ((double)query.size())/((double)candi.size()); 
	  //double maxr = sqrt(ratio) * r; 
	  if (rmsd <  sqrt(ratio) * r) { 
	    find_FPT(candi,  depth+1, dataseq, query, r, L);
	    	  }
	}
	candi.pop_back();  //Lより大きいときはここまで飛んでpop

      }
    }
    return num_match;  
  }
}



//EOF
*/
