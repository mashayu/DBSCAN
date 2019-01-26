#include <iostream>
#include <math.h>
#include <vector>
#include "ProbMap.h"
using namespace std;
ProbMap::ProbMap(int M, int N, double delta_x, double delta_y): 
	gridmap(M / delta_x+1, vector<double>(N / delta_y +1)) {

		height = M; 
		width = N;
		delta_x_ = delta_x;
		delta_y_ = delta_y;
		k = 1;
	}

void ProbMap::fill_target_list(vector<R_t> targs) {
	vector<R_t>::iterator it;
	vector<R_t2>::iterator it2;

	for (it = targs.begin(); it != targs.end(); it++) {
			double temp_x = it->x / delta_x_;
			double temp_y = it->y / delta_y_;
			int x_cell = int(temp_x);
			int y_cell = int(temp_y);

			if (temp_x - x_cell > 0.5)
				x_cell += 1;

			if (temp_y - y_cell > 0.5)
				y_cell += 1;
			
			// look for the targets that contribute to one cell
			it2 = std::find_if(target_list.begin(), target_list.end(), 
				find_id(make_pair(x_cell, y_cell)));
			if (it2 != target_list.end()) {		//was found
				it2->A += it->A;
				it2->num += 1;
			}
			else {								//was not found
				target_list.push_back(R_t2{ make_pair(x_cell, y_cell), it->A, 1 });
			}

		}

		// calculate mean values for each cell
		for (it2 = target_list.begin(); it2 != target_list.end(); ++it2) {
			it2->A = it2->A / it2->num;
		}	
	}

void ProbMap::clean() {
	target_list.clear();
}

inline double ProbMap::log_prob(R_t2 target) {
	return log(target.A / (1 - target.A));
}

void ProbMap::fill_grid() {
	vector<R_t2>::iterator it;
	double x, y;
	for (it = target_list.begin(); it != target_list.end(); ++it) {
		x = it->c.first;
		y = it->c.second;
		gridmap[x][y] = k * gridmap[x][y] + log_prob(*it) - l0;
	}
}