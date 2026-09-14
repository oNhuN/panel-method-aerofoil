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

        double delta_theta{theta_2 - theta_1};

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
    return global_frames; //returning velocities
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
    return global_frames; //returns a global velocity 
}



LinearSystem matrixAssembly(const std::vector<Panel> & panels, double alpha) // where alpha is the angle of attack
{
    const int N = static_cast<int>(panels.size()); //size of the matrix that depends on panel.

    LinearSystem matrix_system{}; //empty list to return
    
    matrix_system.A.resize(N+1, std::vector<double>(N+1)); //(nber of rows, what to fill in each row)
    matrix_system.b.resize(N+1);
    
    Vec2 freestream {std::cos(alpha), std::sin(alpha)};

    for (int i = 0; i <= N-1; ++i)
    {   
        double vortex_counter {0.0}; //accumulator
    
        for (int j = 0; j <= N-1; ++j)
        {
            if (i == j) //diagonal case
            {
                matrix_system.A[i][j] = 0.5;
            }
            else
            {
            Vec2 source_v{sourceInfluence(panels[j], panels[i].midpoint)};
            matrix_system.A[i][j] = dot(source_v, panels[i].normal); //value of A[i][j]

            Vec2 vortex_v{vortexInfluence(panels[j], panels[i].midpoint)};
            vortex_counter = vortex_counter + dot(vortex_v, panels[i].normal);
            }
        }
        matrix_system.A[i][N] = vortex_counter;
        matrix_system.b[i] = -dot(freestream, panels[i].normal);
    }

    //kutta row
    const Vec2 t_0{panels[0].tangent};
    const Vec2 mid_0{panels[0].midpoint};
    const Vec2 t_f{panels[N-1].tangent};
    const Vec2 mid_f{panels[N-1].midpoint};

    double acc{0.0};

    for (int j = 0; j <= N-1; ++j) //influence of each panel on point 0 and N-1
    {
        if (j==0)
        {
            Vec2 sf{sourceInfluence(panels[j], mid_f)};
            Vec2 vf{vortexInfluence(panels[j], mid_f)};
            matrix_system.A[N][j] = dot(sf, t_f);
            acc += -0.5 + dot(vf, t_f);
        }
        else if(j== N-1)
        {
            Vec2 s0{sourceInfluence(panels[j], mid_0)};
            Vec2 v0{vortexInfluence(panels[j], mid_0)};
            matrix_system.A[N][j] = dot(s0, t_0);
            acc += -0.5 + dot(v0, t_0);
        }
        else
        {
            Vec2 s0{sourceInfluence(panels[j], mid_0)};
            Vec2 sf{sourceInfluence(panels[j], mid_f)};
            matrix_system.A[N][j] = dot(s0 , t_0) + dot(sf, t_f);
            Vec2 v0{vortexInfluence(panels[j], mid_0)};
            Vec2 vf{vortexInfluence(panels[j], mid_f)};
            acc = acc + dot(v0 , t_0) + dot(vf, t_f);
        }

    }
    matrix_system.A[N][N] = acc;
    matrix_system.b[N] = -dot(freestream, t_0 + t_f);
    

    return matrix_system;
}