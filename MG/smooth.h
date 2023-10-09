#pragma once

#include <vector>

#include "CSRMatrix.h"

// Smooth using a single iteration of the Gauss-Seidel
void smooth(const CSRMatrix &A, std::vector<double> &X, const std::vector<double> &B);
