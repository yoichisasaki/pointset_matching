//マッチング問題
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "rmsd.hpp"

using namespace std;
using namespace Eigen;

int find_main(vector<Point>& candi, 
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

    num_visit++;

  if (candi.size() == query.size()) {
    double rmsd = RMSD(candi, query); 
    if (RMSD(candi, query) <  r) {    
      num_match++;

      if (rmsd < min_rmsd)
	min_rmsd = rmsd; 
    }
  } else {
    for(size_t x = 0; x < dataseq.size(); x++) {

      //Check if x is not incluced in candi
      if (!is_contained_datapoint(x, candi, dataseq)) {
	candi.push_back(dataseq[x]);
	find(candi,  depth+1, dataseq, query, r);
	candi.pop_back();
      }
    }
  }

   return num_match; //合わせるため 
 }

//EOF

