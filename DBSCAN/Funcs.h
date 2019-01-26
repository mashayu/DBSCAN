#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "MyStructs.h"

using namespace std;

void compensation(vector <R_t> &R, float d_N);

void normalization(vector <R_t>&R);

void scaling(vector <R_t>&R);

pair<double, double> get_vehicle_coords(pair<double, double> coords);

void update_coord(vector <R_t> &targets, pair<double, double> coord);