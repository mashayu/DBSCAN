#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include "ProbMap.h"
using namespace std;
ProbMap::ProbMap(int M, int N, double delta_x, double delta_y): 
	gridmap(M / delta_x+1, vector<double>(N / delta_y +1)) {
		
		grid_height = M / delta_x + 1;
		grid_width = N / delta_y + 1;
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
				
			

			if (abs(temp_y - y_cell) > 0.5) {
				if (temp_y > 0)
					y_cell += 1;
				else
					y_cell -= 1;
			}
				
			y_cell += this->width / 2;
		
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

void ProbMap::print(ofstream& myfile){
	if (myfile.is_open()) {
		for (int i = 0; i < grid_height; i++) {
			for (int j = 0; j < grid_width; j++) {
				myfile << gridmap[i][j]<< " ";
			}
			myfile << endl;
		}
	}
	else cout << "Unable to open file";
}


inline double ProbMap::log_prob(R_t2 target) {  // TODO: what if A == 1?
	if (target.A == 1)
		target.A = 0.9999;
	return log(target.A / (1 - target.A));
}

void ProbMap::fill_grid() {
	vector<R_t2>::iterator it;
	double x, y;
	for (it = target_list.begin(); it != target_list.end(); ++it) {
		x = it->c.first;
		y = it->c.second;
		gridmap[x][y] = k * gridmap[x][y] + log_prob(*it) - l0;
		cout << gridmap[x][y]<<endl;
	}
}

vector<point> ProbMap::make_list_of_points() {
	vector <point> vect;
	for (int x = 0; x < grid_height; x++) {
		for (int y = 0; y < grid_width; y++) {
			if (gridmap[x][y] != 0) {
				vect.push_back(point(x*delta_x_, y*delta_y_, 0));
			}
		}
	}

	return vect;
}