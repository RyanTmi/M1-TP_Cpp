#include <fstream>
#include <iostream>
#include <random>

#include <Eigen/Eigenvalues>

#include "histogram.hpp"

using matrix_double = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

Eigen::VectorXcd generate_random_spectrum(std::mt19937_64& g, std::size_t n);

void generate_goe_matrix(std::mt19937_64& g, matrix_double& m);

int main()
{
    std::random_device rd;
    std::mt19937_64    g(rd());

    constexpr std::size_t sample_count = 200;
    constexpr std::size_t matrices_size = 150;

    constexpr double      lower_bound = -3;
    constexpr double      upper_bound = 3;
    constexpr std::size_t box_count = 20;

    histogram hist(lower_bound, upper_bound, box_count);

    for (std::size_t i = 0; i < sample_count; ++i)
    {
        for (auto eigen_value : generate_random_spectrum(g, matrices_size))
        {
            hist += eigen_value.real() / (2 * std::sqrt(matrices_size));
        }
    }

    std::cout << hist << '\n';
    std::ofstream eigen_datas("eigenvalues.dat");
    eigen_datas << hist;
}

Eigen::VectorXcd generate_random_spectrum(std::mt19937_64& g, std::size_t n)
{
    matrix_double goe(n, n);
    generate_goe_matrix(g, goe);
    Eigen::EigenSolver<matrix_double> solver(goe);
    return solver.eigenvalues();
}

void generate_goe_matrix(std::mt19937_64& g, matrix_double& m)
{
    std::normal_distribution<> n1(0, 1);
    std::normal_distribution<> n2(0, 2);

    for (int i = 0; i < m.rows(); ++i)
    {
        m(i, i) = n1(g);
        for (int j = i + 1; j < m.cols(); ++j)
        {
            m(i, j) = n2(g);
            m(j, i) = m(i, j);
        }
    }
}
