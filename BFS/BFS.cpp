#include "BFS.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <queue>
#include <span>

#include "Vertex.h"

using namespace std;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
void BFS(const VertexID Root, const size_t *__restrict Offsets,
         const VertexID *__restrict Neighbors, Vertex *__restrict Vertices) {

  queue<VertexID> Frontier;

  Frontier.push(Root);
  Vertices[Root].Parent = Root;

  while (!Frontier.empty()) {

    VertexID V = Frontier.front();
    Frontier.pop();

#pragma dpref hint loopCount(5)
    for (size_t I = Offsets[V]; I < Offsets[V + 1]; I++) {

      VertexID NeighborId = Neighbors[I];
      Vertex *Neighbor = &Vertices[NeighborId];


#pragma dpref slice in(V, Neighbor)
      if (Neighbor->Parent == NotAVertex) {
        // Set parent, if not set
        Neighbor->Parent = V;

#pragma dpref slice out(NeighborId)
        Frontier.push(NeighborId);
      }
    }
  }
}
#pragma clang diagnostic pop
