# $Id$

DEVFLAGS = -g -Wall -fmessage-length=0
OPTFLAGS = -O2 -DNDEBUG -Wall -fmessage-length=0

COMMON = NonNegativeInteger.hpp NNIutils.hpp detail/SimpleDigitVector.hpp detail/lowlevel.hpp detail/lowlevel_generic.hpp detail/lowlevel_gcc_x86.hpp

examples.exe: examples.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

tests.exe: tests.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

benchmarks.exe: benchmarks.cpp $(COMMON)
	$(CXX) $(OPTFLAGS) -o $@ $<

all: examples.exe tests.exe benchmarks.exe

clean:
	rm -f examples.exe tests.exe benchmarks.exe
