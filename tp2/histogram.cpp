#include "histogram.hpp"

#include <cmath>

bool histogram::operator+=(double x)
{
    if (x < m_lower_bound || x > m_upper_bound)
    {
        m_out_count++;
        return false;
    }
    m_boxes[std::floor((x - m_lower_bound) / m_delta)]++;
    return true;
}

void histogram::Reset()
{
    m_out_count = 0;
    m_boxes.clear();
}

std::ostream& operator<<(std::ostream& out, const histogram& h)
{
    for (std::size_t k = 0; k < h.m_boxes.size(); ++k)
    {
        out << h.m_lower_bound + k * h.m_delta + h.m_delta / 2.0 << "\t" << h.m_boxes[k] << "\n";
    }
    return out;
}
