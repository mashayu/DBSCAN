#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include "Funcs.h"
using namespace std;

void scaling(vector <R_t>&R) {
	vector<R_t>::iterator it;
	for (it = R.begin(); it != R.end(); it++) {
		it->A = 0.5 + 0.5*it->A;
	}
}

void compensation(vector <R_t>&R, float d_N) {

	vector<R_t>::iterator it;
	for (it = R.begin(); it != R.end(); it++){
		double d = sqrt(pow(it->x, 2) + pow(it->y, 2));
		if (d == 0)												// ???????
			d = 0.001;
		it->A = it->A - 40 * log10(d / d_N);
	}
}

void normalization(vector <R_t> &R) {  // TODO: check for negative values
	vector<R_t>::iterator it;
	std::sort(R.begin(), R.end());
	double min_20 = 0.1*(R.end()-1)->A;
	double max_20 = 0.9*(R.end() - 1)->A;

	for (it = R.begin(); it != R.end(); it++) {
		if (it->A < min_20) {
			it->A = min_20;
		}
		else
			break;
	}
	vector<R_t>::reverse_iterator riter;

	for (riter = R.rbegin(); riter != R.rend(); ++riter) {
		if (riter->A > max_20) {
			riter->A = max_20;
		}
		else 
			break;
	}

	for (it = R.begin(); it != R.end(); it++) {
		it->A = (it->A - min_20) / (max_20 - min_20);
	}

}


pair<double, double> get_vehicle_coords(pair<double, double> coords) {  //TODO: make realistic

	/* code for coords */ 

	double x = coords.first, y = coords.second;
	return make_pair(x+0.1, y+0.1);
}

void update_coord(vector <R_t> &targets, pair<double, double> coord) {  // TODO: make realistic

	vector<R_t>::iterator it;
	for (it = targets.begin(); it != targets.end(); it++) {
		it->x += coord.first;
		it->y += coord.second;
	}

}
