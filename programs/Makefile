# $Id$

LIBPATH = ../src
COMMON = $(LIBPATH)/natural_number.hpp $(LIBPATH)/natural_number_algorithms.hpp $(LIBPATH)/integer.hpp $(LIBPATH)/detail/digit_vector.hpp $(LIBPATH)/detail/lowlevel.hpp $(LIBPATH)/detail/lowlevel_generic.hpp $(LIBPATH)/detail/lowlevel_gcc_x86.hpp

DEVFLAGS = -g -Wall -fmessage-length=0 -I$(LIBPATH)
OPTFLAGS = -O2 -DNDEBUG -Wall -fmessage-length=0 -I$(LIBPATH)

examples: examples.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

tests: tests.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

benchmarks: benchmarks.cpp $(COMMON)
	$(CXX) $(OPTFLAGS) -o $@ $<
    
all: examples tests benchmarks

clean:
	rm -f *.exe
