CXX      := g++
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c++11

all: test_combinaison test_fonction test_vecteur

test_combinaison: test_combinaison.cpp
	$(CXX) $(CFLAGS) $^ -o $@

test_fonction: test_fonction.cpp
	$(CXX) $(CFLAGS) $^ -o $@

test_vecteur: test_vecteur.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_combinaison test_fonction test_vecteur
