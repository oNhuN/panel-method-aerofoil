#include "solver.h"
#include "vec2.h"
#include "panel.h"
#include <cmath>
#include <numbers>

namespace
{

    Vec2 localTransformation(const Panel& panel, const Vec2& field_point)
    {
        Vec2 displacement{field_point.x - panel.start.x, field_point.y - panel.start.y};
        
        double local_x{displacement.x * panel.tangent.x + displacement.y * panel.tangent.y};
        double local_y{displacement.x * panel.normal.x + displacement.y * panel.normal.y};

        Vec2 local_frames{local_x, local_y};
        return local_frames;
    }

    double distanceToEndpoints(double x, double y) //finished
    {
        double r{sqrt((x*x) + (y*y))};

        return r;
    }

    double angleCalculation(double x, double y, double L) //finished
    {
        double theta_1{std::atan2(y, x)};

        double theta_2{std::atan2(y, x-L)};

        double delta_theta{theta_1-theta_2};

        return delta_theta;
    }

}


Vec2 sourceInfluence(const Panel & panel, const Vec2& field_point) //public function
{
    double L{panel.length};
    double pi{std::numbers::pi};
    Vec2 local_frames{localTransformation(panel, field_point)};

    double r_1{distanceToEndpoints(local_frames.x, local_frames.y)}; 
    double r_2{distanceToEndpoints(local_frames.x-L, local_frames.y)};

    double delta_theta{angleCalculation(local_frames.x, local_frames.y, L)};


    double u_source{(std::log(r_1/r_2))/(2*pi)};
    double v_source{delta_theta/(2*pi)};

    double global_x{u_source * panel.tangent.x + v_source * panel.normal.x};
    double global_y{u_source * panel.tangent.y + v_source * panel.normal.y};

    Vec2 global_frames{global_x, global_y};
    return global_frames;
}

Vec2 vortexInfluence(const Panel & panel, const Vec2 & field_point) //second public function
{
    double L{panel.length};
    double pi{std::numbers::pi};
    Vec2 local_frames{localTransformation(panel, field_point)};

    double r_1{distanceToEndpoints(local_frames.x, local_frames.y)}; 
    double r_2{distanceToEndpoints(local_frames.x-L, local_frames.y)};

    double delta_theta{angleCalculation(local_frames.x, local_frames.y, L)};


    double u_vortex{-delta_theta/(2*pi)};
    double v_vortex{(std::log(r_1/r_2))/(2*pi)};

    double global_x{u_vortex * panel.tangent.x + v_vortex * panel.normal.x};
    double global_y{u_vortex * panel.tangent.y + v_vortex * panel.normal.y};

    Vec2 global_frames{global_x, global_y};
    return global_frames;
}