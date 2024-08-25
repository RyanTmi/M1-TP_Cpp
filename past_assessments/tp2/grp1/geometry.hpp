#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

template <typename K>
class segment
{
public:
    segment(K l, K r)
        : m_left(l),
          m_right(r)
    {}

    bool contains(K n) const { return m_left <= n && n <= m_right; }

private:
    K m_left;
    K m_right;
};

#endif /* GEOMETRY_HPP */
