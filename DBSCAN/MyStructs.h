#pragma once
#include <utility>
using namespace std;
struct R_t
{
	int id;
	double x;
	double y;
	double z;
	double snr;
	double A;
	//double velocity;
	//double timeStamp;
	bool operator<(const R_t &rhs) const { return A < rhs.A; };
};



