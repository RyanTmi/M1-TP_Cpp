#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include <algorithm>
#include <iostream>
#include <vector>

template <typename Statistic, typename RandomVariable, typename Measurement, typename RNG>
void monte_carlo(Statistic& res, RandomVariable& x, const Measurement& f, RNG& g, std::uint64_t n)
{
    for (std::uint64_t i = 0; i < n; ++i)
    {
        res += f(x(g));
    }
    res /= static_cast<double>(n);
}

class double_mean_var
{
public:
    double_mean_var(double x = 0.0)
        : m_mean(x),
          m_var(0.0)
    {}

    double mean() const { return m_mean; }

    double var() const { return m_var - m_mean * m_mean; }

    double_mean_var& operator+=(double x)
    {
        m_mean += x;
        m_var += x * x;
        return *this;
    }

    double_mean_var& operator/=(double n)
    {
        m_mean /= n;
        m_var /= n;
        return *this;
    }

private:
    double m_mean;
    double m_var;
};

class histogram
{
public:
    histogram(double lowerBound, double upperBound, std::size_t n)
        : m_boxes(n, 0.0),
          m_box_count(n),
          m_lower_bound(lowerBound),
          m_upper_bound(upperBound),
          m_box_width((upperBound - lowerBound) / n)
    {}

    void operator+=(double x)
    {
        if (x >= m_lower_bound && x <= m_upper_bound)
        {
            m_boxes[std::floor((x - m_lower_bound) / m_box_width)]++;
        }
    }

    void operator/=(double n)
    {
        std::transform(m_boxes.begin(), m_boxes.end(), m_boxes.begin(), [n](double i) { return i / n; });
    }

    friend std::ostream& operator<<(std::ostream& out, const histogram& h);

private:
    std::vector<double> m_boxes;
    std::size_t         m_box_count;
    double              m_lower_bound;
    double              m_upper_bound;
    double              m_box_width;
};

std::ostream& operator<<(std::ostream& out, const histogram& h)
{
    for (std::size_t i = 0; i < h.m_box_count; ++i)
    {
        out << h.m_lower_bound + i * h.m_box_width << ' ' << h.m_boxes[i] << '\n';
    }

    return out;
}

#endif /* MONTE_CARLO_HPP */
