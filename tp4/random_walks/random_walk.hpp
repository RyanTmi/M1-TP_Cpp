#ifndef RANDOM_WALK_HPP
#define RANDOM_WALK_HPP

#include <random>

class random_walk
{
public:
    random_walk(int start_value, double p)
        : m_current_time(0),
          m_current_value(start_value),
          m_start_value(start_value),
          m_distribution(p)
    {}

    int value() const { return m_current_value; }

    unsigned long time() const { return m_current_time; }

    virtual void reset()
    {
        m_current_time = 0;
        m_current_value = m_start_value;
    }

    template <typename RNG>
    void update(RNG& g)
    {
        m_current_time++;
        m_current_value += (2 * m_distribution(g) - 1);
    }

protected:
    unsigned long               m_current_time;
    int                         m_current_value;
    int                         m_start_value;
    std::bernoulli_distribution m_distribution;
};

class random_walk_with_min : public random_walk
{
public:
    random_walk_with_min(int start_value, double p)
        : random_walk(start_value, p),
          m_minimum(start_value)
    {}

    int minimum() const { return m_minimum; }

    void reset() override
    {
        m_current_time = 0;
        m_current_value = m_start_value;
        m_minimum = m_start_value;
    }

    template <typename RNG>
    void update(RNG& g)
    {
        int direction = 2 * m_distribution(g) - 1;
        m_current_value += direction;
        if (direction < 0)
        {
            m_minimum = m_current_value;
        }
        m_current_time++;
    }

protected:
    int m_minimum;
};

#endif /* RANDOM_WALK_HPP */
