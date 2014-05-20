targets = demo_integer demo_natnum

all: $(targets)

%: %.cpp
	g++ -I.. -std=c++11 $^ -o $@

clean:
	rm -rf $(targets)

.PHONY: all clean

