#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <stack>
#include <algorithm>
#include "Clustering.h"

using namespace std;


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



vector<point>clustering(vector<point> dataset, float Eps, int MinPts) {
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
	return corePoint;
}
/*int main(int argc, char** argv) {
	vector<point> dataset = openFile("new 1.txt");
	DBSCAN(dataset, 0.2, 5);
	cin.get();
	return 0;
}*/


vector<point> noiseRemoval(vector<point> clusteredData, int threshold)
{
	vector<point> results;
	vector<int> clusters;
	vector<int> clusterscopy;
	for (int i = 0; i < clusteredData.size(); i++)
	{
		clusters.push_back(clusteredData[i].cluster);
	}
	clusterscopy = clusters;
	sort(clusters.begin(), clusters.end());
	auto last = std::unique(clusters.begin(), clusters.end());
	clusters.erase(last, clusters.end());

	for (int i = 0; i < clusteredData.size(); i++)
	{
		for (int j = 0; j < clusters.size(); j++)
		{
			if (clusteredData[i].cluster == clusters[j] &&
				count(clusterscopy.begin(), clusterscopy.end(), clusters[j]) > threshold)
			{
				results.push_back(clusteredData[i]);
			}
		}
	}
	fstream result;
	result.open("onesecond_clustered_2.txt", ios::out);
	for (int j = 0; j < results.size(); j++)
	{
		result << results[j].x << "," << results[j].y << "," << results[j].cluster << "\n";
	}
	result.close();
	return results;
}

vector<BBox> boundingBox(vector<point> noisedRemovedResults)
{
	vector<BBox> clusteredBoundingBox;
	vector<int> clusters;
	vector<double> valX;
	vector<double> valY;

	for (int i = 0; i < noisedRemovedResults.size(); i++)
	{
		clusters.push_back(noisedRemovedResults[i].cluster);
	}
	sort(clusters.begin(), clusters.end());
	auto last = std::unique(clusters.begin(), clusters.end());
	clusters.erase(last, clusters.end());

	for (int i = 0; i < clusters.size(); i++)
	{
		valX.clear();
		valY.clear();
		for (int j = 0; j < noisedRemovedResults.size(); j++)
		{
			if (noisedRemovedResults[j].cluster == clusters[i])
			{
				valX.push_back(noisedRemovedResults[j].x);
				valY.push_back(noisedRemovedResults[j].y);
			}
		}
		sort(valX.begin(), valX.end());
		sort(valY.begin(), valY.end());
		int sizeX = valX.size();
		int sizeY = valY.size();

		cout << "Min X: " << valX[0] << "& Max X: " << valX[sizeX - 1] << endl;
		cout << "Min Y: " << valY[0] << "& Max Y: " << valY[sizeY - 1] << endl;

		clusteredBoundingBox.push_back(BBox());
		clusteredBoundingBox[i].xMin = valX[0];
		clusteredBoundingBox[i].xMax = valX[sizeX - 1];
		clusteredBoundingBox[i].yMin = valY[0];
		clusteredBoundingBox[i].yMax = valY[sizeY - 1];
		clusteredBoundingBox[i].cluster = clusters[i];
	}
	return clusteredBoundingBox;
}
