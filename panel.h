#pragma once
#include "vec2.h"
#include <vector>

struct Panel
{
    Vec2 start;
    Vec2 end;
    Vec2 midpoint;
    Vec2 tangent;
    Vec2 normal;
    double length;
};

std::vector<Panel> generatePanel(const std::vector<Vec2> & points);