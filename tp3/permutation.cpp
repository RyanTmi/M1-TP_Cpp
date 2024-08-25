#include "permutation.hpp"

#include <algorithm>
#include <iterator>
#include <set>

static std::int64_t pcd(std::int64_t a, std::int64_t b);
static std::int64_t lcm(std::int64_t a, std::int64_t b);

permutation::permutation(std::size_t size)
    : m_size(size),
      m_images(size)
{
    std::iota(m_images.begin(), m_images.end(), 0);
}

permutation::permutation(const std::vector<std::size_t>& images)
    : m_size(images.size()),
      m_images(images)
{}

permutation permutation::inverse() const
{
    std::vector<std::size_t> inverse_images(m_size);
    for (std::size_t i = 0; i < m_size; ++i)
    {
        inverse_images[m_images[i]] = i;
    }
    return permutation(inverse_images);
}

std::size_t permutation::size() const
{
    return m_size;
}

bool permutation::is_derangement() const
{
    return fixed_points().size() == 0;
}

std::size_t permutation::order() const
{
    std::list<cycle> cs = cycles();
    return std::accumulate(cs.begin(), cs.end(), 1L, [](std::size_t p, const cycle& c) { return lcm(p, c.order()); });
}

std::list<std::size_t> permutation::fixed_points() const
{
    std::list<std::size_t> fixed_points;
    std::size_t            i = 0;
    std::copy_if(m_images.begin(), m_images.end(), std::back_inserter(fixed_points), [&i](std::size_t x) {
        return x == i++;
    });
    return fixed_points;
}

std::list<cycle> permutation::cycles() const
{
    std::list<cycle>      cs;
    std::set<std::size_t> s;
    for (std::size_t i = 0; i < m_size; ++i)
    {
        s.emplace_hint(s.end(), i);
    }

    while (!s.empty())
    {
        std::size_t min = *s.begin();
        if (min != m_images[min])
        {
            cycle c;
            c.add_last_point(min);

            std::size_t current = m_images[min];
            while (current != min)
            {
                s.erase(current);
                c.add_last_point(current);
                current = m_images[current];
            }
            cs.push_back(c);
        }
        s.erase(s.begin());
    }

    return cs;
}

permutation operator*(const permutation& a, const permutation& b)
{
    auto minmax = std::minmax(a, b, [](const permutation& a, const permutation& b) { return a.size() < b.size(); });
    std::vector<std::size_t> images(minmax.second.m_size);

    for (std::size_t i = 0; i < minmax.first.m_size; ++i)
    {
        images[i] = b[a[i]];
    }
    for (std::size_t i = minmax.first.m_size; i < minmax.second.m_size; ++i)
    {
        images[i] = b[i];
    }

    return permutation(images);
}

std::size_t permutation::operator[](std::size_t i) const
{
    return m_images[i];
}

std::ostream& operator<<(std::ostream& out, const permutation& p)
{
    out << p.m_size << " : ";
    std::copy(p.m_images.begin(), p.m_images.end(), std::ostream_iterator<std::size_t>(out, " "));
    return out;
}

std::istream& operator>>(std::istream& in, permutation& p)
{
    in >> p.m_size;
    p.m_images.resize(p.m_size);

    in.ignore(3);  // Ignoring the space and the colon
    std::copy_n(std::istream_iterator<std::size_t>(in), p.m_size, p.m_images.begin());
    return in;
}

std::size_t cycle::size() const
{
    return *std::max_element(m_elements.begin(), m_elements.end());
}

std::size_t cycle::order() const
{
    return m_elements.size();
}

cycle cycle::inverse() const
{
    cycle inverse;
    inverse.add_last_point(m_elements.front());
    std::copy(m_elements.rbegin(), m_elements.rend() - 1, std::back_inserter(inverse.m_elements));
    return inverse;
}

std::list<cycle> cycle::cycles() const
{
    return {*this};
}

std::size_t cycle::operator[](std::size_t i) const
{
    auto it = find(i);
    if (it == m_elements.end())
    {
        return i;
    }
    else if (it == m_elements.end() - 1)
    {
        return m_elements.front();
    }
    else
    {
        return *(it + 1);
    }
}

void cycle::add_last_point(std::size_t element)
{
    m_elements.push_back(element);
}

std::vector<std::size_t>::const_iterator cycle::find(std::size_t i) const
{
    return std::find(m_elements.begin(), m_elements.end(), i);
}

bool cycle::operator<(const cycle& rhs) const
{
    return m_elements.size() != rhs.m_elements.size() ? m_elements.size() < rhs.m_elements.size()
                                                      : std::lexicographical_compare(
                                                            m_elements.begin(),
                                                            m_elements.end(),
                                                            rhs.m_elements.begin(),
                                                            rhs.m_elements.end());
}

std::ostream& operator<<(std::ostream& out, const cycle& c)
{
    out << "[ ";
    std::copy(c.m_elements.begin(), c.m_elements.end(), std::ostream_iterator<std::size_t>(out, " "));
    out << "]";
    return out;
}

std::size_t sparse_permutation::size() const
{
    return m_size;
}

std::size_t sparse_permutation::operator[](std::size_t i) const
{
    auto it = m_non_trivial_images.find(i);
    return it != m_non_trivial_images.end() ? it->second : i;
}

std::size_t sparse_permutation::order() const
{
    std::list<cycle> cs = cycles();
    return std::accumulate(cs.begin(), cs.end(), 1L, [](std::size_t p, const cycle& c) { return lcm(p, c.order()); });
}

std::list<cycle> sparse_permutation::cycles() const
{
    std::list<cycle>      cycles;
    std::set<std::size_t> s;
    for (auto kv : m_non_trivial_images)
    {
        s.emplace_hint(s.end(), kv.first);
    }

    while (!s.empty())
    {
        std::size_t min = *s.begin();
        if (m_non_trivial_images.find(min) == m_non_trivial_images.end())
        {
            continue;
        }
        std::size_t min_image = m_non_trivial_images.at(min);
        if (min != min_image)
        {
            cycle c;
            c.add_last_point(min);

            std::size_t current = min_image;
            while (current != min)
            {
                s.erase(current);
                c.add_last_point(current);
                current = m_non_trivial_images.at(current);
            }
            cycles.push_back(c);
        }
        s.erase(s.begin());
    }

    return cycles;
}

sparse_permutation operator*(const sparse_permutation& a, const sparse_permutation& b)
{
    auto minmax =
        std::minmax(a, b, [](const sparse_permutation& a, const sparse_permutation& b) { return a.size() < b.size(); });

    const auto&        min_images = minmax.first.m_non_trivial_images;
    const auto&        max_images = minmax.second.m_non_trivial_images;
    sparse_permutation c = minmax.second;
    for (auto kv : min_images)
    {
        bool contain_key = max_images.find(kv.second) == max_images.end();
        if (contain_key || max_images.at(kv.second) != kv.first)
        {
            std::size_t t = max_images.at(kv.second);
            c.m_non_trivial_images[kv.first] = contain_key ? kv.second : t;
        }
    }

    return c;
}

std::ostream& operator<<(std::ostream& out, const sparse_permutation& p)
{
    out << p.m_size << " : Sparse " << p.m_non_trivial_images.size() << "\n";
    for (const auto kv : p.m_non_trivial_images)
    {
        out << kv.first << ' ' << kv.second << '\n';
    }
    return out;
}

std::istream& operator>>(std::istream& in, sparse_permutation& p)
{
    std::string sparse;
    std::size_t rows;

    in >> p.m_size;
    in >> sparse;
    in >> rows;
    for (std::size_t i = 0; i < rows; ++i)
    {
        std::pair<std::size_t, std::size_t> key_value;
        in >> key_value.first >> key_value.second;
        p.m_non_trivial_images.insert(key_value);
    }
    return in;
}

std::int64_t pcd(std::int64_t a, std::int64_t b)
{
    std::pair<std::int64_t, std::int64_t> u(a, b);
    while (u.second != 0)
    {
        u = {u.second, u.first % u.second};
    }
    return u.first;
}

std::int64_t lcm(std::int64_t a, std::int64_t b)
{
    return a * b / pcd(a, b);
}
