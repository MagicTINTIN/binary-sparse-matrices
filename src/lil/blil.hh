#pragma once
#ifndef BLIL_HH
#define BLIL_HH

#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

class BCSR;

std::string denseMatrixPrinter(std::vector<uint8_t> m, uint32_t height, uint32_t width);
std::string spreadsheetPrinter(std::vector<uint8_t> m, uint32_t height, uint32_t width, std::vector<std::string> linesDescription);

class BLIL {
private:
    uint32_t _width, _height;
    // uint32_t _nz_number; // equal last value of _index_pointers

    /**
     * Insert a dense matrix in the current BCSR matrix
     * @param values[] a array-like dense matrix
     */
    void insertDn2BLIL(uint8_t values[]);

    /**
     * @warning direct access to the structure ! Do not modify unless you really want to do it !
     */
    std::vector<std::vector<uint32_t>> _rows;

public:
    /**
     * Initialise a <0;0> matrix
     */
    BLIL();
    /**
     * Initialise a 0-filled BLIL matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     */
    BLIL(uint32_t height, uint32_t width);

    /**
     * Initialise a BLIL matrix using BLIL matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param rows a vector of vectors containing column indices per row
     */
    BLIL(uint32_t height, uint32_t width, std::vector<std::vector<uint32_t>> &rows);

    /**
     * Initialise a BLIL matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param values the dense array-like matrix values
     */
    BLIL(uint32_t height, uint32_t width, uint8_t values[]);

    /**
     * Initialise a BLIL matrix using a BCSR one
     * @param matrix in BCSR format
     */
    BLIL(BCSR matrix);

    // ######### Conversions to other representations #########
    /**
     * @returns a dense matrix
     */
    std::vector<uint8_t> toDenseMatrix() const;
    /**
     * Convert the BLIL matrix to a printable string
     */
    std::string toString() const;
    /**
     * Convert the BLIL matrix to a printable condensed string
     */
    std::string toCondensedString() const;
    /**
     * Convert the BLIL matrix to a printable condensed string
     */
    std::string toCondensedString(char const separator) const;
    // /**
    //  * Get information about the matrix
    //  * @param excludeMaxNZAbove to hide from stats some high values
    //  */
    // std::string info(uint32_t excludeMaxNZAbove) const;
    /**
     * Get information about the matrix
     * @param excludeFullLineStat to exclude full line from median statistics
     */
    std::string info(bool excludeFullLineStat) const;
    /**
     * Get information about the matrix
     */
    std::string info() const;
    /**
     * Convert the BLIL matrix in dense form to a printable string
     */
    std::string toDnString() const;
    /**
     * Convert the BLIL matrix in dense form to a printable string with indices
     */
    std::string toSpreadsheet() const;
    /**
     * Convert the BLIL matrix in dense form to a printable string with indices
     * @param linesDescription a vector of string with a string per line
     */
    std::string toSpreadsheet(std::vector<std::string> linesDescription) const;
    /**
     * Convert the BLIL matrix to a BCSR one
     */
    BCSR toBCSR() const;

    /**
     * check ordering of the indices
     * CHECK: remove ?
     * @note useless in prod
     */
    bool checkOrder() const;
    /**
     * check ordering of the indices
     * @param verbose to print what are the representation issues
     * CHECK: remove ?
     * @note useless in prod
     */
    bool checkOrder(bool verbose) const;

    /**
     * Check if 2 matrices are the same
     */
    bool operator==(const BLIL &b);

    // ######### OR operation with matrices #########
    /**
     * @note equivalent to operation+= and operation|=
     * @returns if the matrix has changed
     */
    bool operationOr(const BLIL &b);
    void operationOr2(const BLIL &b);
    /**
     * @note equivalent to operationOr method and operation+=
     * @warning different return behaviour than most of the |=, doesn't return a reference to this!
     * @returns if the matrix has changed
     */
    bool operator|=(const BLIL &b);
    /**
     * @note equivalent to operationOr method and operation|=
     * @returns a reference to the current matrix
     */
    BLIL &operator+=(const BLIL &b);
    BLIL operator|(const BLIL &b) const;
    BLIL operator+(const BLIL &b) const;

    // ######### AND operation with matrices #########
    /**
     * @note equivalent to hadamard multiplication
     */
    void operationAnd(const BLIL &b);
    /**
     * @note equivalent to hadamard multiplication
     */
    BLIL operator&(const BLIL &b) const;
    /**
     * @note equivalent to hadamard multiplication
     */
    BLIL &operator&=(const BLIL &b);

    // ######### MULTIPLY operation with matrice matrice #########
    /**
     * @note not fully implemented, only for square matrices
     */
    BLIL operationTimesMatrix(const BLIL &b) const;
    /**
     * @note not fully implemented, only for square matrices
     */
    BLIL operator*(const BLIL &b) const;
    /**
     * @note not fully implemented, only for square matrices
     */
    BLIL &operator*=(const BLIL &b);

    // ######### TRANSPOSITION operation #########
    /**
     * Will transpose the matrix
     * @warning it modifies the current matrix!
     */
    BLIL &selfTranspose();
    /**
     * @returns the transposed matrix
     */
    BLIL transpose() const;
    /**
     * transposition without LIL > CSR.transpose() > LIL conversions
     * @returns the transposed matrix
     * @warning SLOWER than transpose() method
     */
    BLIL transpose_withoutConversion() const;

    // ######### SET RESET GET VALUE #########
    /**
     * Set a value in the matrix
     * @param row of the value
     * @param col of the value
     * @param value (0 or 1) (different from 0)
     */
    void set(uint32_t const row, uint32_t const col, uint8_t const value);
    /**
     * Set the value at (row,col) to 1 in the matrix
     * @param row of the value
     * @param col of the value
     */
    void set(uint32_t const row, uint32_t const col);
    /**
     * Set the value at (row,col) to 0 in the matrix
     * @param row of the value
     * @param col of the value
     */
    void reset(uint32_t const row, uint32_t const col);
    /**
     * Get a value in the matrix
     * @param row of the value
     * @param col of the value
     * @returns if the value at row,col is not a zero
     */
    bool get(uint32_t const row, uint32_t const col) const;

    /**
     * Add an empty row and column to the end of the matrix
     */
    void addDimension();

    /**
     * Add an empty column to the end of the matrix
     */
    void addColumn();

    /**
     * Add an empty row to the end of the matrix
     */
    void addRow();

    /**
     * Add a row with a non zero value and column to the end of the matrix
     * @param nonzero_column the column of the row's non-zero value
     */
    void addDimensionNZC(uint32_t nonzero_column);

    /**
     * Add an empty row and column with a non zero value to the end of the matrix
     * @param nonzero_row the row at which we emplace a new non-zero value at the new column
     */
    void addDimensionNZR(uint32_t nonzero_row);

    /**
     * Add a row with a non zero value and column with a non zero value to the end of the matrix
     * @param nonzero_column the column of the row's non-zero value
     * @param nonzero_row the row at which we emplace a new non-zero value at the new column
     */
    void addDimension(uint32_t nonzero_column, uint32_t nonzero_row);

    // FIXME: better factorization
    struct stats {
        size_t height, width, min, med, max, nnz;
        float sparsity;
    };

    stats getStats();

    /**
     * Retrieve non zeros columns of a row
     * @param row
     * @warning no row check
     */
    std::vector<uint32_t> getRow(uint32_t row) const;

    /**
     * Retrieve non zeros columns of all rows
     */
    std::vector<std::vector<uint32_t>> const &getRows() const;

    // Not needed for the moment
    // ~BLIL();

    friend class BCSR;
};

std::ostream &operator<<(std::ostream &stream, BLIL &matrix);
std::ostream &operator<<(std::ostream &stream, const BLIL &matrix);

#endif // BLIL_HH