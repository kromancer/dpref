#pragma once

#include "Vertex.h"

void bfs(const NodeID Root,
	 const size_t * __restrict Offsets,
	 const NodeID * __restrict Neighbors,
	 Vertex * __restrict Vertices);
