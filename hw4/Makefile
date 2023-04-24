PRECURSOR = module load openmpi/gcc/4.0.5
CXX = mpic++ # or mpicxx
CXXFLAGS = -std=c++11 -O3 -march=native -fopenmp
PROCESSES = 3
TARGETS = $(basename $(wildcard *.cpp))

all : $(TARGETS)

int_ring:int_ring.cpp
	$(PRECURSOR) && $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@

pingpong:pingpong.cpp
	$(PRECURSOR) && $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@ 1 2

int_ring_arr:int_ring_arr.cpp
	$(PRECURSOR) && $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@

MPI_scan:MPI_scan.cpp
	$(PRECURSOR) && $(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ && mpirun -np $(PROCESSES) $@ 4
