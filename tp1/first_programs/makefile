CXX    := g++
CFLAGS := -Wall -Wextra -pedantic -O2 -std=c++11

all: first_program second_program

first_program: first_program.cpp
	$(CXX) $(CFLAGS) $^ -o $@

second_program: second_program.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) first_program second_program
