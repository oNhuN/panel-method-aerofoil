#include "write_csv.h"
#include "panel.h"
#include "naca_geometry.h"
#include "solver.h"
#include <iostream>
#include "linear_solve.h"
#include <numbers>
#include <cmath>

int main()
{
    const int n_per_surface = 200;
    const double m = 0;
    const double p = 0;
    const double t = 0.12;

    auto points{generateAerofoil(n_per_surface, m, p, t)};

    auto panels{generatePanel(points)};

    const int N{static_cast<int>(panels.size())};
    int failures{};

    double alpha{0};

    // Test panel

    Panel test_panel{
        {0.506364, -0.0267117},   // start
        {0.490703, -0.0276280},   // end
        {0.498533, -0.0271698},   // midpoint
        {-0.998293, -0.0584066},  // tangent
        {0.0584066, -0.998293},   // normal
        0.0156881                 // length
    };

    std::cout << "This file will run if the mid test of the whole Hess-Smith solver passes." << '\n';

    // Influence test point

    Vec2 test_point{0.46933, 0.107916};

    Vec2 result{sourceInfluence(test_panel, test_point)};
    Vec2 vortex_result{vortexInfluence(test_panel, test_point)};

    std::cout << "Source influence: (" << result.x << ", " << result.y << ")\n";
    std::cout << "Vortex influence: (" << vortex_result.x << ", " << vortex_result.y << ")\n";

    // Self-influence test

    Vec2 debug{test_panel.normal * 1e-6};
    Vec2 debugging_point{test_panel.midpoint + debug};

    Vec2 dg{sourceInfluence(test_panel, debugging_point)};

    std::cout << "Self-influence test point: (" << debugging_point.x << ", " << debugging_point.y << ")\n";
    std::cout << "Self-influence result: (" << dg.x << ", " << dg.y << ")\n";

    // Week C Hess-Smith linear system

    LinearSystem linear_system(matrixAssembly(panels, alpha));

    std::vector<double> x{solveLinearSystem(linear_system.A, linear_system.b)};

    // Last unknown is the constant vortex-sheet strength gamma
    double gamma{x.back()};

    // Net source strength and perimeter

    double testing{};
    double perimeter{};

    for (int j = 0; j < N; ++j)
    {
        testing += x[j] * panels[j].length;
        perimeter += panels[j].length;
    }

    // Test 1: symmetry
    // NACA 0012 at alpha = 0 should have gamma = 0

    if (std::abs(gamma) < 1e-12)
    {
        std::cout << "Symmetry: gamma = 0 at alpha = 0 ... PASS" << '\n';
    }
    else
    {
        std::cout << "Symmetry: gamma = 0 at alpha = 0 ... FAIL" << '\n';

        std::cout << "gamma = " << gamma << '\n';
        ++failures;
    }

    // Test 2: source influence

    const double tolerance{1e-4};

    const double expected_x{-0.003809}; //obtained form hand-on calculation.
    const double expected_y{0.01769};

    const bool influence_x{std::abs(result.x - expected_x) < tolerance};
    const bool influence_y{std::abs(result.y - expected_y) < tolerance};

    if (influence_x && influence_y)
    {
        std::cout << "Source influence: PASS" << '\n';
    }
    else
    {
        std::cout << "Source influence: FAIL" << '\n'; // THIS HAS FAILED, CHECK THIS!
        std::cout << "Expected: (" << expected_x << ", " << expected_y << ")\n";
        std::cout << "Obtained: (" << result.x << ", " << result.y << ")\n";
        ++failures;
    }

    // Test 3: self-influence
    // Expected velocity is 0.5 times the outward normal

    const double self_tolerance{1e-3}; // loosen tolerance as it is not a exactness check but limit check.
    Vec2 expected_self{test_panel.normal * 0.5};

    const bool self_x{std::abs(dg.x - expected_self.x) < self_tolerance};
    const bool self_y{std::abs(dg.y - expected_self.y) < self_tolerance};

    if (self_x && self_y)
    {
        std::cout << "Self-influence: PASS" << '\n';
    }
    else
    {
        std::cout << "Self-influence: FAIL" << '\n';
        std::cout << "Expected: (" << expected_self.x << ", " << expected_self.y << ")\n";
        std::cout << "Obtained: (" << dg.x << ", " << dg.y << ")\n";
        ++failures;
    }

    // Circulation and lift coefficient

    double GAMMA{gamma * perimeter};

    double Cl{2.0 * GAMMA};

    std::cout << "Net source strength: " << testing << '\n';
    std::cout << "Perimeter: " << perimeter << '\n';
    std::cout << "GAMMA: " << GAMMA << '\n';
    std::cout << "Cl at alpha = 0 rad: " << Cl << '\n';

    // Final result
    
    if (failures == 0)
    {
        std::cout << "\nAll tests PASS." << '\n';
    }
    else
    {
        std::cout << "\n" << failures << " test(s) FAILED." << '\n';
    }
    return failures == 0 ? 0 : 1;
}