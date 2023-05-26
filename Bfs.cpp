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
