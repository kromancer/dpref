#include <algorithm>
#include <cassert>

#include "Utils.h"

using namespace std;

// Function to calculate residual
vector<double> calcResidual(const CSRMatrix &A, vector<double> &X, vector<double> &B) {
  size_t N = X.size();
  vector<double> R(N, 0.0);

  for (size_t Row = 0; Row < N; ++Row) {
    double Sum = 0.0;
    for (size_t Idx = A.Offsets[Row]; Idx < A.Offsets[Row + 1]; ++Idx) {
      size_t Col = A.ColIdx[Idx];
      Sum += A.Values[Idx] * X[Col];
    }
    R[Row] = B[Row] - Sum;
  }

  return R;
}

// Function to restrict to a coarser grid
CSRMatrix restrict(const CSRMatrix &AFine, size_t N) {

  size_t NCoarse = N / 2;
  CSRMatrix ACoarse = CSRMatrix(NCoarse, NCoarse);

  for (int I = 0; I < NCoarse; ++I) {
    for (int J = 0; J < NCoarse; ++J) {

      // Averaging from the fine grid
      double Val = 0.0;
      Val += AFine.getValue(2 * I, 2 * J);
      Val += AFine.getValue(2 * I + 1, 2 * J);
      Val += AFine.getValue(2 * I, 2 * J + 1);
      Val += AFine.getValue(2 * I + 1, 2 * J + 1);
      Val /= 4.0;

      // Set value in the coarser grid
      ACoarse.setValue(I, J, Val);
    }
  }

  return ACoarse;
}

// Function to restrict a vector to a coarser grid
vector<double> restrictVector(const vector<double> &VFine) {
  size_t NCoarse = VFine.size() / 2;
  vector<double> VCoarse(NCoarse, 0.0);

  for (size_t I = 0; I < NCoarse; ++I) {
    VCoarse[I] = 0.5 * (VFine[2 * I] + VFine[2 * I + 1]);
  }

  return VCoarse;
}

// Function to interpolate back to a finer grid using piecewise linear interpolation
void interpolate(vector<double> &XCoarse, vector<double> &XFine) {
  size_t NCoarse = XCoarse.size();
  size_t NFine = XFine.size();

  // Clear fine grid solution vector
  fill(XFine.begin(), XFine.end(), 0.0);

  // Interpolation logic here
  for (size_t I = 0; I < NCoarse - 1; ++I) {
    // Corresponding fine grid indices might depend on your specific restriction logic
    size_t FineStart = I * 2;
    size_t FineEnd = FineStart + 1;

    double Slope = (XCoarse[I+1] - XCoarse[I]);

    XFine[FineStart] = XCoarse[I];
    XFine[FineEnd] = XCoarse[I] + 0.5 * Slope; // halfway point
  }

  // Handle the last two fine grid points
  XFine[NFine - 2] = XCoarse[NCoarse - 1];
  XFine[NFine - 1] = XCoarse[NCoarse - 1];
}


// Function to interpolate in-place from coarse to fine grid
void interpolateInPlace(vector<double> &X, size_t NCoarse) {

  // Work from end to start to avoid overwriting values
  for (int I = static_cast<int>(NCoarse - 1); I > 0; --I) {
    int FineStart = I * 2;
    int FineEnd = FineStart + 1;

    double Slope = X[I] - X[I - 1];

    // Interpolate
    X[FineEnd] = X[I];
    X[FineStart] = X[I - 1] + 0.5 * Slope;
  }

  X[1] = X[0]; // first element remains as is
}


// Gaussian Elimination without pivoting
void eliminate(vector<vector<double>> &A, vector<double> &X, vector<double> &B) {
  const size_t N = A.size();

  // Forward elimination
  for (int I = 0; I < N; ++I) {
    for (int J = I + 1; J < N; ++J) {
      double Factor = A[J][I] / A[I][I];
      for (int K = I; K < N; ++K) {
        A[J][K] -= Factor * A[I][K];
      }
      B[J] -= Factor * B[I];
    }
  }

  // Back substitution
  for (int I = static_cast<int>(N - 1); I >= 0; --I) {
    X[I] = B[I];
    for (int J = I + 1; J < N; ++J) {
      X[I] -= A[I][J] * X[J];
    }
    X[I] /= A[I][I];
  }
}

vector<double> solve2x2(vector<vector<double>> &A, const vector<double> &B) {
  double AA = A[0][0], BB = A[0][1], CC = A[1][0], DD = A[1][1], EE = B[0],
         FF = B[1];

  double Denom = AA * DD - BB * CC;
  assert(Denom != 0.0);

  double X = (EE * DD - BB * FF) / Denom;
  double Y = (AA * FF - EE * CC) / Denom;

  return {X, Y};
}

// Function for solving on the coarsest grid
void solve(const CSRMatrix &A, vector<double> &X, vector<double> &B) {

  // Convert CSR matrix A to dense
  vector<vector<double>> AD = A.toDense();

  const size_t N = B.size();

  // Solve
  if (N <= 1) {
    X[0] = B[0] / AD[0][0];
  } else if (N <= 2) {
    X = solve2x2(AD, B);
  } else {
    eliminate(AD, X, B);
  }
}
