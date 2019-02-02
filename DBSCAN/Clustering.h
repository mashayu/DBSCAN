#pragma once
#include <vector>
using namespace std;
class point {
public:
	float x;
	float y;
	int cluster = 0;
	int pointType = 1;//1 noise 2 border 3 core
	int pts = 0;//points in MinPts 
	vector<int> corepts;
	int visited = 0;
	point() {}

	point(float a, float b, int c = 1){
		x = a;
		y = b;
		cluster = c;
	}
};

void clustering(vector<point> dataset, float Eps, int MinPts);

vector<point> openFile(const char* dataset);