CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++11

all: test_permutation test_sparse

test_permutation: test_permutation.cpp permutation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

test_sparse: test_sparse.cpp permutation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_permutation test_sparse
