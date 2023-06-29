#pragma once

#include <cstddef>
#include <cstdint>

constexpr size_t NotAVertex = SIZE_MAX;

typedef size_t VertexID;

class Vertex {
public:
  size_t Parent;
  VertexID Id;

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
