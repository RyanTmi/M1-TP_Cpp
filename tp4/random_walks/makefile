CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -O2 -std=c++11

all: test_random_walk

test_random_walk: test_random_walk.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_random_walk
