#pragma once
#include <vector>

std::vector<double> solveLinearSystem(std::vector<std::vector<double>> & A, std::vector<double> & b); //calls both function in order and returns x.

void printMatrix(const std::vector<std::vector<double>> & A);

void printVector(const std::vector<double> & b);