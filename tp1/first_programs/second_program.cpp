#include <fstream>
#include <iostream>
#include <vector>

int main()
{
    int n;

    std::cout << "Entrez un nombre entier <100:" << std::endl;
    std::cin >> n;
    std::vector<int> tableau(n);

    for (std::size_t i = 0; i < tableau.size(); ++i)
    {
        tableau[i] = i * i;
    }

    std::ofstream fichier("donnees.dat");
    fichier << "Voici les carrés et les cubes des entiers:" << std::endl;
    for (int i = tableau.size() - 1; i >= 0; --i)
    {
        fichier << i << ": " << tableau[i] << " " << tableau[i] * i << std::endl;
    }
    fichier.close();

    return 0;
}
