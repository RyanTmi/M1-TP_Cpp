#ifndef PI_HPP
#define PI_HPP

#include <cmath>
#include <random>

class square_in_disk
{
public:
    square_in_disk()
        : m_u(0, 1)
    {}

    template <typename RNG>
    double operator()(RNG& g)
    {
        return (std::pow(m_u(g), 2) + std::pow(m_u(g), 2)) <= 1;
    }

private:
    std::uniform_real_distribution<> m_u;
};

#endif /* PI_HPP */
