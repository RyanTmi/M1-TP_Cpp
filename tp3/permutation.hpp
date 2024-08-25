#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <iostream>
#include <list>
#include <map>
#include <random>
#include <vector>

class cycle;

class immutable_permutation
{
public:
    virtual ~immutable_permutation() = default;

public:
    virtual std::size_t      size() const = 0;
    virtual std::size_t      operator[](std::size_t i) const = 0;
    virtual std::size_t      order() const = 0;
    virtual std::list<cycle> cycles() const = 0;
};

class permutation : public immutable_permutation
{
public:
    template <typename RNG>
    permutation(std::size_t size, RNG& g);
    permutation(std::size_t size = 1);
    permutation(const std::vector<std::size_t>& images);

    std::size_t      size() const override;
    std::size_t      operator[](std::size_t i) const override;
    std::size_t      order() const override;
    std::list<cycle> cycles() const override;

    permutation            inverse() const;
    bool                   is_derangement() const;
    std::list<std::size_t> fixed_points() const;

public:
    friend permutation   operator*(const permutation& a, const permutation& b);
    friend std::ostream& operator<<(std::ostream& out, const permutation& p);
    friend std::istream& operator>>(std::istream& in, permutation& p);

private:
    std::size_t              m_size;
    std::vector<std::size_t> m_images;
};

template <typename RNG>
permutation::permutation(std::size_t size, RNG& g)
    : permutation(size)
{
    for (std::size_t i = 0; i < size - 1; ++i)
    {
        std::uniform_int_distribution<std::size_t> u(i, size - 1);
        std::swap(m_images[i], m_images[u(g)]);
    }
}

class sparse_permutation : public immutable_permutation
{
public:
    std::size_t      size() const override;
    std::size_t      operator[](std::size_t i) const override;
    std::size_t      order() const override;
    std::list<cycle> cycles() const override;

    friend sparse_permutation operator*(const sparse_permutation& a, const sparse_permutation& b);
    friend std::ostream&      operator<<(std::ostream& out, const sparse_permutation& p);
    friend std::istream&      operator>>(std::istream& in, sparse_permutation& p);

private:
    std::size_t                        m_size;
    std::map<std::size_t, std::size_t> m_non_trivial_images;
};

class cycle : public immutable_permutation
{
public:
    std::size_t      size() const override;
    std::size_t      operator[](std::size_t i) const override;
    std::size_t      order() const override;
    std::list<cycle> cycles() const override;

    cycle inverse() const;
    bool  operator<(const cycle& rhs) const;

    friend permutation;
    friend sparse_permutation;
    friend std::ostream& operator<<(std::ostream& out, const cycle& c);

private:
    void                                     add_last_point(std::size_t element);
    std::vector<std::size_t>::const_iterator find(std::size_t i) const;

private:
    std::vector<std::size_t> m_elements;
};

#endif /* PERMUTATION_HPP */
