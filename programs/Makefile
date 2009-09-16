# $Id$

LIBPATH = ../src
COMMON = $(LIBPATH)/nonnegative_integer.hpp $(LIBPATH)/nni_utils.hpp $(LIBPATH)/integer.hpp $(LIBPATH)/detail/simple_digit_vector.hpp $(LIBPATH)/detail/lowlevel.hpp $(LIBPATH)/detail/lowlevel_generic.hpp $(LIBPATH)/detail/lowlevel_gcc_x86.hpp

DEVFLAGS = -g -Wall -fmessage-length=0 -I$(LIBPATH)
OPTFLAGS = -O2 -DNDEBUG -Wall -fmessage-length=0 -I$(LIBPATH)

examples.exe: examples.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

test_nni.exe: test_nni.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

benchmarks.exe: benchmarks.cpp $(COMMON)
	$(CXX) $(OPTFLAGS) -o $@ $<
    
all: examples.exe test_nni.exe benchmarks.exe

clean:
	rm -f examples.exe test_nni.exe benchmarks.exe
