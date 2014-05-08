TARGETS = test_natnum_vs_gmp test_int_vs_gmp lowlevel_perf lowlevel_test performance_test

all: $(TARGETS)

test_natnum_vs_gmp: test_natnum_vs_gmp.cpp
	g++ -O2 $< -o $@ -I .. -lgmp -lboost_system -lboost_chrono

test_int_vs_gmp: test_int_vs_gmp.cpp
	g++ -O2 $< -o $@ -I .. -lgmp -lboost_system -lboost_chrono

lowlevel_perf: lowlevel_perf.cpp
	g++ -O2 $< -o $@ -I .. -lboost_system -lboost_chrono

lowlevel_test: lowlevel_test.cpp
	g++ -O2 $< -o $@ -I .. -lboost_system -lboost_chrono

performance_test: performance_test.cpp
	g++ -O2 $< -o $@ -I .. -lboost_system -DTEST_CPP_INT -DTEST_KANOOTH_INT

clean:
	rm -f $(TARGETS)

.PHONY: all clean
