#ifndef FINITE_ATOMIC_MEASURES_HPP
#define FINITE_ATOMIC_MEASURES_HPP

#include <map>
#include <numeric>
#include <ostream>

template <typename E>
class finite_atomic_measure;

template <typename E1, typename Function>
finite_atomic_measure<std::invoke_result_t<Function, E1>> image(const finite_atomic_measure<E1>& mu, const Function& f);

template <typename E>
std::ostream& operator<<(std::ostream& out, const finite_atomic_measure<E>& fam);

template <typename E>
std::istream& operator>>(std::istream& in, finite_atomic_measure<E>& fam);

template <typename E>
finite_atomic_measure<E> operator+(const finite_atomic_measure<E>& f1, const finite_atomic_measure<E>& f2);

template <typename E>
class finite_atomic_measure
{
public:
    finite_atomic_measure() = default;

    template <typename It>
    finite_atomic_measure(It first, It last);

    const std::map<E, double>& atomic_masses() const { return m_mass; }

    std::size_t nb_of_atoms() const { return m_mass.size(); }

    double total_mass() const;

    void add_mass(const E& point, double mass_value) { m_mass[point] += mass_value; }

    template <typename Domain>
    double measure(const Domain& d) const;

    template <typename RealFunction_on_E>
    double integral(const RealFunction_on_E& f) const;

    friend finite_atomic_measure operator+ <>(const finite_atomic_measure& f1, const finite_atomic_measure& f2);

    friend std::ostream& operator<< <>(std::ostream& out, const finite_atomic_measure& fam);
    friend std::istream& operator>> <>(std::istream& in, finite_atomic_measure& fam);

private:
    std::map<E, double> m_mass;  // Atomes et leurs masses associées
};

template <typename E>
template <typename It>
finite_atomic_measure<E>::finite_atomic_measure(It first, It last)
{
    for (; first != last; ++first)
    {
        m_mass[*first] = 1;
    }
}

template <typename E>
double finite_atomic_measure<E>::total_mass() const
{
    return std::accumulate(m_mass.cbegin(), m_mass.cend(), 0.0, [](double x, const std::pair<E, double>& p) {
        return x + p.second;
    });
}

template <typename E>
template <typename Domain>
double finite_atomic_measure<E>::measure(const Domain& d) const
{
    return std::accumulate(m_mass.cbegin(), m_mass.cend(), 0.0, [&d](double m, const std::pair<E, double>& p) {
        return m + (d.contains(p.first) ? p.second : 0.0);
    });
}

template <typename E>
template <typename RealFunction_on_E>
double finite_atomic_measure<E>::integral(const RealFunction_on_E& f) const
{
    return std::accumulate(m_mass.cbegin(), m_mass.cend(), 0.0, [&](double x, const std::pair<E, double>& p) {
        return x + p.second * f(p.first);
    });
}

template <typename E>
std::ostream& operator<<(std::ostream& out, const finite_atomic_measure<E>& fam)
{
    out << fam.nb_of_atoms() << '\n';
    for (const std::pair<E, double>& p : fam.m_mass)
    {
        out << p.first << ' ' << p.second << '\n';
    }
    return out;
}

template <typename E>
std::istream& operator>>(std::istream& in, finite_atomic_measure<E>& fam)
{
    fam.m_mass.clear();

    std::size_t size = 0;
    in >> size;

    E      point;
    double mass;
    for (std::size_t i = 0; i < size; ++i)
    {
        in >> point >> mass;
        fam.m_mass[point] += mass;
    }

    return in;
}

template <typename E1, typename Function>
finite_atomic_measure<std::invoke_result_t<Function, E1>> image(const finite_atomic_measure<E1>& mu, const Function& f)
{
    finite_atomic_measure<std::invoke_result_t<Function, E1>> im;
    for (const std::pair<E1, double>& p : mu.atomic_masses())
    {
        im.add_mass(f(p.first), p.second);
    }

    return im;
}

template <typename E>
finite_atomic_measure<E> operator+(const finite_atomic_measure<E>& f1, const finite_atomic_measure<E>& f2)
{
    finite_atomic_measure<E> m = f1;
    for (const std::pair<E, double>& p : f2.m_mass)
    {
        m.add_mass(p.first, p.second);
    }

    return m;
}

#endif /* FINITE_ATOMIC_MEASURES_HPP */
