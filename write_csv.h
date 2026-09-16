#pragma once
#include "vec2.h"
#include "panel.h"
#include <iostream>
#include <vector>
#include <fstream>

bool writeCSV_NACA(std::vector <Vec2> & points, const std::string & point_csv);

bool writeCSV_Panel(std::vector<Panel> & panels, const std::string & panel_csv);

bool writeCSV_Polar(std::vector<double> & angles, std::vector<double> & Cl, const std::string & polar_csv);