#ifndef POLYNOME_HPP
#define POLYNOME_HPP

#include <algorithm>
#include <complex>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <type_traits>
#include <vector>

template <typename T>
bool is_zero(T x)
{
    return x == 0;
}

template <typename T>
bool is_one(T x)
{
    return x == 1;
}

template <>
bool is_zero<double>(double x)
{
    return std::abs(x) <= std::numeric_limits<double>::epsilon();
}

template <>
bool is_one<double>(double x)
{
    return std::abs(x - 1.0) <= std::numeric_limits<double>::epsilon();
}

template <typename T>
bool is_zero(std::complex<T> x)
{
    return is_zero(x.real()) && is_zero(x.imag());
}

template <typename T>
bool is_one(std::complex<T> x)
{
    return is_one(x.real()) && is_zero(x.imag());
}

template <typename T>
bool is_invertible(T x)
{
    if (std::is_integral<T>::value)
    {
        return x == 1 || x == -1;
    }
    return !is_zero(x);
}

template <typename U>
U identity(const U&)
{
    return U(1);
}

// std::complex type traits

template <typename T>
struct is_complex : public std::false_type
{};

template <typename T>
struct is_complex<std::complex<T>> : public std::true_type
{};


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////          BASIC POLYNOME           ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class basic_polynome;

template <typename T>
std::ostream& operator<<(std::ostream& out, const basic_polynome<T>& p);

template <typename T>
basic_polynome<T> operator+(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
basic_polynome<T> operator-(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
basic_polynome<T> operator*(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
basic_polynome<T> operator/(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
basic_polynome<T> operator%(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
std::pair<basic_polynome<T>, basic_polynome<T>>
    euclidean_division(const basic_polynome<T>& p1, const basic_polynome<T>& p2);

template <typename T>
class basic_polynome
{
public:
    basic_polynome(std::int64_t degree = -1)
        : m_degree(degree),
          m_coefficients(degree + 1, 0)
    {}

    basic_polynome(const T& a, std::int64_t n)
        : m_degree(n),
          m_coefficients(n + 1, 0)
    {
        m_coefficients[n] = a;
    }

    basic_polynome(const std::vector<T>& coefficients)
        : m_degree(coefficients.size() - 1),
          m_coefficients(coefficients)
    {}

    basic_polynome(std::vector<T>&& coefficients)
        : m_degree(coefficients.size() - 1),
          m_coefficients(std::move(coefficients))
    {}

public:
    std::int64_t degree() const { return m_degree; }

    T operator[](std::int64_t i) const { return (i >= 0 && i <= m_degree) ? m_coefficients[i] : 0; }

    template <typename U, typename I = decltype(identity<U>)>
    U operator()(const U& x, const I& e = identity<U>) const;

    friend std::ostream& operator<< <>(std::ostream& out, const basic_polynome& p);
    friend basic_polynome operator+ <>(const basic_polynome& p1, const basic_polynome& p2);
    friend basic_polynome operator- <>(const basic_polynome& p1, const basic_polynome& p2);
    friend basic_polynome operator* <>(const basic_polynome& p1, const basic_polynome& p2);
    friend basic_polynome operator/ <>(const basic_polynome& p1, const basic_polynome& p2);
    friend basic_polynome operator% <>(const basic_polynome& p1, const basic_polynome& p2);

    friend std::pair<basic_polynome, basic_polynome>
        euclidean_division<>(const basic_polynome& p1, const basic_polynome& p2);

private:
    void           print_monome(std::ostream& out, const T& a, std::size_t n) const;
    basic_polynome extend(std::int64_t m) const;
    void           adjust();

private:
    std::int64_t   m_degree;
    std::vector<T> m_coefficients;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const basic_polynome<T>& p)
{
    if (p.m_degree == -1)
    {
        return out << 0;
    }
    for (std::int64_t i = p.m_degree; i > 0; --i)
    {
        if (!is_zero(p[i]))
        {
            p.print_monome(out, p[i], i);
        }
        if (!is_zero(p[i - 1]))
        {
            out << " + ";
        }
    }
    return is_zero(p[0]) ? out : out << p[0];
}

template <typename T>
void basic_polynome<T>::print_monome(std::ostream& out, const T& a, std::size_t n) const
{
    char x = is_complex<T>::value ? 'z' : 'x';
    if (is_zero(a))
    {
        return;
    }
    out << a << '*';
    if (n == 1)
    {
        out << x;
    }
    else
    {
        out << x << '^' << n;
    }
}

template <typename T>
basic_polynome<T> basic_polynome<T>::extend(std::int64_t m) const
{
    std::vector<T> coefficients;
    coefficients.reserve(std::max(m_degree, m) + 1);
    std::copy(m_coefficients.cbegin(), m_coefficients.cend(), coefficients.begin());
    return basic_polynome<T>(std::move(coefficients));
}

template <typename T>
void basic_polynome<T>::adjust()
{
    auto it = std::find_if(m_coefficients.rbegin(), m_coefficients.rend(), [](const T& a) { return !is_zero(a); });
    auto d = std::distance(it, m_coefficients.rend());
    m_coefficients.resize(d);
    m_degree = d - 1;
}

template <typename T>
template <typename U, typename I>
U basic_polynome<T>::operator()(const U& x, const I& e) const
{
    U one = e(x);
    U zero = T(0) * one;
    return (m_degree >= 0) ? std::accumulate(
                                 m_coefficients.crbegin(),
                                 m_coefficients.crend(),
                                 zero,
                                 [&](const U& a, T b) { return a * x + b * one; })
                           : zero;
}

template <typename T>
basic_polynome<T> operator+(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    auto minmax = std::minmax(p1, p2, [](const basic_polynome<T>& p1, const basic_polynome<T>& p2) {
        return p1.m_degree < p2.m_degree;
    });

    basic_polynome<T> r = minmax.second;
    std::transform(
        minmax.first.m_coefficients.begin(),
        minmax.first.m_coefficients.end(),
        r.m_coefficients.begin(),
        r.m_coefficients.begin(),
        std::plus<T>());
    r.adjust();
    return r;
}

template <typename T>
basic_polynome<T> operator-(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    auto minmax = std::minmax(p1, p2, [](const basic_polynome<T>& p1, const basic_polynome<T>& p2) {
        return p1.m_degree < p2.m_degree;
    });
    bool sign = (p1.m_degree > p2.m_degree);

    basic_polynome<T> r = minmax.second;
    std::transform(
        minmax.first.m_coefficients.begin(),
        minmax.first.m_coefficients.end(),
        r.m_coefficients.begin(),
        r.m_coefficients.begin(),
        [sign](const T& a, const T& b) { return sign ? b - a : a - b; });
    if (!sign)
    {
        auto b = r.m_coefficients.begin() + minmax.first.m_degree + 1;
        std::transform(b, r.m_coefficients.end(), b, [](const T& a) { return -a; });
    }
    r.adjust();
    return r;
}

template <typename T>
basic_polynome<T> operator*(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    std::vector<T> coefficients(p1.m_degree + p2.m_degree + 1);
    for (std::int64_t i = 0; i <= p2.m_degree; ++i)
    {
        for (std::int64_t j = 0; j <= p1.m_degree; ++j)
        {
            coefficients[i + j] += p1[j] * p2[i];
        }
    }
    return basic_polynome<T>(std::move(coefficients));
}

template <typename T>
basic_polynome<T> operator/(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    return euclidean_division(p1, p2).first;
}

template <typename T>
basic_polynome<T> operator%(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    return euclidean_division(p1, p2).second;
}

template <typename T>
std::pair<basic_polynome<T>, basic_polynome<T>>
    euclidean_division(const basic_polynome<T>& p1, const basic_polynome<T>& p2)
{
    basic_polynome<T> r(p1);
    basic_polynome<T> q;

    while (r.m_degree >= p2.m_degree)
    {
        if (!is_invertible(p2[p2.m_degree]))
        {
            std::cerr << "Error: " << p2[p2.m_degree] << "is not invertible!\n";
            return std::make_pair(basic_polynome<T>(), p1);
        }
        std::int64_t      m = r.m_degree - p2.m_degree;
        basic_polynome<T> qm(r[r.m_degree] / p2[p2.m_degree], m);
        r = r - p2 * qm;
        q = q + qm;
    }
    return std::make_pair(q, r);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////         MONOME & POLYNOME         ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Implement monone

template <typename T>
class monome;

template <typename T>
std::ostream& operator<<(std::ostream& out, const monome<T>& m);

template <typename T>
bool operator<(const monome<T>& m1, const monome<T>& m2);

template <typename T>
class monome
{
public:
    monome() = default;

    monome(const T& coefficient, std::int64_t degree = 0)
        : m_coefficient(coefficient),
          m_degree(degree)
    {}

    friend std::ostream& operator<< <>(std::ostream& out, const monome& m);
    friend bool operator< <>(const monome& m1, const monome& m2);

private:
    T            m_coefficient;
    std::int64_t m_degree;
};

template <typename T>
bool operator<(const monome<T>& m1, const monome<T>& m2)
{
    return m1.m_degree < m2.m_degree;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const monome<T>& m)
{
    (void) m;
    return out;
}

template <typename T>
class polynome;

template <typename T>
std::ostream& operator<<(std::ostream& out, const polynome<T>& p);

template <typename T>
polynome<T> operator+(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
polynome<T> operator-(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
polynome<T> operator*(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
polynome<T> operator/(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
polynome<T> operator%(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
class polynome
{
public:
    std::int64_t degree() const { return m_monomes.empty() ? -1 : m_monomes.back().m_degree; }

    friend monome<T>;
    friend std::ostream& operator<< <>(std::ostream& out, const polynome& p);

    friend polynome operator+ <>(const polynome& p1, const polynome& p2);
    friend polynome operator- <>(const polynome& p1, const polynome& p2);
    friend polynome operator* <>(const polynome& p1, const polynome& p2);
    friend polynome operator/ <>(const polynome& p1, const polynome& p2);
    friend polynome operator% <>(const polynome& p1, const polynome& p2);

private:
    std::list<monome<T>> m_monomes;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const polynome<T>& p)
{
    for (const monome<T>& m : p.m_monomes)
    {
        out << m;
    }
    return out;
}

template <typename T>
polynome<T> operator+(const polynome<T>& p1, const polynome<T>& p2)
{
    (void) p1;
    (void) p2;
    return {};
}

template <typename T>
polynome<T> operator-(const polynome<T>& p1, const polynome<T>& p2)
{
    (void) p1;
    (void) p2;
    return {};
}

template <typename T>
polynome<T> operator*(const polynome<T>& p1, const polynome<T>& p2)
{
    (void) p1;
    (void) p2;
    return {};
}

template <typename T>
polynome<T> operator/(const polynome<T>& p1, const polynome<T>& p2)
{
    (void) p1;
    (void) p2;
    return {};
}

template <typename T>
polynome<T> operator%(const polynome<T>& p1, const polynome<T>& p2)
{
    (void) p1;
    (void) p2;
    return {};
}

#endif /* POLYNOME_HPP */
