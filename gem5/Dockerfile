FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt -y update && apt -y upgrade && \
    apt -y install build-essential gdb git m4 scons zlib1g zlib1g-dev \
    libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
    python3-dev doxygen libboost-all-dev libhdf5-serial-dev python3-pydot \
    libpng-dev libelf-dev pkg-config pip python3-venv black cmake ninja-build
    
RUN pip install mypy pre-commit

RUN git clone https://gem5.googlesource.com/public/gem5

WORKDIR gem5

# Single threaded compilation, to avoid "surpsises"
RUN python3 `which scons` build/ARM/gem5.debug --linker=gold -j1
RUN python3 `which scons` build/ARM/gem5.opt --linker=gold -j1
RUN python3 `which scons` build/X86/gem5.debug --linker=gold -j1
RUN python3 `which scons` build/X86/gem5.opt --linker=gold -j1
