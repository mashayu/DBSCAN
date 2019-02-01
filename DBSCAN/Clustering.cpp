//#define _CRT_SECURE_NO_DEPRECATE
//#include <stdio.h>
//#include <iostream>
//#include "dbscan.h"
//
//#define MINIMUM_POINTS 4     // minimum number of cluster
//#define EPSILON (0.75*0.75)  // distance for clustering, metre^2
//
//void readBenchmarkData(vector<Point>& points)
//{
//	// load point cloud
//	FILE *stream;
//	stream = fopen("vfvf.txt", "r");
//
//	unsigned int minpts, num_points, cluster, i = 0;
//	double epsilon;
//	fscanf(stream, "%u\n", &num_points);
//
//	Point *p = (Point *)calloc(num_points, sizeof(Point));
//
//	while (i < num_points)
//	{
//		fscanf(stream, "%f,%f,%f,%d\n", &(p[i].x), &(p[i].y), &(p[i].z), &cluster);
//		p[i].clusterID = UNCLASSIFIED;
//		points.push_back(p[i]);
//		++i;
//	}
//
//	free(p);
//	fclose(stream);
//}
//
//void printResults(vector<Point>& points, int num_points)
//{
//	int i = 0;
//	printf("Number of points: %u\n"
//		" x     y     z     cluster_id\n"
//		"-----------------------------\n"
//		, num_points);
//	while (i < num_points)
//	{
//		printf("%5.2lf %5.2lf %5.2lf: %d\n",
//			points[i].x,
//			points[i].y, points[i].z,
//			points[i].clusterID);
//		++i;
//	}
//}
//
//int main()
//{
//	vector<Point> points;
//
//	// read point data
//	readBenchmarkData(points);
//
//	// constructor
//	DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
//
//	// main loop
//	ds.run();
//
//	// result of DBSCAN algorithm
//	printResults(ds.m_points, ds.getTotalPointSize());
//	cin.get();
//	return 0;
//}



#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <stack>

#include "Clustering.h"

float stringToFloat(string i) {
	stringstream sf;
	float score = 0;
	sf << i;
	sf >> score;
	return score;
}
vector<point> openFile(const char* dataset) {
	fstream file;
	file.open(dataset, ios::in);
	if (!file)
	{
		cout << "Open File Failed!" << endl;
		vector<point> a;
		return a;
	}
	vector<point> data;
	int i = 1;
	while (!file.eof()) {
		string temp;
		file >> temp;
		int split = temp.find(',', 0);
		point p(stringToFloat(temp.substr(0, split)), stringToFloat(temp.substr(split + 1, temp.length() - 1)), i++);
		data.push_back(p);
	}
	file.close();
	cout << "successful!" << endl;
	return data;
}
float squareDistance(point a, point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
void clustering(vector<point> dataset, float Eps, int MinPts) {
	int len = dataset.size();
	//calculate pts
	cout << "calculate pts" << endl;
	for (int i = 0;i<len;i++) {
		for (int j = i + 1;j<len;j++) {
			if (squareDistance(dataset[i], dataset[j])<Eps)
				dataset[i].pts++;
			dataset[j].pts++;
		}
	}
	//core point 
	cout << "core point " << endl;
	vector<point> corePoint;
	for (int i = 0;i<len;i++) {
		if (dataset[i].pts >= MinPts) {
			dataset[i].pointType = 3;
			corePoint.push_back(dataset[i]);
		}
	}
	cout << "joint core point" << endl;
	//joint core point
	for (int i = 0;i<corePoint.size();i++) {
		for (int j = i + 1;j<corePoint.size();j++) {
			if (squareDistance(corePoint[i], corePoint[j])<Eps) {
				corePoint[i].corepts.push_back(j);
				corePoint[j].corepts.push_back(i);
			}
		}
	}
	for (int i = 0;i<corePoint.size();i++) {
		stack<point*> ps;
		if (corePoint[i].visited == 1) continue;
		ps.push(&corePoint[i]);
		point *v;
		while (!ps.empty()) {
			v = ps.top();
			v->visited = 1;
			ps.pop();
			for (int j = 0;j<v->corepts.size();j++) {
				if (corePoint[v->corepts[j]].visited == 1) continue;
				corePoint[v->corepts[j]].cluster = corePoint[i].cluster;
				corePoint[v->corepts[j]].visited = 1;
				ps.push(&corePoint[v->corepts[j]]);
			}
		}
	}
	cout << "border point,joint border point to core point" << endl;
	//border point,joint border point to core point
	for (int i = 0;i<len;i++) {
		if (dataset[i].pointType == 3) continue;
		for (int j = 0;j<corePoint.size();j++) {
			if (squareDistance(dataset[i], corePoint[j])<Eps) {
				dataset[i].pointType = 2;
				dataset[i].cluster = corePoint[j].cluster;
				break;
			}
		}
	}
	cout << "output" << endl;
	//output
	fstream clustering;
	clustering.open("clustering3.txt", ios::out);
	for (int i = 0;i<len;i++) {
		if (dataset[i].pointType == 2)
			clustering << dataset[i].x << "," << dataset[i].y << "," << dataset[i].cluster << "\n";
	}
	for (int i = 0;i<corePoint.size();i++) {
		clustering << corePoint[i].x << "," << corePoint[i].y << "," << corePoint[i].cluster << "\n";
	}
	clustering.close();
}
/*int main(int argc, char** argv) {
	vector<point> dataset = openFile("new 1.txt");
	DBSCAN(dataset, 0.2, 5);
	cin.get();
	return 0;
}*/


