#include "melange_gaussien.hpp"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include <Eigen/Dense>

template <typename T>
void estimate_params(mat<T>& phi, vec<T>& x, vec<T>& m, vec<T>& s, T sigma, double threshold);

int main()
{
    std::mt19937_64 g(time(0));

    melange_gaussien<double> mg;

    double                   sigma = std::sqrt(5);
    melange_gaussien<double> mixture({0.3, 0.3, 0.4}, sigma);

    std::cout << "Taille du mélange : " << mixture.size() << '\n';
    std::cout << "Mélange :\n" << mixture << '\n';

    mixture.update_components(g);
    std::cout << "Mélange après update:\n" << mixture << '\n';

    std::size_t n = 10000;

    auto obs = mixture.generate_observations(g, n);
    std::cout << "Affichage des 10 premières observations générées" << '\n';
    for (std::size_t i = 0; i < 10; ++i)
    {
        std::cout << obs[i] << ' ';
    }
    std::cout << '\n';

    std::size_t k = mixture.size();

    std::cout << "Test convergence :" << '\n';
    mat<double> phi(n, k);
    vec<double> s(k);
    vec<double> m(k);
    vec<double> x = Eigen::Map<vec<double>>(obs.data(), obs.size());

    phi.setRandom();  // Random values between -1 and 1
    phi = (phi.array() + 1).matrix() / 2.0;
    s.setRandom();
    s = (s.array() + 1).matrix() / 2.0;

    std::normal_distribution<double> nd;
    for (std::size_t i = 0; i < k; ++i)
    {
        m(i) = nd(g);
    }

    std::cout << "m =\n";
    std::cout << m << '\n';
    for (std::size_t i = 0; i < 100; ++i)
    {
        update_phi(phi, x, m, s);
        update_m(m, x, phi, sigma);
        update_s(s, phi, sigma);
    }
    std::cout << "m =\n";
    std::cout << m << '\n';

    estimate_params(phi, x, m, s, sigma, 1e-9);
}

template <typename T>
void estimate_params(mat<T>& phi, vec<T>& x, vec<T>& m, vec<T>& s, T sigma, double threshold)
{
    constexpr std::size_t max_iter = 1000;
    vec<T>                last_iter;

    std::cout << "m =\n";
    std::cout << m << '\n';
    for (std::size_t i = 0; i < max_iter; ++i)
    {
        last_iter = m;
        update_phi(phi, x, m, s);
        update_m(m, x, phi, sigma);
        update_s(s, phi, sigma);

        vec<T> diff = last_iter - m;
        if (std::all_of(diff.begin(), diff.end(), [threshold](T x) { return x <= threshold; }))
        {
            break;
        }
    }
    std::cout << "m =\n";
    std::cout << m << '\n';
}
