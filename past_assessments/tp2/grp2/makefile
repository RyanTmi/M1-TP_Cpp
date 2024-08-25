CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++11

all: test_melange

test_melange: main.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_melange
