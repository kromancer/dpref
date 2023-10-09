#pragma once

#include "CSRMatrix.h"
#include <vector>

std::vector<double> *mgVCycle(const CSRMatrix *A, std::vector<double> *B);
