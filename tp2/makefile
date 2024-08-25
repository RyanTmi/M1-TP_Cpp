CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++17 -DNDEBUG

#! Please modify the following INCLUDES variable to point to the location of Eigen library on your machine
INCLUDES := /opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3

all: test_matrix_power test_matrix_random

test_matrix_power: matrix_power.cpp
	$(CXX) $(CFLAGS) $^ -I$(INCLUDES) -o $@

test_matrix_random: matrix_random.cpp histogram.cpp
	$(CXX) $(CFLAGS) $^ -I$(INCLUDES) -o $@

clean:
	$(RM) test_matrix_power test_matrix_random
