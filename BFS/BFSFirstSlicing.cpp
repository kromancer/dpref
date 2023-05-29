#include "BFS.h"

#include <iostream>
#include <thread>
#include <tuple>

#include "Queue.h"
#include "Token.h"
#include "Vertex.h"

using namespace std;

// Arbitrarily chosen, affected by "size of MLP"
constexpr size_t QueueSize = 12;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

void firstSlice(NodeID V, const size_t *__restrict Offsets,
                const NodeID *__restrict Neighbors, Vertex *__restrict Vertices,
                dpref::Queue<tuple<NodeID, Vertex *>> *OutQ) {

  /* Inner loop, enumerate neighbors */
  for (size_t I = Offsets[V]; I < Offsets[V + 1]; I++) {

    NodeID NeighborId = Neighbors[I];

    /* Visit neighbor */
    Vertex *Neighbor = &Vertices[NeighborId];

    OutQ->push(make_tuple(V, Neighbor));
  }
}

void firstSliceThreadFunction(NodeID Root, const size_t *__restrict Offsets,
                              const NodeID *__restrict Neighbors,
                              Vertex *__restrict Vertices,
                              dpref::Queue<NodeID> *InQ,
                              dpref::Queue<tuple<NodeID, Vertex *>> *OutQ) {

  NodeID V = Root;
  OutQ->push(make_tuple(Root, &Vertices[Root]));
  firstSlice(V, Offsets, Neighbors, Vertices, OutQ);
  
  OutQ->pushTerminationToken(dpref::TerminationToken());

  for (;;) {

    auto Token = InQ->pop();

    if (Token.isTerminationToken()) {
  
      auto TermToken = Token.getTerminationToken();

      if (TermToken.isWhite()) {
        return;
      }

      TermToken.setWhite();
      OutQ->pushTerminationToken(TermToken);

    } else {

      V = Token.getData();
      firstSlice(V, Offsets, Neighbors, Vertices, OutQ);
      
    }
  }
}
#pragma clang diagnostic pop

bool secondSlice(NodeID V, Vertex *Neighbor, dpref::Queue<NodeID> *OutQ) {

  bool IsBlack = false;
  
  // Set parent, if not set
  if (Neighbor->Parent == NotAVertex) {

    Neighbor->Parent = V;

    OutQ->push(Neighbor->Id);
    IsBlack = true;
  }

  return IsBlack;
}

void secondSliceThreadFunction(dpref::Queue<tuple<NodeID, Vertex *>> *InQ,
                               dpref::Queue<NodeID> *OutQ) {

  bool IsBlack = false;

  for (;;) {

    auto Token = InQ->pop();

    if (Token.isTerminationToken()) {
      dpref::TerminationToken TermToken = Token.getTerminationToken();

      if (IsBlack) {
	TermToken.setBlack();
	OutQ->pushTerminationToken(TermToken);
	IsBlack = false;
      } else {
        OutQ->pushTerminationToken(TermToken);
        return;
      }

    } else {
      auto [V, Neighbor] = Token.getData();
      IsBlack = secondSlice(V, Neighbor, OutQ);
    }
  }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

// Arranges the dataflow network
void BFS(NodeID Root, const size_t *__restrict Offsets,
         const NodeID *__restrict Neighbors, Vertex *__restrict Vertices) {

  // from first slice -> second slice
  dpref::Queue<NodeID> Q1(QueueSize);

  // feedback Q, from second slice -> first slice
  dpref::Queue<tuple<NodeID, Vertex *>> Q2(QueueSize);

  thread FirstSliceThread(firstSliceThreadFunction, Root, Offsets, Neighbors,
                          Vertices, &Q1, &Q2);

  thread SecondSliceThread(secondSliceThreadFunction, &Q2, &Q1);

  FirstSliceThread.join();
  SecondSliceThread.join();
}
#pragma clang diagnostic pop
