#ifndef RULES_HPP
#define RULES_HPP

#include <array>

template <typename I, std::size_t N>
void update_polya_n(std::size_t j, std::array<I, N>& v)
{
    ++v[j];
}

class friedman_2
{
public:
    friedman_2(int q, int r)
        : m_q(q),
          m_r(r)
    {}

    void operator()(std::size_t j, std::array<int, 2>& v)
    {
        v[j] += m_q;
        v[1 - j] += m_r;
    }

private:
    int m_q;
    int m_r;
};

#endif /* RULES_HPP */
