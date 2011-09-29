ROOTDIR = ..

INCLUDES = ${ROOTDIR}/

test_type_traits: ../tests/test_type_traits.cpp
	g++ ../tests/test_type_traits.cpp -o $@ -I..

example: examples.cpp
	g++ examples.cpp -o $@ -I..

clean:
	rm -f test_type_traits example
