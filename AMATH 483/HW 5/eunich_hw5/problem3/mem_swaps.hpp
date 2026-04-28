#include <vector>
#include <stdexcept>
#include <algorithm> 

// Swap rows i and j in a matrix stored in column-major order
void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    if (i < 0 || i >= nRows || j < 0 || j >= nRows)
        throw std::out_of_range("swapRows: row index out of range");
    if (matrix.size() != std::size_t(nRows) * nCols)
        throw std::invalid_argument("matrix size does not match nRows*nCols");
    if (i == j) return;
    for (int col = 0; col < nCols; ++col) {
        std::swap(matrix[col * nRows + i], matrix[col * nRows + j]);
    }
}

// Swap columns i and j in a matrix stored in column-major order
void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    if (i < 0 || i >= nCols || j < 0 || j >= nCols)
        throw std::out_of_range("swapCols: column index out of range");
    if (matrix.size() != std::size_t(nRows) * nCols)
        throw std::invalid_argument("matrix size does not match nRows*nCols");
    if (i == j) return;
    for (int row = 0; row < nRows; ++row) {
        std::swap(matrix[i * nRows + row], matrix[j * nRows + row]);
    }
}
