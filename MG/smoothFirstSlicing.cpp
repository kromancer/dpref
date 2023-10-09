#include "CSRMatrix.h"
#include "smooth.h"

#include <thread>
#include <tuple>
#include <vector>

#include "Queue.h"

using namespace std;

// Smooth using a single iteration of the Gauss-Seidel
void smoothFirstSlice(dpref::Queue<const CSRMatrix *> *InQ,
                      dpref::Queue<size_t> *OutQ) {

  for (;;) {
    auto Token = InQ->pop();

    if (Token.isTerminationToken())
      return;

    auto *A = Token.getData();
    const size_t N = A->Offsets.size() - 1;

    for (int Row = 0; Row < N; ++Row) {
      for (size_t I = A->Offsets[Row]; I < A->Offsets[Row + 1]; ++I) {
        size_t Col = A->ColIdx[I];
        OutQ->push(Col);
      }
    }
  }
}

void smoothSecondSlice(
    dpref::Queue<tuple<const CSRMatrix *, vector<double> *, const vector<double> *>> *InQ1,
    dpref::Queue<size_t> *InQ2) {

  for (;;) {

    auto Token = InQ1->pop();

    if (Token.isTerminationToken())
      return;

    auto [A_, X_, B_] = Token.getData();
    const CSRMatrix &A = *A_;
    vector<double> &X = *X_;
    const vector<double> &B = *B_;

    const size_t N = A.Offsets.size() - 1;

    for (size_t Row = 0; Row < N; ++Row) {

      double Sum = 0.0;
      double DiagVal = 0.0;

      for (size_t Idx = A.Offsets[Row]; Idx < A.Offsets[Row + 1]; ++Idx) {
        double AVal = A.Values[Idx];

        auto Col = InQ2->pop().getData();

        if (Col == Row) {
          DiagVal = AVal;
          continue;
        }

        Sum += AVal * X[Col];
      }

      X[Row] = (B[Row] - Sum) / DiagVal;
    }
  }
}

void smooth(const CSRMatrix &A, vector<double> &X, const vector<double> &B) {

  dpref::Queue<const CSRMatrix *> InQ1;
  dpref::Queue<tuple<const CSRMatrix *, vector<double> *, const vector<double> *>> InQ2;
  dpref::Queue<size_t> First2SecondQ;

  thread FirstSliceThread(smoothFirstSlice, &InQ1, &First2SecondQ);
  thread SecondSliceThread(smoothSecondSlice, &InQ2, &First2SecondQ);

  InQ1.push(&A);
  InQ1.pushTerminationToken();

  InQ2.push(make_tuple(&A, &X, &B));
  InQ2.pushTerminationToken();

  FirstSliceThread.join();
  SecondSliceThread.join();
}
