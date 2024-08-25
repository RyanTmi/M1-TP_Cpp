#include "linear_combination.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    std::ifstream data_file("data.txt");
    auto          xys = read_data(data_file);
    data_file.close();

    auto cos_fct = [](double x) { return std::cos(6.0 * x); };
    auto exp_fct = [](double x) { return std::exp(4.0 * x); };
    auto cos_v = app_fct(cos_fct, xys.first);
    auto exp_v = app_fct(exp_fct, xys.first);

    std::vector<std::vector<double>> abs = {
        { -2.0, 0.1},
        {  2.0, 0.9},
        {-10.0, 0.2},
    };

    std::size_t         size = cos_v.size();
    std::vector<double> values(size);
    std::vector<double> ly(size);
    for (const auto& ab : abs)
    {
        linear_combination<double> l(ab);

        std::transform(cos_v.begin(), cos_v.end(), exp_v.begin(), values.begin(), [&l](double x1, double x2) {
            return l(std::vector<double> {x1, x2});
        });

        std::transform(values.begin(), values.end(), xys.second.begin(), ly.begin(), [](double x1, double x2) {
            return (x1 - x2) * (x1 - x2);
        });

        std::cout << std::accumulate(ly.begin(), ly.end(), 0.0) << '\n';
    }
}
