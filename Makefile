DEVFLAGS = -g -Wall -fmessage-length=0

COMMON = NonNegativeInteger.hpp

examples.exe: examples.cpp $(COMMON)
	$(CXX) $(DEVFLAGS) -o $@ $<

all: examples.exe

clean:
	rm -f examples.exe
