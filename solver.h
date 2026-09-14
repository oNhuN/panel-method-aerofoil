#pragma once
#include "vec2.h"
#include "panel.h"
#include <vector>


struct LinearSystem //for matrix assembly.
{
    std::vector<std::vector<double>> A;
    std::vector<double> b;
};

Vec2 sourceInfluence(const Panel & panel, const Vec2 & field_point);
Vec2 vortexInfluence(const Panel & panel, const Vec2 & field_point);

LinearSystem matrixAssembly(const std::vector<Panel> & panels, double alpha);



