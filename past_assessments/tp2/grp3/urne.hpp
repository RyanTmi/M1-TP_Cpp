#ifndef URNES_HPP
#define URNES_HPP

#include <algorithm>
#include <array>
#include <ostream>
#include <random>
#include <numeric>

template <typename I, std::size_t N>
class urne;

template <typename I, std::size_t N>
std::ostream& operator<<(std::ostream& out, const urne<I, N>& u);

template <typename I, std::size_t N = 2>
class urne
{
public:
    urne();

    std::size_t nb_boules() const;

    void reset();

    double fraction(std::size_t i) const { return static_cast<double>(m_contenu[i]) / nb_boules(); }

    template <typename R, typename RNG>
    void maj(const R& rule, RNG& g);

    template <typename R, typename RNG>
    void maj_p_fois(const R& rule, RNG& g, std::size_t p);

    friend std::ostream& operator<< <>(std::ostream& out, const urne& u);

private:
    std::array<I, N> m_contenu_init;
    std::array<I, N> m_contenu;
};

template <typename I, std::size_t N>
urne<I, N>::urne()
{
    std::fill(m_contenu_init.begin(), m_contenu_init.end(), 1);
    std::fill(m_contenu.begin(), m_contenu.end(), 1);
}

template <typename I, std::size_t N>
std::size_t urne<I, N>::nb_boules() const
{
    return std::accumulate(m_contenu.cbegin(), m_contenu.cend(), 0ULL, std::plus<std::size_t>());
}

template <typename I, std::size_t N>
void urne<I, N>::reset()
{
    std::copy(m_contenu_init.cbegin(), m_contenu_init.cend(), m_contenu.begin());
}

template <typename I, std::size_t N>
template <typename R, typename RNG>
void urne<I, N>::maj(const R& rule, RNG& g)
{
    std::discrete_distribution<std::size_t> random_color(m_contenu.cbegin(), m_contenu.cend());
    rule(random_color(g), m_contenu);
}

template <typename I, std::size_t N>
template <typename R, typename RNG>
void urne<I, N>::maj_p_fois(const R& rule, RNG& g, std::size_t p)
{
    for (std::size_t i = 0; i < p; ++i)
    {
        maj(rule, g);
    }
}

template <typename I, std::size_t N>
std::ostream& operator<<(std::ostream& out, const urne<I, N>& u)
{
    for (const I& i : u.m_contenu)
    {
        out << i << ' ';
    }
    return out;
}

#endif /* URNES_HPP */
