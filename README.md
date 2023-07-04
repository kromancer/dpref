# DPREF: Decoupling using Dead Code Elimination for Prefetching Irregular Memory Accesses

To clone this repository along with its dependencies:

``` bash
git clone --recurse-submodules git@github.com:kromancer/dpref.git
```

This repository supports the extended abstract sumbission for [ACACES
2023](https://www.hipeac.net/acaces/2023/#/). The extended abstract and
the corresponding poster can be found in `./doc`

# Demos

- `SpMV`: A naive implementation of the Sparse Matrix Vector
  multiplication. The baseline is **manually** sliced twice to
  demonstate the **scalabillity** of the approach.

- `BFS`: A naive implementation of the Breadth First Search, manually
  sliced once. The purpose of this demo is to demonstrate the
  **applicability** of our approach, by handling loops in the dataflow
  via distributed termination detection

To build the demos:

``` bash
cmake -GNinja -B build && cmake --build build
```
