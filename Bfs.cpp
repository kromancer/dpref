#include <cassert>
#include <cstddef>
#include <iostream>
#include <queue>
#include <span>

#include "Vertex.h"

using namespace std;

/** Computes the parent of a vertex
    in the BFS spanning tree of a given graph in CSR format,
    from a given vertex as root.
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
void bfs(const NodeID Root,
	 const size_t * __restrict Offsets,
	 const NodeID * __restrict Neighbors,
	 Vertex * __restrict Vertices) {
  
  queue<NodeID> Frontier;

  Frontier.push(Root);
  Vertices[Root].Parent = Root;

  while (!Frontier.empty()) {

    NodeID V = Frontier.front();
    Frontier.pop();

    #pragma dpref hint loopCount(5)
    for (size_t I = Offsets[V]; I < Offsets[V + 1]; I++) {

      NodeID NeighborId = Neighbors[I];
      Vertex *Neighbor = &Vertices[NeighborId];

      /* Set parent, if not set */        
      #pragma dpref slice in(V, Neighbor)
      if (Neighbor->Parent == NotAVertex) { 

	Neighbor->Parent = V;

        #pragma dpref slice out(NeighborId)
	Frontier.push(NeighborId);
      }
    }
  }
}
#pragma clang diagnostic pop

size_t Vertex::NumOfVertices = 0;

int main() {
  
  constexpr size_t NumOfVertices = 4;
  
  constexpr NodeID Neighbors[] = {
    /* Neighbors of vertex 0, index range [0, 1] */
    1, 2,

    /* Neighbors of vertex 1, index range [2, 3] */
    0, 2,
    
    /* Neighbors of vertex 2, index range [4, 4] */
    3,

    /* Neighbors of vertex 3, index range [5, 6] */
    1, 2
  };

  /*
     The neighbors of Vertex i are in the slice
     neighbor[offsets[i]:offsets[i + 1]]
   */
  constexpr size_t Offsets[] = {
    0, 2,
    4,
    5, 7
  };

  Vertex Vertices[NumOfVertices];

  bfs(0, Offsets, Neighbors, Vertices);

  for (auto &V:span<Vertex>{Vertices})
    cout << "Parent of " << V.Id << " is " << V.Parent << endl;
  
  return 0;
}
