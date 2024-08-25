#include <cmath>
#include <fstream>
#include <iostream>

#include "fma.hpp"
#include "geometry.hpp"

int main()
{
    // Section 2.3:
    finite_atomic_measure<double> lambda;
    {
        std::ifstream atomic_data("atomic_data.txt");
        atomic_data >> lambda;
    }

    std::cout << "*** Masse totale: " << lambda.total_mass() << "\n";

    segment<double> s1(-3.0, 1.5);
    segment<double> s2(0.5, 4.2);

    std::cout << "*** Mesure lambda([-3.0, 1.5]): " << lambda.measure(s1) << "\n";
    std::cout << "*** Mesure lambda([ 0.5, 4.2]): " << lambda.measure(s2) << "\n";

    // Section 2.4:
    finite_atomic_measure<int> mu = image(lambda, [](double x) { return static_cast<int>(std::floor(x)); });

    segment<int> s3(0, 2);
    segment<int> s4(-10, 10);
    std::cout << "*** Mesure mu([  0,  2]): " << mu.measure(s3) << "\n";
    std::cout << "*** Mesure mu([-10, 10]): " << mu.measure(s4) << "\n";
    std::cout << "*** Mesure image mu:\n" << mu << "\n";
}
