#ifndef FILE_SWAPS_HPP
#define FILE_SWAPS_HPP

#include <fstream>
#include <vector>      
#include <stdexcept>


// Swap two rows i and j in a double matrix stored in column-major order in a binary file.

inline void swapRowsInFile(std::fstream &file,
                           int nRows, int nCols,
                           int i, int j)
{
    if (i == j) return;
    if (!file) throw std::runtime_error("Invalid file stream for row swap.");

    const std::streamoff elemSize = sizeof(double);
    double val_i, val_j;

    for (int col = 0; col < nCols; ++col) {
        std::streamoff pos_i = (static_cast<std::streamoff>(col) * nRows + i) * elemSize;
        std::streamoff pos_j = (static_cast<std::streamoff>(col) * nRows + j) * elemSize;

        file.seekg(pos_i);
        file.read(reinterpret_cast<char*>(&val_i), sizeof(double));
        file.seekg(pos_j);
        file.read(reinterpret_cast<char*>(&val_j), sizeof(double));

        file.seekp(pos_i);
        file.write(reinterpret_cast<char*>(&val_j), sizeof(double));
        file.seekp(pos_j);
        file.write(reinterpret_cast<char*>(&val_i), sizeof(double));
    }
}


// Swap two columns i and j in a double matrix stored in column-major order in a binary file.

inline void swapColsInFile(std::fstream &file,
                           int nRows, int nCols,
                           int i, int j)
{
    if (i == j) return;
    if (!file) throw std::runtime_error("Invalid file stream for column swap.");

    const std::streamoff colBytes = static_cast<std::streamoff>(nRows) * sizeof(double);
    std::vector<char> buf_i(colBytes), buf_j(colBytes);

    std::streamoff off_i = static_cast<std::streamoff>(i) * colBytes;
    std::streamoff off_j = static_cast<std::streamoff>(j) * colBytes;

    file.seekg(off_i);
    file.read(buf_i.data(), colBytes);
    file.seekg(off_j);
    file.read(buf_j.data(), colBytes);

    file.seekp(off_i);
    file.write(buf_j.data(), colBytes);
    file.seekp(off_j);
    file.write(buf_i.data(), colBytes);
}

#endif // FILE_SWAPS_HPP
