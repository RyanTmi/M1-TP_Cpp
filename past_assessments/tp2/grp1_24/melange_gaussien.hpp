#ifndef MELANGE_GAUSSIEN_HPP
#define MELANGE_GAUSSIEN_HPP

#include <algorithm>
#include <cmath>
#include <ostream>
#include <random>
#include <vector>

#include <Eigen/Dense>

template <typename T>
using vec = Eigen::Vector<T, Eigen::Dynamic>;

template <typename T>
using mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;


template <typename T>
class melange_gaussien;

template <typename T>
std::ostream& operator<<(std::ostream& out, const melange_gaussien<T>& mg);

template <typename T>
class melange_gaussien
{
public:
    melange_gaussien() = default;

    melange_gaussien(const std::vector<T>& weights, T sigma)
        : m_weights(weights),
          m_nd(0, sigma),
          m_conditional_components(weights.size())
    {}

    std::size_t size() const { return m_weights.size(); }

    const std::vector<T>& weights() const { return m_weights; }

    std::vector<T> get_means() const;

    template <typename RNG>
    void update_components(RNG& g);

    template <typename RNG>
    std::vector<T> generate_observations(RNG& g, std::size_t n);

    friend std::ostream& operator<< <>(std::ostream& out, const melange_gaussien& mg);

private:
    std::vector<T>                           m_weights;
    std::normal_distribution<T>              m_nd;
    std::vector<std::normal_distribution<T>> m_conditional_components;
};

template <typename T>
std::vector<T> melange_gaussien<T>::get_means() const
{
    std::vector<T> means(size());
    std::transform(
        m_conditional_components.cbegin(),
        m_conditional_components.cend(),
        means.begin(),
        [](const std::normal_distribution<T>& nd) { return nd.mean(); });
    return means;
}

template <typename T>
template <typename RNG>
void melange_gaussien<T>::update_components(RNG& g)
{
    std::transform(
        m_conditional_components.begin(),
        m_conditional_components.end(),
        m_conditional_components.begin(),
        [&](const std::normal_distribution<T>&) { return std::normal_distribution<T>(m_nd(g), 1); });
}

template <typename T>
template <typename RNG>
std::vector<T> melange_gaussien<T>::generate_observations(RNG& g, std::size_t n)
{
    std::vector<T>                          observations(n);
    std::discrete_distribution<std::size_t> mode(m_weights.cbegin(), m_weights.cend());
    std::transform(observations.begin(), observations.end(), observations.begin(), [&](T) {
        return m_conditional_components[mode(g)](g);
    });
    return observations;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const melange_gaussien<T>& mg)
{
    for (std::size_t i = 0; i < mg.size(); ++i)
    {
        out << mg.m_weights[i] << ' ' << mg.m_conditional_components[i].mean() << '\n';
    }
    return out;
}

template <typename T>
void update_phi(mat<T>& phi, const vec<T>& x, const vec<T>& m, const vec<T>& s)
{
    phi = exp(((x * m.transpose()).transpose().colwise() - (((m.array() * m.array()).matrix() + s) / 2.0)).array())
              .matrix()
              .transpose();
    phi = (phi.array().rowwise() * phi.array().colwise().sum()).matrix();
}

template <typename T>
void update_m(vec<T>& m, const vec<T>& x, const mat<T>& phi, T sigma)
{
    m = ((phi.array().colwise() * x.array()).colwise().sum() / (1 / (sigma * sigma) + phi.array().colwise().sum()))
            .matrix();
}

template <typename T>
void update_s(vec<T>& s, const mat<T>& phi, T sigma)
{
    s = (1 / (1 / (sigma * sigma) + phi.array().colwise().sum())).matrix();
}

#endif /* MELANGE_GAUSSIEN_HPP */
