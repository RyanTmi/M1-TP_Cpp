#ifndef CHI2_HPP
#define CHI2_HPP

#include <random>

template <typename Real, std::size_t D>
class chi2_distribution
{
public:
    chi2_distribution() = default;

    template <typename RNG>
    Real operator()(RNG& g)
    {
        Real result = 0;
        for (std::size_t i = 0; i < D; ++i)
        {
            Real x = m_nd(g);
            result += x * x;
        }
        return result;
    }

private:
    std::normal_distribution<Real> m_nd;
};

#endif /* CHI2_HPP */
