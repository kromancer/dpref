#include "SpMV.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

#pragma dpref indirection(Values[Offsets], X[ColIdx[Offsets]])
void SpMV(const size_t N,
	  const size_t * __restrict Offsets,
	  const size_t * __restrict ColIdx,
	  const double * __restrict Values,
	  const double * __restrict X,
	  double * __restrict Y) {
  for (size_t I = 0; I < N; I++) {
    Y[I] = 0;
    for (size_t J = Offsets[I]; J < Offsets[I + 1]; J++) {

      // Provide a "criterion" for the first slicing
      #pragma dpref sliceOn(J)
      Y[I] += Values[J] * X[ColIdx[J]];
    }
  }
}




#pragma clang diagnostic pop
