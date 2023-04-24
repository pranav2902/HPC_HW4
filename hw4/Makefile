CXX = mpic++ # or mpicxx
CXXFLAGS = -std=c++11 -O3 -march=native -fopenmp
PROCESSES = 3
TARGETS = $(basename $(wildcard *.cpp))

int_ring:int_ring.cpp
        $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@

pingpong:pingpong.cpp
        $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@ 1 2        

int_ring_arr:int_ring_arr.cpp
        $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@

MPI_scan:MPI_scan.cpp
        $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@ 4        

clean:
        -$(RM) $(TARGETS) *~

.PHONY: all, clean
