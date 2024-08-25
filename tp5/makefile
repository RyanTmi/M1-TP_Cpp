CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++11

all: test_monte_carlo

test_monte_carlo: simulation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_monte_carlo
