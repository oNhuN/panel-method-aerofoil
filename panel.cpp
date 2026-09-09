#include "panel.h"
#include <iostream>
#include <cmath>
#include <numbers>
#include <vector>

namespace
{
    double length(const Vec2& start, const Vec2& end)
    {
        double magnitude{std::sqrt((end.x-start.x)*(end.x-start.x) + (end.y-start.y)*(end.y-start.y))};
        return magnitude;
    }

    Vec2 midPoint(const Vec2& start, const Vec2& end)
    {
        Vec2 mid_point{Vec2{(start.x + end.x)/2.0,(start.y + end.y)/2.0}};
        
        return mid_point;
    }
    
    Vec2 tgentUnitVector(const Vec2& start, const Vec2& end)
    {
        double magnitude{length(start, end)};

        if (magnitude == 0){
            return {Vec2{0.0, 0.0}};

        }

        Vec2 tgentVector{(end.x-start.x)/magnitude, (end.y-start.y)/magnitude};

        return tgentVector;


    }

    Vec2 normalVector(const Vec2& start, const Vec2& end)
    {
        Vec2 tangent{tgentUnitVector(start, end)};

        Vec2 normal{-tangent.y, tangent.x}; //return of the normal vector (pointing outwards)

        return normal;
    }
}


std::vector<Panel> generatePanel(const std::vector<Vec2> & points) //public function returning std::vector<Panel>
{
    std::vector<Panel> panels{}; //empty list for later push_back

    for (std::size_t i = 0; i + 1 < points.size(); ++i)
    {
        const Vec2& start {points[i]};
        const Vec2& end {points[i + 1]};

        double magnitude{length(start, end)};
        Vec2 mid_point{midPoint(start, end)};
        Vec2 tangent{tgentUnitVector(start, end)};
        Vec2 normal{normalVector(start, end)};
        
        panels.push_back({start, end, mid_point, tangent, normal, magnitude}); //
    }
    
    return panels;
}

