#pragma once
#include "MyStructs.h"
#include "Clustering.h"
#include <vector>
#include <algorithm>
struct R_t2
{
	std::pair <int, int> c;
	//double z;
	double A;
	int num;

};

struct find_id : std::unary_function<R_t2, bool> {
	std::pair <int, int> c;
	find_id(std::pair <int, int> c) :c(c) { }
	bool operator()(R_t2 const& m) const {
		return m.c == c;
	}
};

class ProbMap {
private:
	std::vector<R_t2> target_list;
	int height, width;
	int grid_height, grid_width;
	double delta_x_, delta_y_;
	int targ_num;
	int mapWidth;
	vector<vector<double>> gridmap;
	double l0, k;


public:
	ProbMap(int M, int N, double delta_x, double delta_y);

	void fill_target_list(vector<R_t>R);	// calculates cells' detection probabilities

	void clean();							//cleans the target list

	void fill_grid();						// places the new targets onto grid

	void print(ofstream& myfile);

	inline double log_prob(R_t2 target); 

	vector<point> make_list_of_points();

};