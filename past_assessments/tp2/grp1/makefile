CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++17

all: test_fma test_input

test_fma: test_fma.cpp
	$(CXX) $(CFLAGS) $^ -o $@

test_input: test_input.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_fma test_input
