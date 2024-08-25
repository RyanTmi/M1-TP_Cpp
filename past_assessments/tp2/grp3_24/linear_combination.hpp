#ifndef LINEAR_COMBINATION_HPP
#define LINEAR_COMBINATION_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <iostream>
#include <utility>
#include <vector>

template <typename T, std::size_t N>
std::array<T, N> operator+(const std::array<T, N>& a1, const std::array<T, N>& a2)
{
    std::array<T, N> r;
    std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), r.begin(), std::plus<T>());
    return r;
}

template <typename T, std::size_t N>
std::array<T, N> operator-(const std::array<T, N>& a1, const std::array<T, N>& a2)
{
    std::array<T, N> r;
    std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), r.begin(), std::minus<T>());
    return r;
}

template <typename T, std::size_t N>
std::array<T, N> operator*(const std::array<T, N>& a, T x)
{
    std::array<T, N> r;
    std::transform(a.cbegin(), a.cend(), r.begin(), [x](T i) { return x * i; });
    return r;
}

template <typename T>
class linear_combination;

template <typename T>
std::ostream& operator<<(std::ostream& out, const linear_combination<T>& l);

template <typename T>
linear_combination<T> operator*(T x, const linear_combination<T>& l);

template <typename T>
linear_combination<T> operator+(const linear_combination<T>& l1, const linear_combination<T>& l2);

template <typename T>
linear_combination<T> operator-(const linear_combination<T>& l1, const linear_combination<T>& l2);

template <typename T>
class linear_combination
{
public:
    linear_combination(std::size_t size = 0)
        : m_size(size),
          m_coeffs(size, 0)
    {}

    linear_combination(const std::vector<T>& coeffs)
        : m_size(coeffs.size()),
          m_coeffs(coeffs)
    {}

    std::size_t size() const { return m_size; }

    T operator[](std::size_t i) const { return m_coeffs[i]; }

    T& operator[](std::size_t i) { return m_coeffs[i]; }

    template <typename U>
    U operator()(const std::vector<U>& values) const;

    friend linear_combination operator* <>(T x, const linear_combination& l);
    friend linear_combination operator+ <>(const linear_combination& l1, const linear_combination& l2);
    friend linear_combination operator- <>(const linear_combination& l1, const linear_combination& l2);

private:
    std::size_t    m_size;
    std::vector<T> m_coeffs;
};

template <typename T>
template <typename U>
U linear_combination<T>::operator()(const std::vector<U>& values) const
{
    U result = values[0] * m_coeffs[0];
    for (std::size_t i = 1; i < values.size(); ++i)
    {
        result = result + values[i] * m_coeffs[i];
    }
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const linear_combination<T>& l)
{
    if (l.size() == 0)
    {
        return out;
    }

    out << l[0] << '*' << "x_0";
    for (std::size_t i = 1; i < l.size(); ++i)
    {
        out << " + " << l[i] << '*' << "x_" << i;
    }
    return out;
}

template <typename T>
linear_combination<T> operator*(T x, const linear_combination<T>& l)
{
    std::vector<T> coeffs = l.m_coeffs;
    std::transform(coeffs.begin(), coeffs.end(), coeffs.begin(), [x](T v) { return x * v; });
    return linear_combination<T>(coeffs);
}

template <typename T>
linear_combination<T> operator+(const linear_combination<T>& l1, const linear_combination<T>& l2)
{
    auto coeffs = l1.m_coeffs;
    std::transform(coeffs.begin(), coeffs.end(), l2.m_coeffs.begin(), coeffs.begin(), std::plus<T>());
    return linear_combination<T>(coeffs);
}

template <typename T>
linear_combination<T> operator-(const linear_combination<T>& l1, const linear_combination<T>& l2)
{
    auto coeffs = l1.m_coeffs;
    std::transform(coeffs.begin(), coeffs.end(), l2.m_coeffs.begin(), coeffs.begin(), std::minus<T>());
    return linear_combination<T>(coeffs);
}

std::pair<std::vector<double>, std::vector<double>> read_data(std::istream& in)
{
    std::vector<double> xs;
    std::vector<double> ys;

    double x = 0.0;
    double y = 0.0;
    while (in >> x >> y)
    {
        xs.push_back(x);
        ys.push_back(y);
    }

    return std::make_pair(xs, ys);
}

template <typename Fonction>
std::vector<double> app_fct(const Fonction& func, const std::vector<double>& values)
{
    std::vector<double> results = values;
    std::transform(values.begin(), values.end(), results.begin(), [&func](double x) { return func(x); });
    return results;
}

template <typename T, std::size_t N>
std::pair<bool, std::array<T, N>> solve_system(std::vector<linear_combination<T>> ls, std::array<T, N> w)
{
    for (std::size_t i = 0; i < ls.size(); ++i)
    {
        if (std::abs(ls[i][i]) <= std::numeric_limits<T>::epsilon())
        {
            return std::make_pair(false, std::array<T, N>());
        }
        double inv = 1.0 / ls[i][i];
        ls[i] = inv * ls[i];
        w[i] *= inv;
        for (std::size_t j = 0; j < ls.size(); ++j)
        {
            if (i != j)
            {
                T factor = ls[j][i];
                ls[j] = ls[j] - factor * ls[i];
                w[j] = w[j] - factor * w[i];
            }
        }
    }

    return std::make_pair(true, w);
}

#endif /* LINEAR_COMBINATION_HPP */
