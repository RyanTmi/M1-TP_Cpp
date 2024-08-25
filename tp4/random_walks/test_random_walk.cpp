#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

#include "random_walk.hpp"

int main()
{
    const size_t t = 10000;

    std::mt19937         g(time(0));
    random_walk          s(0, 0.5);
    random_walk_with_min m(0, 0.5);

    std::ofstream s_file("rw.dat");
    std::ofstream m_file("rw_min.dat");
    for (size_t i = 0; i < t; ++i)
    {
        s_file << i << " " << s.value() << "\n";
        m_file << i << " " << m.value() << "\n";
        s.update(g);
        m.update(g);
    }
}
