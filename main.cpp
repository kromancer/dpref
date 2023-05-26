#include <iostream>
#include <span>

#include "Bfs.h"
#include "Vertex.h"

using namespace std;

size_t Vertex::NumOfVertices = 0;

int main() {

  constexpr size_t NumOfVertices = 5;

  constexpr size_t Neighbors[] = {
      /* Neighbors of vertex 0, index range [0, 1] */
      1, 2,

      /* Neighbors of vertex 1, index range [2, 3] */
      0, 2,

      /* Neighbors of vertex 2, index range [4, 4] */
      3,

      /* Neighbors of vertex 3, index range [5, 7] */
      1, 2, 4,

      /* Neighbors of vertex 4, index range [8, 8] */
      0
  };

  /*
     The neighbors of Vertex i are in the slice
     neighbor[offsets[i]:offsets[i + 1]]
   */
  constexpr size_t Offsets[] = {0, 2, 4, 5, 8, 9};

  Vertex Vertices[NumOfVertices];

  bfs(0, Offsets, Neighbors, Vertices);

  for (auto &V:span<Vertex>{Vertices})
    cout << "Parent of " << V.Id << " is " << V.Parent << endl;

  return 0;
}
