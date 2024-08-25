#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

#include "regles.hpp"
#include "urne.hpp"

int main()
{
    urne<int> urne_2;
    auto      update_polya_2 = [](std::size_t j, std::array<int, 2>& v) { ++v[j]; };

    std::random_device rd;
    std::mt19937_64    g(rd());

    const std::size_t   size_prop = 10000;
    std::vector<double> prop(size_prop);

    std::generate(prop.begin(), prop.end(), [&]() {
        urne_2.reset();
        urne_2.maj_p_fois(update_polya_2, g, 1000);
        std::cout << urne_2 << '\n';
        return urne_2.fraction(0);
    });

    // std::sort(prop.begin(), prop.end());
    // for (std::size_t i = 0; i < 10; ++i)
    // {
    //     std::cout << prop[size_prop / 10 * i] << '\n';
    // }

    urne<long, 7> urne_7;
    std::cout << urne_7 << '\n';

    urne_7.maj(update_polya_n<long, 7>, g);
    std::cout << urne_7 << '\n';
}
