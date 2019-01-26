#pragma once
#include <utility>
using namespace std;
struct R_t
{
	double x;
	double y;

	double A;
	//double velocity;
	//double timeStamp;
	bool operator<(const R_t &rhs) const { return A < rhs.A; };
};



