#include "SpMV.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


// What kind of annotations shall the programmer provide?
//
// Option 1: Help the compiler "understand" if/where is the indirection
//
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

      // Option 2: "Describe" a slice by annotating a scope with in/out params
      #pragma dpref slice in(Values[J], ColIdx[J])
      Y[I] += Values[J] * X[ColIdx[J]];
    }
  }
}




#pragma clang diagnostic pop
