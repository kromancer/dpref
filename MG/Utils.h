#pragma once

#include <vector>

#include "CSRMatrix.h"

std::vector<double> calcResidual(const CSRMatrix &A, std::vector<double> &X, std::vector<double> &B);

CSRMatrix restrict(const CSRMatrix &AFine, size_t N);

std::vector<double> restrictVector(const std::vector<double> &VFine);

void interpolate(std::vector<double> &XCoarse, std::vector<double> &XFine);

void interpolateInPlace(std::vector<double> &X, size_t NCoarse);

void eliminate(std::vector<std::vector<double>> &A, std::vector<double> &X, std::vector<double> &B);

std::vector<double> solve2x2(std::vector<std::vector<double>> &A, const std::vector<double> &B);

void solve(const CSRMatrix &A, std::vector<double> &X, std::vector<double> &B);
