#include "linear_combination.hpp"

#include <array>
#include <iostream>
#include <vector>

void test_solve_system();

int main()
{
    std::vector<double> coeff {2.0, -1.0};

    std::array<double, 3> v1 = {1.0, 2.0, 3.0};
    std::array<double, 3> v2 = {4.0, 5.0, 6.0};
    std::array<double, 3> w = {-2.0, -1.0, 0.0};

    linear_combination<double> lc1(coeff);

    // Evaluation
    std::vector<std::array<double, 3>> argument {v1, v2};
    std::array<double, 3>              res = lc1(argument);

    if ((res - w) == std::array<double, 3>())
    {
        std::cout << "w est une combinaison linéaire de v1 et v2" << std::endl;
    }
    else
    {
        std::cout << "w n'est une combinaison linéaire de v1 et v2" << std::endl;
    }

    coeff = std::vector<double>({-3., 4., 5.});
    std::array<double, 3> v3 = {10., -4., 1.};

    linear_combination<double> lc2(coeff);

    // Evaluation
    argument = std::vector<std::array<double, 3>>({v1, v2, v3});
    res = lc2(argument);

    if ((res - w) == std::array<double, 3>())
    {
        std::cout << "w est une combinaison linéaire de v1, v2 et v3" << std::endl;
    }
    else
    {
        std::cout << "w n'est pas une combinaison linéaire de v1,v2 et v3 avec ces coefficients" << std::endl;
    }

    // Affichage sur le terminal
    // w est une combinaison linéaire de v1 et v2
    // w n'est pas une combinaison linéaire de v1,v2 et v3 avec ces coefficients
    test_solve_system();
}

void test_solve_system()
{
    // std::vector<std::vector<std::vector<double>>> vs = {
    //     {{1, 2, 4}, {-1, 1, 3},  {1, -1, 1}},
    //     {{1, 3, 8}, {2, -1, 2}, {-3, 2, -2}},
    //     {{1, 3, 1},  {2, 4, 2},  {3, 5, -2}},
    // };
    std::vector<std::vector<std::vector<double>>> vs = {
        {{1, -1, 1}, {2, 1, -1},  {4, 3, 1}},
        {{1, 2, -3}, {3, -1, 2}, {8, 2, -2}},
        { {1, 2, 3},  {3, 4, 5}, {1, 2, -2}},
    };

    std::vector<std::array<double, 3>> ws = {{{2, 1, 3}}, {{-1, 7, 9}}, {{4, 6, -7}}};

    for (std::size_t i = 0; i < 3; ++i)
    {
        std::vector<linear_combination<double>> ls = {
            linear_combination<double>(vs[i][0]),
            linear_combination<double>(vs[i][1]),
            linear_combination<double>(vs[i][2]),
        };
        auto result = solve_system(ls, ws[i]);
        std::cout << "w combinaison linaire de v1, v2, et v3 : " << result.first << '\n';
        if (result.first)
        {
            for (auto x : result.second)
            {
                std::cout << x << ' ';
            }
            std::cout << '\n';
        }
    }
}
