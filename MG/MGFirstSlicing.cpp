#include "MG.h"

#include <thread>
#include <utility>
#include <vector>

#include "CSRMatrix.h"
#include "Queue.h"
#include "Utils.h"

using namespace std;

constexpr size_t CoarseGridThreshold = 10;

// MultiGrid V-cycle
vector<double> *mgVCycleRecursive(const CSRMatrix *A, vector<double> *B) {

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
    mgVCycleRecursive(&ACoarse, &BCoarse);

  // Step 4: Interpolate solution back to fine grid
  interpolate(*XCoarse, *X);

  // Step 5: Post-smoothing
  smooth(*A, *X, *B);

  return X;
}

// Wrapper around mgVCycleRecursive
void mgVCycleSlice(dpref::Queue<pair<const CSRMatrix *, vector<double> *>> *InQ,
                   dpref::Queue<vector<double> *> *OutQ) {

  for (;;) {

    auto Token = InQ->pop();

    if (Token.isTerminationToken())
      return;

    auto [A, B] = Token.getData();

    auto *X = mgVCycleRecursive(A, B);

    OutQ->push(X);

  }
}

vector<double> *mgVCycle(const CSRMatrix *A, vector<double> *B) {

  dpref::Queue<pair<const CSRMatrix *, vector<double> *>> InQ;
  dpref::Queue<vector<double> *> OutQ;

  thread FirstSliceThread(mgVCycleSlice, &InQ, &OutQ);

  InQ.push(make_pair(A, B));
  InQ.pushTerminationToken();

  FirstSliceThread.join();

  return OutQ.pop().getData();
}
