#include "write_csv.h"
#include "panel.h"
#include "naca_geometry.h"
#include "solver.h"
#include <iostream>

int main ()
{
    const int N = 100;
    const double m = 0.04;
    const double p = 0.4;
    const double t = 0.15;

    auto points{generateAerofoil(N, m, p, t)}; //auto: works out the type of data from the initializer
    
    auto panels{generatePanel(points)};

     Panel test_panel{
        {0.506364, -0.0267117},   // start
        {0.490703, -0.0276280},   // end
        {0.498533, -0.0271698},   // midpoint
        {-0.998293, -0.0584066},  // tangent
        {0.0584066, -0.998293},   // normal
        0.0156881                 // length
    };

    Vec2 test_point{0.46933, 0.107916};

    Vec2 result{sourceInfluence(test_panel, test_point)};
    Vec2 vortex_result{vortexInfluence(test_panel, test_point)};

    std::cout << result.x << ", " << result.y << '\n';
    std::cout << vortex_result.x << ", " << vortex_result.y << '\n';

    if (!writeCSV_NACA(points, "points.csv"))
        std::cout <<"Failed to open the csv file!";
    if(!writeCSV_Panel(panels, "panel.csv"))
        std::cout <<"Failed to open the csv file!";

    return 0;
    
}