CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++11

#! Please modify the following INCLUDES variable to point to the location of Eigen library on your machine
INCLUDES := /opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3

all: test_melange_gaussien

test_melange_gaussien: test_melange_gaussien.cpp
	$(CXX) $(CFLAGS) $^ -I$(INCLUDES) -o $@

clean:
	$(RM) test_melange_gaussien
