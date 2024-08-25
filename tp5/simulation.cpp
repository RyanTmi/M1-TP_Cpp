#include <fstream>
#include <iostream>
#include <random>

#include "chi2.hpp"
#include "mcmc.hpp"
#include "monte_carlo.hpp"
#include "pi.hpp"

template <typename RNG>
void mc_pi(std::size_t n, RNG& g);

template <typename RNG>
void mc_i1(std::size_t n, RNG& g);

template <typename RNG>
void mc_i2(std::size_t n, RNG& g);

template <typename RNG>
void mc_normal(std::size_t n, RNG& g);

template <typename RNG>
void mc_chi2(std::size_t n, RNG& g);

template <typename RNG>
void mc_mc(std::size_t n, RNG& g);

template <typename RNG>
void mc_ising(std::size_t n, RNG& g);

int main()
{
    constexpr std::size_t n = 100000000;

    std::random_device rd;
    std::mt19937_64    g(rd());

    mc_pi(n, g);
    mc_i1(n, g);
    mc_i2(n, g);
    mc_normal(n, g);
    mc_chi2(n, g);
    mc_mc(n, g);
    mc_ising(n, g);
}

template <typename RNG>
void mc_pi(std::size_t n, RNG& g)
{
    square_in_disk a;
    double         pi_approx = 0;

    auto f = [](double x) { return 4 * x; };
    monte_carlo(pi_approx, a, f, g, n);
    std::cout << pi_approx << '\n';

    double_mean_var mv;
    monte_carlo(mv, a, f, g, n);
    std::cout << "[" << mv.mean() - 1.96 * std::sqrt(mv.var() / n) << "; " << mv.mean() + 1.96 * std::sqrt(mv.var() / n)
              << "]\n";
}

template <typename RNG>
void mc_i1(std::size_t n, RNG& g)
{
    std::uniform_real_distribution<double> u(0, 1);
    auto                                   f = [](double x) { return std::log(1 + x * x); };

    double i = 0;
    monte_carlo(i, u, f, g, n);
    std::cout << "I1 = " << i << '\n';
}

template <typename RNG>
void mc_i2(std::size_t n, RNG& g)
{
    std::exponential_distribution<>  e(1);
    std::uniform_real_distribution<> u(0, 1);

    auto couple_var = [&](RNG& g) { return std::make_pair(e(g), u(g)); };
    auto f = [](std::pair<double, double> p) { return std::log(1 + p.first * p.second); };

    double i = 0;
    monte_carlo(i, couple_var, f, g, n);
    std::cout << "I2 = " << i << '\n';
}

template <typename RNG>
void mc_normal(std::size_t n, RNG& g)
{
    histogram                  h(-3, 3, 50);
    std::normal_distribution<> nd(0, 1);
    monte_carlo(h, nd, [](double x) { return x; }, g, n);

    std::ofstream out("histogram.dat");
    out << h;
}

template <typename RNG>
void mc_chi2(std::size_t n, RNG& g)
{
    histogram                    h1(0, 10, 50);
    histogram                    h2(0, 10, 50);
    chi2_distribution<double, 3> chi3;
    chi2_distribution<double, 6> chi6;
    auto                         id = [](double x) { return x; };

    monte_carlo(h1, chi3, id, g, n);
    std::ofstream out3("chi3.dat");
    out3 << h1;

    monte_carlo(h2, chi6, id, g, n);
    std::ofstream out6("chi6.dat");
    out6 << h2;
}

template <typename RNG>
void mc_mc(std::size_t n, RNG& g)
{
    double          aa = 0.7;
    double          bb = 0.2;
    markov_2_states mc(1, aa, bb);
    stat_2_states   s;
    auto            id = [](double x) { return x; };

    monte_carlo(s, mc, id, g, n);
    std::cout << bb / (aa + bb) << ' ' << aa / (aa + bb) << '\n';
    std::cout << s << '\n';
}

template <typename RNG>
void mc_ising(std::size_t n, RNG& g)
{
    auto f = [](const ising_1d<1000>::state& s) { return s[500 - 1]; };

    for (double j = -10; j < 10; j += 0.7)
    {
        ising_1d<1000> ising(1, 1 / j);
        double         x_500 = 0;
        monte_carlo(x_500, ising, f, g, n);
        std::cout << "Beta = " << 1 << "| h = " << 1 / j << '\n';
        std::cout << "x_500 = " << x_500 << '\n';
    }
}
