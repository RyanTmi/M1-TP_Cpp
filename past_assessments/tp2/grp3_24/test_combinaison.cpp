#include "linear_combination.hpp"

#include <iostream>

int main()
{
    // Combinaison linéaire
    linear_combination<int> c;
    std::cout << "Combinaison par défaut : " << c << std::endl;

    std::vector<int>        v1 {1, 0, 1};
    linear_combination<int> c1(v1);

    std::cout << "C1 : " << c1 << std::endl;
    std::cout << "La taille de C1 est égale à " << c1.size() << " et son 2ème élément est égal à " << c1[1] << ".\n";

    linear_combination<double> c2(6);
    c2[3] = 2.4;
    std::cout << "C2 : " << c2 << std::endl;

    std::vector<double> eval {1.0, 2, 3};
    std::cout << "Evaluation : " << c1(eval) << std::endl;

    // Affichage terminal
    // Combinaison par défaut :
    // C1 : x_0 + 0*x_1 + x_2
    // La taille de C1 est égale à 3 et son 2ème élément est égal à 0.
    // C2 : 0*x_0 + 0*x_1 + 0*x_2 + 2.4*x_3 + 0*x_4 + 0*x_5
    // Evaluation : 4
}
