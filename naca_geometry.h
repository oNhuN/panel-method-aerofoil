#pragma once
#include "vec2.h"
#include <iostream>
#include <vector>
#include <fstream>

/*The point is to write out a NACA 4-digit code and a number of points, generate an orderes list of surface coordinates
that plots as the expected aerofoil and meets cleanlyat the trailing edge.*/

bool writeCSV(std::vector <Vec2> & points, const std::string & point_csv);

std::vector<Vec2> generateAerofoil(const int N, double m, double p, double t);