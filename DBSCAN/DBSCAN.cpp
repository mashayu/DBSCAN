// DBSCAN.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "ProbMap.h"
#include "Funcs.h"
#include "Clustering.h"

int main()
{

	R_t temp;
	ifstream infile("polar.txt");
	double buff;
	vector<R_t> sample;
	/*for (int i = 0; i < 5; i++)
	{
		infile >> buff;
		cout << buff << endl;
	}*/
	

	for (int i = 0; i < 150; i++) {
		infile >> buff;
		temp.x = buff;
		infile >> buff;
		temp.y = buff;
		infile >> buff;
		temp.A = buff;
		sample.push_back(temp);
	}

	//vector<R_t> sample;
	/*R_t a{ 1,2,3 }, b{0.5,1,4 }, c{ 2.2,1.4,6 };
	

	sample.push_back(a);
	sample.push_back(b);
	sample.push_back(c);*/

	int tar_num = sample.size();
	float d_N = 50;

	int M = 300, N = 300;
	double delta_x = 1, delta_y = 1;
	pair<double, double> coords = make_pair(0,0);

	ProbMap parking = ProbMap(M, N, delta_x, delta_y);

	compensation(sample, d_N);

	normalization(sample);

	scaling(sample);

	//coords = get_vehicle_coords(coords); 

	//update_coord(sample, coords);

	parking.fill_target_list(sample);

	parking.fill_grid();

	parking.clean();
	
	vector<point> test = parking.make_list_of_points();

	float eps = 0.2;
	int minPts = 5;

	clustering(test, eps, minPts);

	ofstream myfile("example.txt");

	parking.print(myfile);

}
