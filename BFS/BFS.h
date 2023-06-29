#pragma once

#include <cstddef>

#include "Vertex.h"

void BFS(const VertexID Root,
	 const size_t * __restrict Offsets,
	 const VertexID * __restrict Neighbors,
	 Vertex * __restrict Vertices);
