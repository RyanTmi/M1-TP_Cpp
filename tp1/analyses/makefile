CXX    := g++
CFLAGS := -Wall -Wextra -pedantic -O2 -std=c++11

all: analysis

analysis: analysis.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) analysis
