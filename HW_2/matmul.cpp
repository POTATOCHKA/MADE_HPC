#include "matmul.h"

using namespace std;

template <class T>
T **rand_square_matr(size_t N)
{
    T **res = new T *[N];
    for (int i = 0; i < N; ++i)
    {
        res[i] = new T[N];
        for (int j = 0; j < N; ++j)
            res[i][j] = rand() % 500;
    }
    return res;
}

template <class T>
T **mat_mul_ijk(T **A, T **B, size_t N)
{
    T **result = new T *[N];
    for (int i = 0; i < N; ++i)
    {
        result[i] = new T[N];
    }

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
        {
            for (size_t k = 0; k < N; k++)
                result[i][j] += A[i][k] * B[k][j];
        }

    return result;
}

template <class T>
T **mat_mul_jik(T **A, T **B, size_t N)
{
    T **result = new T *[N];
    for (int i = 0; i < N; ++i)
    {
        result[i] = new T[N];
    }

    for (size_t j = 0; j < N; j++)
        for (size_t i = 0; i < N; i++)
        {
            for (size_t k = 0; k < N; k++)
                result[i][j] += A[i][k] * B[k][j];
        }

    return result;
}

template <class T>
T **mat_mul_kij(T **A, T **B, size_t N)
{
    T **result = new T *[N];
    for (int i = 0; i < N; ++i)
    {
        result[i] = new T[N];
    }

    for (size_t k = 0; k < N; k++)
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
                result[i][j] += A[i][k] * B[k][j];
        }

    return result;
}

void show_matr(int **A, int n, int m)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            cout << A[i][j] << ' ';
        }
        cout << endl;
    }
}

float calc_time(size_t matr_shape, char func_name)
{
    int **a = rand_square_matr<int>(matr_shape);
    int **b = rand_square_matr<int>(matr_shape);

    auto begin = std::chrono::high_resolution_clock::now();
    if (func_name == 'i')
    {
        int **c = mat_mul_ijk<int>(a, b, matr_shape);
    }
    else if (func_name == 'j')
    {
        int **c = mat_mul_jik<int>(a, b, matr_shape);
    }
    else if (func_name == 'k')
    {
        int **c = mat_mul_kij<int>(a, b, matr_shape);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    return elapsed.count() * 1e-9;
}