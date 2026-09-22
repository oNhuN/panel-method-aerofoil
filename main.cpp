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
    std::cout << "This is the total point count " << points.size() << '\n';
    auto panels{generatePanel(points)};
    std::cout << "This is the total point count " << panels.size() << '\n';
    const int N {static_cast<int>(panels.size())}; //number of panels
    const double pi{std::numbers::pi};
    double perimeter{};

    for (int j = 0; j < N; ++j) //N is the number of panels
    {
        perimeter += panels[j].length;
    }



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