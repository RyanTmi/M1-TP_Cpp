#ifndef LOI_MELANGE_HPP
#define LOI_MELANGE_HPP

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

template <typename RV>
class melange;

template <typename RV>
std::ostream& operator<<(std::ostream& out, const melange<RV>& m);

template <typename RV>
melange<RV> operator+(const melange<RV>& m1, const melange<RV>& m2);

template <typename RV>
class melange
{
public:
    melange() = default;
    melange(const std::vector<typename RV::param_type>& params, const std::vector<double> weights);

public:
    std::size_t nb_of_modes() const { return m_modes.size(); }

    std::vector<double> weights() const { return m_choose_mode.probabilities(); }

    const RV& operator[](std::size_t i) const { return m_modes[i]; }

    void add_mode(double alpha, const RV& to_add);

    void print_type() const { std::cout << typeid(RV()).name() << '\n'; }

    template <typename RNG>
    typename RV::result_type operator()(RNG& g)
    {
        return m_modes[m_choose_mode(g)](g);
    }

public:
    friend std::ostream& operator<< <>(std::ostream& out, const melange& m);
    friend melange operator+ <>(const melange& m1, const melange& m2);

private:
    std::vector<RV>                 m_modes;
    std::discrete_distribution<int> m_choose_mode;
};

template <typename RV>
melange<RV>::melange(const std::vector<typename RV::param_type>& params, const std::vector<double> weights)
    : m_modes(weights.size()),
      m_choose_mode(weights.cbegin(), weights.cend())
{
    std::transform(params.cbegin(), params.cend(), m_modes.begin(), [](const typename RV::param_type& p) {
        return RV(p);
    });
}

template <typename RV>
void melange<RV>::add_mode(double alpha, const RV& to_add)
{
    if (m_modes.empty())
    {
        m_choose_mode = std::discrete_distribution<int>({1});
    }
    else
    {
        auto p = m_choose_mode.probabilities();
        std::transform(p.begin(), p.end(), p.begin(), [alpha](double x) { return x * (1.0 - alpha); });
        p.push_back(alpha);
        // m_choose_mode = std::discrete_distribution<int>(p.cbegin(), p.cend());
        std::discrete_distribution<int> m_choose_mode(p.cbegin(), p.cend());
    }

    m_modes.push_back(to_add);
}

template <typename RV>
std::ostream& operator<<(std::ostream& out, const melange<RV>& m)
{
    auto w = m.weights();

    out << m.nb_of_modes() << '\n';
    for (std::size_t i = 0; i < m.nb_of_modes(); ++i)
    {
        out << w[i] << ' ' << m[i] << '\n';
    }
    return out;
}

template <typename RV>
melange<RV> operator+(const melange<RV>& m1, const melange<RV>& m2)
{
    auto w1 = m1.weights();
    auto w2 = m2.weights();

    std::vector<typename RV::param_type> p(w1.size() + w2.size());
    std::transform(m1.m_modes.cbegin(), m1.m_modes.cend(), p.begin(), [](const RV& rv) { return rv.param(); });
    std::transform(m2.m_modes.cbegin(), m2.m_modes.cend(), p.begin() + w1.size(), [](const RV& rv) {
        return rv.param();
    });

    std::vector<double> w(w1.size() + w2.size());
    std::transform(w1.cbegin(), w1.cend(), w.begin(), [](double x) { return x * 0.5; });
    std::transform(w2.cbegin(), w2.cend(), w.begin() + w1.size(), [](double x) { return x * 0.5; });

    return melange<RV>(p, w);
}

#endif /* LOI_MELANGE_HPP */
