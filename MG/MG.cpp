// Simplified MultiGrid Pseudo-Code Adapted for CSR Matrix Representation
// Co-synthesized by Konstantinos Sotiropoulos <ioanniss@chalmers.se> and
// ChatGPT from OpenAI as of August 3, 2023.

// WARNING: It does not work!

#include "MG.h"

#include <vector>

#include "CSRMatrix.h"
#include "Utils.h"
#include "smooth.h"

using namespace std;

constexpr size_t CoarseGridThreshold = 10;

// MultiGrid V-cycle
vector<double> *mgVCycle(const CSRMatrix *A, vector<double> *B) {

  const size_t N = B->size();

  auto *X = new vector<double>(N, 0);

  // Check for "small enough" size
  if (N <= CoarseGridThreshold) {
    solve(*A, *X, *B);
    return X;
  }

  // Step 1: Pre-smoothing and residual calculation
  smooth(*A, *X, *B);
  vector<double> R = calcResidual(*A, *X, *B);

  // Step 2: Restrict residual and matrix to coarse grid
  vector<double> BCoarse = restrictVector(R);
  CSRMatrix ACoarse = restrict(*A, N);

  // Step 3: Solve on coarser grid (recursively)
  vector<double> *XCoarse =
    mgVCycle(&ACoarse, &BCoarse);

  // Step 4: Interpolate solution back to fine grid
  interpolate(*XCoarse, *X);

  // Step 5: Post-smoothing
  smooth(*A, *X, *B);

  return X;
}
