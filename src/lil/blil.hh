#pragma once
#ifndef BLIL_HH
#define BLIL_HH

#include <sys/types.h>
#include <vector>
#include <string>
// #include "../csr/bcsr.hh"
class BCSR;

class BLIL
{
private:
    u_int32_t _width, _height;
    // u_int32_t _nz_number; // equal last value of _index_pointers
    std::vector<std::vector<u_int32_t>> _rows;

    /**
     * Insert a dense matrix in the current BCSR matrix
     * @param values[] a array-like dense matrix
     */
    void insertDn2BLIL(u_int8_t values[]);
public:
    /**
     * Initialise a 0-filled BLIL matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     */
    BLIL(u_int32_t height, u_int32_t width);

    /**
     * Initialise a BLIL matrix using BLIL matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param rows a vector of vectors containing column indices per row
     */
    BLIL(u_int32_t height, u_int32_t width, std::vector<std::vector<u_int32_t>> &rows);

    /**
     * Initialise a BLIL matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param values the dense array-like matrix values
     */
    BLIL(u_int32_t height, u_int32_t width, u_int8_t values[]);

    /**
     * Initialise a BLIL matrix using a BCSR one
     * @param matrix in BCSR format
     */
    BLIL(BCSR matrix);

    // ######### Conversions to other representations #########
    /**
     * @returns a dense matrix
     */
    std::vector<u_int8_t> toDenseMatrix() const;
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
    /**
     * Convert the BLIL matrix in dense form to a printable string
     */
    std::string toDnString() const;
    /**
     * Convert the BLIL matrix to a BCSR one
     */
    BCSR toBCSR() const;

    /**
     * check ordering of the indices
     * TODO: remove ?
     * @note useless in prod
     */
    bool checkOrder() const;
    /**
     * check ordering of the indices
     * @param verbose to print what are the representation issues
     * TODO: remove ?
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
     */
    void operationOr(const BLIL &b);
    void operationOr2(const BLIL &b);
    /**
     * @note equivalent to operationOr method and operation+=
     */
    BLIL &operator|=(const BLIL &b);
    /**
     * @note equivalent to operationOr method and operation|=
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

    // ######### SET RESET VALUE #########
    /**
     * Set a value in the matrix
     * @param row of the value
     * @param col of the value
     * @param value (0 or 1) (different from 0)
     */
    void set(const u_int32_t row, const u_int32_t col, const u_int8_t value);
    /**
     * Set the value at (row,col) to 1 in the matrix
     * @param row of the value
     * @param col of the value
     */
    void set(const u_int32_t row, const u_int32_t col);
    /**
     * Set the value at (row,col) to 0 in the matrix
     * @param row of the value
     * @param col of the value
     */
    void reset(const u_int32_t row, const u_int32_t col);

    // Not needed for the moment
    // ~BLIL();

    friend class BCSR;
};

std::ostream &operator<<(std::ostream &stream, BLIL &matrix);
std::ostream &operator<<(std::ostream &stream, const BLIL &matrix);

#endif // BLIL_HH