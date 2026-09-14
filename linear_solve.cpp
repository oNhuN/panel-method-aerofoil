#include <vector>
#include "linear_solve.h"
#include <iostream>
#include <cmath>
#include <utility>
namespace
{
    void forward_elimination(std::vector<std::vector<double>> & A, std::vector<double> & b) 
    {
        const int n = static_cast<int>(A.size()); //number of rows that depends on the matrix A (size gives us the size of the matrix)
        

        for (int k = 0; k <= n-2; ++k) //keeping track of the step that we are in.
        {
            int pivot_row = k;

            for (int r = k + 1; r <= n-1; ++r) //where r is the row
            {
                if (std::abs (A[r][k]) > std::abs(A[pivot_row][k]))
                pivot_row = r;
            }
            
            if (pivot_row !=k )
                {   std::swap(A[k], A[pivot_row]);
                    std::swap(b[k], b[pivot_row]);
                }
            for (int i = k + 1; i <= n-1; ++i) // solves for the row
            {
                double factor = (A[i][k])/(A[k][k]); //solves for the column

                for (int j = k; j <= n-1 ; ++j)
                {
                    A[i][j] -= factor * (A[k][j]) ; //rewrites the new A[i][j] after the factor substraction
                }

                b[i] -= factor * (b[k]); //also rewrites the new b.
            }
        }

    }


    std::vector<double> back_substitution(const std::vector<std::vector<double>> &A, const std::vector<double> & b)
    {
        const int n = static_cast<int>(A.size());

        std::vector<double> x(n); //placeholder for the solutions

        for (int i = n-1; i >= 0; --i)
        {
            double running_total = b[i];

            for (int j = i + 1; j <= n-1; ++j )
            {
                running_total -= (A[i][j] * x[j]); //is what we obtain by moving the already known coefficient to the right side.
            }

            x[i] = running_total / (A[i][i]); //where A[i][i] is the coefficient of the term that we want to solve. 
        }

        return x; //returns the solutions as an array.
    }

}

std::vector<double> solveLinearSystem(std::vector<std::vector<double>> & A, std::vector<double> & b)
{
    forward_elimination(A, b);
    std::vector<double> x {back_substitution(A, b)};

    return x;

}


void printMatrix(const std::vector<std::vector<double>>& A)
{
    const int n = static_cast<int>(A.size());

    std::cout <<"This is matrix A \n";

    for (int i = 0; i<= n-1; ++i)
    {
        for (int j = 0; j<= n-1; ++j)
        {
            std::cout << A[i][j] << '\t'; 
        }
        std::cout << '\n';
    }
}

void printVector(const std::vector<double> & b) //is const because we are printing out so we are not changin anything
{
    const int n = static_cast<int>(b.size());
    std::cout << "The solution is: \n"; 
    
    for (int k = 0; k <= n-1; ++k)
    {
        std::cout << b[k] << '\t';
    };
}