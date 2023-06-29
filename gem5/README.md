# Running the benchmarks on gem5

Pre-built docker images with gem5 compiled for `ARM` and `X86` simulated
platforms exist here:
<https://hub.docker.com/repository/docker/kromancer/gem5/tags>

You can find the corresponding `Dockerfile` in this directory.

To pull one:

``` bash

# If you have apple silicon
docker pull kromancer/gem5:latest-arm64

# If you have intel/amd
docker pull kromancer/gem5:latest-x86

```

Run a container, compile the applications and run them on gem5 as such:

``` bash
docker run -it -v <path to this repository>:/dpref gem5
cd /dpref
cmake -GNinja -B build-gem5
cmake --build build-gem5
../gem5/build/<ARM or X86>/gem5.opt ../gem5/configs/example/se.py --caches --num-cpus 3 --cmd build-gem5/SpMV/SpMVFirstSlicing
```
