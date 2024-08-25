#include <cmath>
#include <iostream>

double circle_area(double r)
{
    return M_PI * r * r;
}

double square_area(double c)
{
    return c * c;
}

int main()
{
    double r;
    std::cout << "Entrez le rayon du cercle: " << std::endl;
    std::cin >> r;
    std::cout << "Son aire est " << circle_area(r) << std::endl;

    std::cout << "Entrez la longueur du coté du carré: " << std::endl;
    std::cin >> r;
    std::cout << "Son aire est " << square_area(r) << std::endl;
    return 0;
}
