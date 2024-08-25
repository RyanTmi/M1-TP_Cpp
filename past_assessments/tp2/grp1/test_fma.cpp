#include <cmath>
#include <iostream>

#include "fma.hpp"
#include "geometry.hpp"

int main()
{
    // Section 2.1:
    finite_atomic_measure<int> mu;
    mu.add_mass(3, 1.);
    mu.add_mass(5, 2.);
    mu.add_mass(8, 0.5);

    std::cout << "*** Masse totale: " << mu.total_mass() << "\n";
    std::cout << "*** Nb points: " << mu.nb_of_atoms() << "\n";
    std::cout << "*** Mesure nu:\n" << mu << "\n";
    // fin section 2.1

    // Section 2.2:
    segment<int> s1(4, 9);
    segment<int> s2(-3, 6);
    segment<int> s3(-3, 0);
    std::cout << "*** Mesure mu([ 4, 9]): " << mu.measure(s1) << "\n";  // attendu: 2.5
    std::cout << "*** Mesure mu([-3, 6]): " << mu.measure(s2) << "\n";  // attendu: 3.0
    std::cout << "*** Mesure mu([-3, 0]): " << mu.measure(s3) << "\n";  // attendu: 0.0
    std::cout << "*** Intégrale de sqrt: " << mu.integral([](int n) { return sqrt(n); }) << "\n";
    // fin section 2.2

    // Section 2.4, question 14:
    finite_atomic_measure<double> imu = image(mu, [](int z) { return (z - 4) * (z - 4) + M_PI; });
    std::cout << "*** Mesure image de mu:\n" << imu << "\n";

    // Section 2.5
    std::array<int, 3>         m {3, 5, 10};
    finite_atomic_measure<int> delta(m.cbegin(), m.cend());
    std::cout << "*** Mesure delta:\n" << delta << "\n";
    std::cout << "*** Mesure mu + delta:\n" << mu + delta << "\n";
}
