#include "write_csv.h"
#include "panel.h"
#include "naca_geometry.h"
#include "solver.h"
#include <iostream>
#include "linear_solve.h"
#include <numbers>
int main ()
{
    const int n_per_surface = 200;
    const double m = 0;
    const double p = 0;
    const double t = 0.12;

    auto points{generateAerofoil(n_per_surface, m, p, t)}; //auto: works out the type of data from the initializer
    
    auto panels{generatePanel(points)};

    const int N {static_cast<int>(panels.size())}; //number of panels
    const double pi{std::numbers::pi};

    // double alpha{(5*pi)/180};
    // double alpha{(2.5*pi)/180};
    // double alpha{(10 *pi)/180};

    Panel test_panel{
        {0.506364, -0.0267117},   // start
        {0.490703, -0.0276280},   // end
        {0.498533, -0.0271698},   // midpoint
        {-0.998293, -0.0584066},  // tangent
        {0.0584066, -0.998293},   // normal
        0.0156881                 // length
    };


    // Vec2 debug{test_panel.normal * 1e-6};
    // Vec2 debugging_point{test_panel.midpoint + debug};

    // std::cout << "The debuggin point is: (" << debugging_point.x << ','<< debugging_point.y << ')' << '\n';
    // Vec2 dg{sourceInfluence(test_panel, debugging_point)};
    // std::cout << "The debug test is: (" << dg.x << ','<< dg.y << ')' << '\n';


    Vec2 test_point{0.46933, 0.107916};

    Vec2 result{sourceInfluence(test_panel, test_point)};
    Vec2 vortex_result{vortexInfluence(test_panel, test_point)};

    
    std::cout << result.x << ", " << result.y << '\n';
    std::cout << vortex_result.x << ", " << vortex_result.y << '\n';


    // //Week C milestone
    // //linear system storing
    // LinearSystem linear_system(matrixAssembly(panels, alpha)); 
    // std::vector<double> x{solveLinearSystem(linear_system.A, linear_system.b)}; 

    // printVector(x); //printing it out.

    // double testing{};
    double perimeter{};

    for (int j = 0; j < N; ++j) //N is the number of panels
    {
        // testing += x[j] * panels[j].length;
        perimeter += panels[j].length;

    }
    // std::cout << "The testing is: "<< testing << '\n';
    // // double GAMMA{x.back() * perimeter};
    // // double Cl{2*GAMMA};
    // // std::cout << "GAMMA: " << GAMMA << '\n';
    // // std::cout << "Cl for alpha{5 degrees} in radians is = " << Cl << '\n';



    //Week D

    std::vector<double> angles{};
    std::vector<double> Cl{};

    for (int degrees = -15; degrees <= 15; ++degrees)
    {
        double alpha{(degrees * pi)/180}; 


        LinearSystem linear_system(matrixAssembly(panels, alpha)); 
        std::vector<double> x{solveLinearSystem(linear_system.A, linear_system.b)}; 
        
        double GAMMA{x.back() * perimeter};
        angles.push_back(static_cast<double>(degrees));
        Cl.push_back(-2*GAMMA); //sign swapping for convention as initially clockwise building has been chosen.
    }





    if (!writeCSV_NACA(points, "points.csv"))
        std::cout <<"Failed to open the csv file!";
    if(!writeCSV_Panel(panels, "panel.csv"))
        std::cout <<"Failed to open the csv file!";
    if (!writeCSV_Polar(angles, Cl, "polar.csv"))
        std::cout <<"Failed to open the csv file!";
    return 0;
    
}