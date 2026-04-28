// eunich@uw.edu
// AMATH 483-583
// strassen.cpp : starter code for Strassen implementation

#include <iostream>
#include <vector>
#include <cmath>       
#include <functional>  


using namespace std;

template <typename T>
vector<vector<T>> addMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> subtractMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = pow(2, ceil(log2(n)));

    // Padding
    vector<vector<T>> A_pad(m, vector<T>(m, 0)), B_pad(m, vector<T>(m, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            A_pad[i][j] = A[i][j];
            B_pad[i][j] = B[i][j];
        }

    function<vector<vector<T>>(const vector<vector<T>> &, const vector<vector<T>> &)> recurse;

    recurse = [&](const vector<vector<T>> &A, const vector<vector<T>> &B) -> vector<vector<T>> {
        int size = A.size();
        if (size == 1)
            return {{A[0][0] * B[0][0]}};

        int k = size / 2;
        vector<vector<T>> A11(k, vector<T>(k)), A12(k, vector<T>(k)), A21(k, vector<T>(k)), A22(k, vector<T>(k));
        vector<vector<T>> B11(k, vector<T>(k)), B12(k, vector<T>(k)), B21(k, vector<T>(k)), B22(k, vector<T>(k));

        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                A11[i][j] = A[i][j];
                A12[i][j] = A[i][j + k];
                A21[i][j] = A[i + k][j];
                A22[i][j] = A[i + k][j + k];
                B11[i][j] = B[i][j];
                B12[i][j] = B[i][j + k];
                B21[i][j] = B[i + k][j];
                B22[i][j] = B[i + k][j + k];
            }
        }

        auto M1 = recurse(addMatrix(A11, A22), addMatrix(B11, B22));
        auto M2 = recurse(addMatrix(A21, A22), B11);
        auto M3 = recurse(A11, subtractMatrix(B12, B22));
        auto M4 = recurse(A22, subtractMatrix(B21, B11));
        auto M5 = recurse(addMatrix(A11, A12), B22);
        auto M6 = recurse(subtractMatrix(A21, A11), addMatrix(B11, B12));
        auto M7 = recurse(subtractMatrix(A12, A22), addMatrix(B21, B22));

        vector<vector<T>> C(size, vector<T>(size));
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
                C[i][j + k] = M3[i][j] + M5[i][j];
                C[i + k][j] = M2[i][j] + M4[i][j];
                C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
            }
        }
        return C;
    };

    auto C_pad = recurse(A_pad, B_pad);
    vector<vector<T>> C(n, vector<T>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = C_pad[i][j];

    return C;
}

template <typename T>
void printMatrix(const vector<vector<T>> &matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// int
template vector<vector<int>> addMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> subtractMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> strassenMultiply<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template void printMatrix<int>(const vector<vector<int>> &matrix);
// double
template vector<vector<double>> addMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> subtractMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> strassenMultiply<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template void printMatrix<double>(const vector<vector<double>> &matrix);
