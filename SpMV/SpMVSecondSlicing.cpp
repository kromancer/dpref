#include "SpMV.h"

#include <thread>

#include "Queue.h"

using namespace std;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
void firstSlice(const size_t N,
		const size_t *__restrict Offsets,
		dpref::Queue<size_t> *QToSecond,
		dpref::Queue<size_t> *QToThird) {
  for (size_t I = 0; I < N; I++) {
    for (size_t J = Offsets[I]; J < Offsets[I + 1]; J++) {
      QToSecond->push(J);
      QToThird->push(J);
    }
    
    QToSecond->pushLoopTerminationToken();
    QToThird->pushLoopTerminationToken();
  }

  QToSecond->pushTerminationToken();
  QToThird->pushTerminationToken();
}

void secondSlice(const size_t N,
		 const size_t * __restrict ColIdx,
                 dpref::Queue<size_t> *InQ,
		 dpref::Queue<size_t> *OutQ) {
  for (size_t I = 0; I < N; I++) {
    for (;;) {   
      auto Token = InQ->pop();
      if (Token.isTerminationToken())
        return;

      if (Token.isLoopTerminationToken())
        break;

      size_t J = Token.getData();
      OutQ->push(ColIdx[J]);
    }
  }
}

void thirdSlice(const size_t N,
		const double * __restrict Values,
		const double *__restrict X,
		double *__restrict Y,
		dpref::Queue<size_t> *QFromFirst,
		dpref::Queue<size_t> *QFromSecond) {

  for (size_t I = 0; I < N; I++) {
    Y[I] = 0;

    for (;;) {   
      auto Token = QFromFirst->pop();
      if (Token.isTerminationToken())
        return;

      if (Token.isLoopTerminationToken())
        break;
      
      size_t J = Token.getData();
      size_t ColIdx = QFromSecond->pop().getData();
      Y[I] += Values[J] * X[ColIdx];
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

  // Check the poster in doc/ACACES.2023.poster for the dataflow graph
  dpref::Queue<size_t> QFirstToSecond;
  dpref::Queue<size_t> QFirstToThird;
  dpref::Queue<size_t> QSecondToThird;

  thread FirstSliceThread(firstSlice, N, Offsets, &QFirstToSecond, &QFirstToThird);
  thread SecondSliceThread(secondSlice, N, ColIdx, &QFirstToSecond, &QSecondToThird);
  thread ThirdSliceThread(thirdSlice, N, Values, X, Y, &QFirstToThird, &QSecondToThird);
  
  FirstSliceThread.join();
  SecondSliceThread.join();
  ThirdSliceThread.join();
}
