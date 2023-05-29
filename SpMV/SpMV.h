#pragma once

#include <cstddef>

void SpMV(const size_t N,
	  const size_t * __restrict Offsets,
	  const size_t * __restrict ColIdx,
	  const double * __restrict Values,
	  const double * __restrict X,
	  double * __restrict Y);
