#ifndef MCMC_HPP
#define MCMC_HPP

#include <iostream>
#include <random>
#include <vector>

class markov_2_states
{
public:
    markov_2_states(int x0 = 1, double a0 = 0.5, double b0 = 0.5)
        : m_state(x0),
          m_ua(a0),
          m_ub(b0)
    {}

    template <typename RNG>
    int operator()(RNG& g)
    {
        if (m_state == 1)
        {
            m_state = m_ua(g) ? 2 : 1;
        }
        else
        {
            m_state = m_ub(g) ? 1 : 2;
        }
        return m_state;
    }

private:
    int                         m_state;  // 1 or 2
    std::bernoulli_distribution m_ua;
    std::bernoulli_distribution m_ub;
};

class stat_2_states
{
public:
    stat_2_states()
        : m_visit1(0),
          m_visit2(0),
          m_total(0)
    {}

    void operator+=(int s) { s == 1 ? ++m_visit1 : ++m_visit2; }

    void operator/=(double n) { m_total += n; }

    double Visit1() const { return static_cast<double>(m_visit1) / m_total; }

    double Visit2() const { return static_cast<double>(m_visit2) / m_total; }

    friend std::ostream& operator<<(std::ostream& out, const stat_2_states& s);

private:
    std::size_t m_visit1;
    std::size_t m_visit2;
    std::size_t m_total;
};

std::ostream& operator<<(std::ostream& out, const stat_2_states& s)
{
    return out << s.Visit1() << ' ' << s.Visit2();
}

template <std::size_t N>
class ising_1d
{
public:
    using state = std::vector<std::int8_t>;

public:
    ising_1d(double beta, double h)
        : m_u(0, N),
          m_state(N, 1),
          m_beta(beta),
          m_h(h)
    {}

    template <typename RNG>
    state& operator()(RNG& g)
    {
        std::size_t k = m_u(g);
        int8_t      xk = m_state[k];
        int8_t      xk1 = 0, xk2 = 0;

        if (k > 0)
        {
            xk1 = m_state[k - 1];
        }
        if (k < N - 1)
        {
            xk2 = m_state[k + 1];
        }

        double                      p = std::min(1.0, std::exp(-2 * m_beta * xk * (xk1 + xk2) - 2 * m_h * xk));
        std::bernoulli_distribution b(p);
        if (b(g))
        {
            m_state[k] *= -1;
        }

        return m_state;
    }

private:
    std::uniform_int_distribution<> m_u;
    state                           m_state;
    double                          m_beta;
    double                          m_h;
};

#endif /* MCMC_HPP */
