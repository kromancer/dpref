#pragma once

#include <climits>
#include <cstddef>

constexpr size_t NotAVertex = SIZE_T_MAX;

typedef size_t NodeID;


class Vertex {
public:
  size_t Parent;
  NodeID Id;

  Vertex(): Parent{NotAVertex}, Id{NumOfVertices++} {}
  ~Vertex() {
    --NumOfVertices;
  }

  static size_t getNumOfVertices() {
    return NumOfVertices;
  }
  
private:
  static size_t NumOfVertices;

};
