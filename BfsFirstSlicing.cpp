#include <iostream>
#include <thread>
#include <tuple>

#include "Vertex.h"
#include "Queue.h"

using namespace std;

// Arbitrarily chosen, affected by "size of MLP"
constexpr size_t QueueSize = 12;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


void firstSlice(const NodeID V,
		const size_t *__restrict Offsets,
		const NodeID *__restrict Neighbors,
		Vertex *__restrict Vertices,
		dpref::Queue<tuple<NodeID, Vertex *>> *OutQ) {
  
    /* Inner loop, enumerate neighbors */
    for (size_t I = Offsets[V]; I < Offsets[V + 1]; I++) {

      NodeID NeighborId = Neighbors[I];

      /* Visit neighbor */
      Vertex *Neighbor = &Vertices[NeighborId];

      OutQ->push(make_tuple(V, Neighbor));
    }
}

[[noreturn]] void firstSliceThreadFunction(dpref::Queue<NodeID> *InQ,
					   dpref::Queue<tuple<NodeID, Vertex *>> *OutQ,
					   const NodeID Root,
					   const size_t *__restrict Offsets,
					   const NodeID *__restrict Neighbors,
					   Vertex *__restrict Vertices) {

  NodeID V = Root;
  OutQ->push(make_tuple(Root, &Vertices[Root]));

  for (;;) {

    firstSlice(V, Offsets, Neighbors, Vertices, OutQ);

    auto Token = InQ->pop();
    V = Token.getData();
  }

}
#pragma clang diagnostic pop

void secondSlice(NodeID V, Vertex *Neighbor, dpref::Queue<NodeID> *OutQ) {

    // Set parent, if not set
    if (Neighbor->Parent == NotAVertex) {

      Neighbor->Parent = V;

      cout << "secondSlice: Parent of: " << Neighbor->Id << " is: " << V << endl;

      OutQ->push(Neighbor->Id);
    }

}

[[noreturn]] void secondSliceThreadFunction(dpref::Queue<tuple<NodeID, Vertex *>> *InQ,
					    dpref::Queue<NodeID> *OutQ) {

  for (;;) {
    
    auto Token = InQ->pop();    
    auto [V, Neighbor] = Token.getData();

    secondSlice(V, Neighbor, OutQ);

  }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

// Arranges the dataflow network
void bfs(const NodeID Root,
    const size_t *__restrict Offsets,
    const NodeID *__restrict Neighbors,
    Vertex *__restrict Vertices) {

  // from first slice -> second slice
  dpref::Queue<NodeID> Q1(QueueSize);

  // feedback Q, from second slice -> first slice
  dpref::Queue<tuple<NodeID, Vertex *>> Q2(QueueSize);

  thread FirstSliceThread(firstSliceThreadFunction, &Q1, &Q2, Root,
			  Offsets, Neighbors, Vertices);

  thread SecondSliceThread(secondSliceThreadFunction, &Q2, &Q1);

  FirstSliceThread.join();
  SecondSliceThread.join();
}
#pragma clang diagnostic pop

size_t Vertex::NumOfVertices = 0;

int main() {

  constexpr size_t NumOfVertices = 4;

  constexpr size_t Neighbors[] = {
      /* Neighbors of vertex 0, index range [0, 1] */
      1, 2,

      /* Neighbors of vertex 1, index range [2, 3] */
      0, 2,

      /* Neighbors of vertex 2, index range [4, 4] */
      3,

      /* Neighbors of vertex 3, index range [5, 6] */
      1, 2};

  /*
     The neighbors of Vertex i are in the slice
     neighbor[offsets[i]:offsets[i + 1]]
   */
  constexpr size_t Offsets[] = {0, 2, 4, 5, 7};

  Vertex Vertices[NumOfVertices];

  bfs(0, Offsets, Neighbors, Vertices);

  return 0;
}
