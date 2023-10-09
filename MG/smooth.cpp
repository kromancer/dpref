#include "smooth.h"

#include <cassert>
#include <vector>

using namespace std;

// Smooth using a single iteration of the Gauss-Seidel
void smooth(const CSRMatrix &A, vector<double> &X, const vector<double> &B) {

  size_t N = X.size();
  assert(B.size() == N);
  assert(A.Offsets.size() == N + 1);

  for (size_t Row = 0; Row < N; ++Row) {

    double Sum = 0.0;
    double DiagVal = 0.0;

    for (size_t Idx = A.Offsets[Row]; Idx < A.Offsets[Row + 1]; ++Idx) {

      size_t Col = A.ColIdx[Idx];
      double AVal = A.Values[Idx];

      // Excludes the diagonal contribution from Sum
      if (Col == Row) {
        DiagVal = AVal;
        continue;
      }

      #pragma dpref sliceOn(Col)
      Sum += AVal * X[Col];
    }

    X[Row] = (B[Row] - Sum) / DiagVal;
  }
}
