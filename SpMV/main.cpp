#include <cstddef>
#include <iostream>
#include <span>

#include "SpMV.h"

using namespace std;

int main() {

  constexpr size_t N = 4;
  constexpr double X[N] = {1.1, 2.2, 3.3, 4.4};
  double Y[N] = { 0 };

  size_t Offsets[] = {
    // First Row [0, 2)
    0, 2,
    
    // Second Row [2, 3)
    3,
    
    // Third Row [3, 5)
    5,

    // Fourth Row [5, 6)
    6
  };
  
  size_t ColIdx[] = {
    // First Row
    0, 3,
    
    // Second Row
    1,
    
    // Third Row
    0, 2,

    // Fourth Row
    3
  };
  
  double Values[] = {
    // First Row
    3.2, 4.1,

    // Second Row
    5.1,

    // Third Row
    7.1, 2.2,
    
    // Fourth Row
    8.2
  };

  SpMV(N, Offsets, ColIdx, Values, X, Y);

  for (double Val:span<double>{Y})
    cout <<  Val << ", ";
  cout << endl;
  
  return 0;
  
}
