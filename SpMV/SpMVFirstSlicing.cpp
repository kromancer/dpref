#include "SpMV.h"

#include <thread>
#include <tuple>

#include "Queue.h"

using namespace std;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
void firstSlice(const size_t N,
		const size_t *__restrict Offsets,
		const size_t *__restrict ColIdx,
		const double *__restrict Values,
		dpref::Queue<tuple<double, size_t>> *Q) {
  
  for (size_t I = 0; I < N; I++) {
    for (size_t J = Offsets[I]; J < Offsets[I + 1]; J++) {
      Q->push(make_tuple(Values[J], ColIdx[J]));
    }
    
    Q->pushLoopTerminationToken();
  }

  Q->pushTerminationToken();
}

void secondSlice(const size_t N,
		 const double *__restrict X,
		 double *__restrict Y,
                 dpref::Queue<tuple<double, size_t>> *Q) {

  for (size_t I = 0; I < N; I++) {
    Y[I] = 0;

    for (;;) {   
      auto Token = Q->pop();
      if (Token.isTerminationToken())
        return;

      if (Token.isLoopTerminationToken())
        break;

      auto [MVal, ColIdx] = Token.getData();
      Y[I] += MVal * X[ColIdx];
    }
  }
}
#pragma clang diagnostic pop

void SpMV(const size_t N,
	  const size_t *__restrict Offsets,
	  const size_t *__restrict ColIdx,
	  const double *__restrict Values,
	  const double *__restrict X,
	  double *__restrict Y) {

  dpref::Queue<tuple<double, size_t>> Q;

  thread FirstSliceThread(firstSlice, N, Offsets, ColIdx, Values, &Q);
  thread SecondSliceThread(secondSlice, N, X, Y, &Q);
  
  FirstSliceThread.join();
  SecondSliceThread.join();
}
