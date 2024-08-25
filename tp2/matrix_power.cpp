#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using matrix_double = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
using sparse_matrix_double = Eigen::SparseMatrix<double>;

matrix_double slow_power(const matrix_double& m, std::size_t n);

template <typename Matrix>
Matrix fast_power(const Matrix& matrix, std::size_t pow);

template <typename Matrix>
void profile_power(const char* func_name, std::function<Matrix()> func, bool show_result = false);

int main()
{
    constexpr std::size_t pow = 10'000;

    matrix_double a {
        { 0.4f,  0.6f, 0.0f},
        {0.75f, 0.25f, 0.0f},
        { 0.0f,  0.0f, 1.0f}
    };
    matrix_double        b(30, 30);
    sparse_matrix_double sb(b.rows(), b.cols());

    std::ifstream matrix_datas("matrice.dat");
    for (int i = 0; i < 30; ++i)
    {
        for (int j = 0; j < 30; ++j)
        {
            matrix_datas >> b(i, j);
            if (b(i, j) != 0)
            {
                sb.coeffRef(i, j) = b(i, j);
            }
        }
    }
    matrix_datas.close();

    std::cout << "----------------------------\n"
              << "a :\n"
              << a << "\n"
              << "----------------------------\n"
              << "b :\n"
              << b << "\n"
              << "----------------------------\n\n";

    std::cout << "Power : " << pow << "\n\n";
    profile_power<matrix_double>("slow_power a", [&a] { return slow_power(a, pow); });
    profile_power<matrix_double>("slow_power b", [&b] { return slow_power(b, pow); });
    profile_power<matrix_double>("fast_power a", [&a] { return fast_power(a, pow); });
    profile_power<matrix_double>("fast_power b", [&b] { return fast_power(b, pow); });
    profile_power<sparse_matrix_double>("sparse_fast_power sb", [&sb] { return fast_power(sb, pow); });
}

matrix_double slow_power(const matrix_double& m, std::size_t n)
{
    switch (n)
    {
        case 0:  return matrix_double::Identity(m.rows(), m.cols());
        case 1:  return m;
        default: return m * slow_power(m, n - 1);
    }
}

template <typename Matrix>
Matrix fast_power(const Matrix& matrix, std::size_t pow)
{
    switch (pow)
    {
        case 0:
        {
            if constexpr (std::is_same<Matrix, matrix_double>::value)
            {
                return Matrix::Identity(matrix.rows(), matrix.cols());
            }
            else
            {
                Matrix id;
                id.setIdentity();
                return id;
            }
        }
        case 1:  return matrix;
        default: break;
    }
    Matrix half_pow = fast_power(matrix, pow / 2);
    half_pow = half_pow * half_pow;
    return (pow % 2 == 0) ? half_pow : matrix * half_pow;
}

template <typename Matrix>
void profile_power(const char* func_name, std::function<Matrix()> func, bool show_result)
{
    const auto                          start = std::chrono::system_clock::now();
    const Matrix                        m = func();
    const auto                          end = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;

    if (show_result)
    {
        std::cout << "============================\n"
                  << m << "\n----------------------------\n"
                  << func_name << ": " << diff.count() * 1000 << "ms\n"
                  << "============================\n\n";
    }
}
