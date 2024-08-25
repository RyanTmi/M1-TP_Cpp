#include <fstream>
#include <iostream>
#include <random>

#include "loi_melange.hpp"

melange<std::normal_distribution<>> read_from_file(const char* file_name)
{
    std::ifstream melange_gauss_1_file(file_name);
    std::size_t   count = 0;

    melange_gauss_1_file >> count;
    std::vector<std::normal_distribution<>::param_type> params(count);
    std::vector<double>                                 weights(count);

    for (std::size_t i = 0; i < count; ++i)
    {
        double m = 0.0;
        double v = 0.0;

        melange_gauss_1_file >> weights[i];
        melange_gauss_1_file >> m >> v;
        params[i] = std::normal_distribution<>::param_type(m, v);
    }

    return melange<std::normal_distribution<>>(params, weights);
}

int main()
{
    std::random_device rd;
    std::mt19937_64    g(rd());

    melange<std::bernoulli_distribution> m_bernoulli;
    m_bernoulli.print_type();

    m_bernoulli.add_mode(1.0, std::bernoulli_distribution(0.5));
    m_bernoulli.add_mode(0.3, std::bernoulli_distribution(0.2));

    std::cout << "Melange bernoulli:" << '\n';
    std::cout << m_bernoulli << '\n';

    std::cout << "Realisation: " << m_bernoulli(g) << '\n';

    double me = 0.0;
    for (std::size_t i = 0; i < 100000; ++i)
    {
        me += m_bernoulli(g);
    }
    me /= 100000;
    double mt = 0.5 * 0.7 + 0.2 * 0.3;

    std::cout << "me - mt: " << me - mt << '\n';

    auto melange_gauss_1 = read_from_file("melange_gauss_1.txt");
    auto melange_gauss_2 = read_from_file("melange_gauss_2.txt");

    auto melange_gauss = melange_gauss_1 + melange_gauss_2;
}
