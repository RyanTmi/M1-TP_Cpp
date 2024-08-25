#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>

#include "permutation.hpp"

int main()
{
    // ******* Premiere partie: bases de la bibliothèque

    // Déclaration de deux permutations de deux manières différentes:
    permutation b(6);  // Identité

    std::vector<std::size_t> v {2, 4, 5, 3, 1, 0};
    permutation              a(v);

    // Calcul des itérées d'une permutation
    for (std::size_t i = 0; i <= 6; ++i)
    {
        std::cout << "a^" << i << '\n' << b << '\n';
        b = b * a;
    }

    // Calcul des points fixes d'une permutation
    std::list<std::size_t> fp = a.fixed_points();
    std::copy(fp.begin(), fp.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
    std::cout << '\n';

    // ******* Deuxieme partie: un peu plus d'algorithmique

    // Lecture de deux permutations dans deux fichiers
    permutation   s;
    std::ifstream fichier_s("file_s.dat");
    fichier_s >> s;
    fichier_s.close();

    permutation   t;
    std::ifstream fichier_t("file_t.dat");
    fichier_t >> t;
    fichier_t.close();

    // Calcul de cycles et ordres
    std::list<cycle> a_cycles = a.cycles();
    std::cout << "Les cycles de a sont: ";
    std::copy(a_cycles.begin(), a_cycles.end(), std::ostream_iterator<cycle>(std::cout));
    std::cout << "\nL'ordre de la permutation a est égal à " << a.order() << '\n';

    // Calcul de u = s * t^{-1} et son ordre
    permutation u = s * t.inverse();
    std::size_t o = u.order();
    std::cout << "L'ordre de la permutation s*t^-1 est égal à " << o << '\n';

    // Extraction des cycles de u
    std::list<cycle> u_cycles = u.cycles();
    std::cout << "Cette permutation a " << u_cycles.size() << " cycles, dont le plus grand a pour longueur "
              << std::max_element(u_cycles.begin(), u_cycles.end())->order() << '\n';
    // Attention, cela utilise < sur des cycle !

    // ******* Troisieme partie: génération aléatoire et Monte-Carlo
    std::random_device rd;
    std::mt19937_64    g(rd());

    constexpr unsigned n = 100;
    constexpr unsigned nb_echant = 10000;
    unsigned           nb_derang = 0;

    for (unsigned i = 0; i < nb_echant; ++i)
    {
        nb_derang += permutation(n, g).is_derangement();
    }

    std::cout << "La proportion de dérangements est environ " << nb_derang / static_cast<double>(nb_echant) << '\n';
}
