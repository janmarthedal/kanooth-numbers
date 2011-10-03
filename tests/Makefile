GCC=g++
BASE=..
CFLAGS=-Wall -I${BASE}
DEPS= \
  ${BASE}/kanooth/shared_ptr.hpp \
  ${BASE}/kanooth/type_traits.hpp \
  ${BASE}/kanooth/numbers/common_functions.hpp \
  ${BASE}/kanooth/numbers/number_theory.hpp \
  ${BASE}/kanooth/numbers/positive_float.hpp \
  ${BASE}/kanooth/numbers/number_traits.hpp \
  ${BASE}/kanooth/numbers/natural_number.hpp \
  ${BASE}/kanooth/numbers/integer.hpp \
  ${BASE}/kanooth/numbers/detail/named_ops.hpp \
  ${BASE}/kanooth/numbers/detail/lowlevel/generic.hpp \
  ${BASE}/kanooth/numbers/detail/lowlevel/gmp.hpp \
  ${BASE}/kanooth/numbers/detail/integer/int_abst.hpp \
  ${BASE}/kanooth/numbers/detail/integer/int_impl.hpp \
  ${BASE}/kanooth/numbers/detail/default_ops.hpp \
  ${BASE}/kanooth/numbers/detail/number_abst.hpp \
  ${BASE}/kanooth/numbers/detail/positive_float/pos_float_impl.hpp \
  ${BASE}/kanooth/numbers/detail/positive_float/pos_float_abst.hpp \
  ${BASE}/kanooth/numbers/detail/memory/array_allocator.hpp \
  ${BASE}/kanooth/numbers/detail/memory/digit_array.hpp \
  ${BASE}/kanooth/numbers/detail/natural_number/nat_num_impl.hpp \
  ${BASE}/kanooth/numbers/detail/natural_number/nat_num_abst.hpp \
  ${BASE}/kanooth/numbers/detail/natural_number/nat_num_shared.hpp \
  ${BASE}/kanooth/numbers/detail/overload.hpp \

all: test_type_traits test_return_types examples

test_type_traits: test_type_traits.cpp ${DEPS}
	${GCC} $< -o $@ ${CFLAGS}

test_return_types: test_return_types.cpp ${DEPS}
	${GCC} $< -o $@ ${CFLAGS}
	
test_return_types.cpp: return_types.txt
	perl make_return_type_test.pl $< > $@

examples: dev/examples.cpp ${DEPS}
	${GCC} $< -o $@ -g ${CFLAGS}

clean:
	rm -f examples test_type_traits test_return_types

