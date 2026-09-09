#include "naca_geometry.h"
#include <cmath>
#include <numbers>


namespace { //anonymous namspace just to keep this unique of naca_geometry

    double cosineSpacing(int N, int station_index)
    {
        double pi {std::numbers::pi};
        double radians {std::cos((pi*static_cast<double>(station_index))/(N-1))}; 
        double x {(1-radians)/2};
        return x;
    }

    double thickness(double x, double t)
    {
        double y_t{(t/0.2) * (0.2969 * std::sqrt(x) - 0.1260 * x- 0.3516 * x *x + 0.2843 * std::pow(x, 3) - 0.1036*std::pow(x, 4))};

        return y_t;
    }

    double camberHeight(double m, double p, double x) //taking in the NACA parameters
    {
        double y_c{};
        if (x < p)
        {
            y_c = m * ((2*p*x - x * x)/(p*p));
        }
        else
        {
            y_c = m * (((1-2*p) + (2*p*x) - x*x)/((1-p)*(1-p)));
        }
    
        return y_c;
    }


    double slopeCalculation(double m, double p, double x)
    {
        double dy_c{};
        if (x < p)
        {
            dy_c = (m/(p*p)) * (2*p - 2*x);
        }
        else 
        {
            dy_c = (m/((1-p) * (1-p))) * (2*p-2*x);
        }
        return dy_c;
    }
}

std::vector<Vec2> generateAerofoil(const int N, double m, double p, double t)
{
    std::vector<Vec2> points {};
    
    for (int i = N - 1; i >= 0; --i) //descending case
    {
        double x{cosineSpacing(N, i)};
        double y_c{camberHeight(m, p, x)};
        double dy_c{slopeCalculation(m, p, x)};
        double theta{std::atan(dy_c)};
        double y_t{thickness(x, t)};


        points.push_back({(x + y_t * std::sin(theta)), (y_c -y_t * std::cos(theta))}); 
    }

    for (int i = 1; i < N; ++i) //ascending case
    {
        double x{cosineSpacing(N, i)};
        double y_c{camberHeight(m, p, x)};
        double dy_c{slopeCalculation(m, p, x)};
        double theta{std::atan(dy_c)};
        double y_t{thickness(x, t)};
        points.push_back({(x - y_t * std::sin(theta)), (y_c + y_t * std::cos(theta))});
    }

    return points;
}