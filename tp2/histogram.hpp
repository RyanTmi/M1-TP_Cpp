#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <iostream>
#include <vector>

class histogram
{
public:
    histogram(double lower_bound, double upper_bound, std::size_t box_count)
        : m_lower_bound(lower_bound),
          m_upper_bound(upper_bound),
          m_delta((upper_bound - lower_bound) / box_count),
          m_boxes(box_count, 0),
          m_out_count(0)
    {}

public:
    bool operator+=(double x);

    double lower_bound() const { return m_lower_bound; }

    double upper_bound() const { return m_upper_bound; }

    double box_count() const { return m_boxes.size(); }

    double out_count() const { return m_out_count; }

    void Reset();

    friend std::ostream& operator<<(std::ostream& out, const histogram& h);

private:
    double           m_lower_bound;
    double           m_upper_bound;
    double           m_delta;
    std::vector<int> m_boxes;
    std::size_t      m_out_count;
};

#endif /* HISTOGRAM_HPP */
