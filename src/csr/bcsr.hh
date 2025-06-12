#pragma once
#ifndef BCSR_HH
#define BCSR_HH

#include <sys/types.h>
#include <vector>
#include <string>

class BCSR
{
private:
    u_int32_t _width, _height;
    u_int32_t _nz_number;
    std::vector<u_int32_t> _index_pointers;
    std::vector<u_int32_t> _indices;

    /**
     * Insert a dense matrix in the current BCSR matrix
     * @param values[] a array-like dense matrix
     */
    void insertDn2BCSR(u_int8_t values[]);

public:
    /**
     * Initialise a 0-filled BCSR matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     */
    BCSR(u_int32_t height, u_int32_t width);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param values the dense array-like matrix values
     */
    BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param nz_number number of non zero values (optimisation to prevent too many resizes)
     */
    BCSR(u_int32_t height, u_int32_t width, u_int32_t nz_number);

    /**
     * @returns a dense matrix
     */
    std::vector<u_int8_t> toDenseVector() const;

    /**
     * Convert the BCSR matrix to a printable string
     */
    std::string toString() const;

    /**
     * Convert the BCSR matrix in dense form to a printable string
     */
    std::string toDnString() const;

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

    // or operation with matrices
    /**
     * @note equivalent to operation+= and operation|=
     */
    void operationOr(const BCSR &b);

    /**
     * @note equivalent to operationOr method and operation+=
     */
    BCSR &operator|=(const BCSR &b);

    /**
     * @note equivalent to operationOr method and operation|=
     */
    BCSR &operator+=(const BCSR &b);
    BCSR operator|(const BCSR &b) const;


    /**
     * Will transpose the matrix
     * @warning it modifies the current matrix!
     * @note equivalent to transform it as a CSC
     */
    BCSR &selfTranspose();

    /**
     * @note equivalent to transform it as a CSC
     * @returns the transposed matrix
     */
    BCSR transpose() const;

    /**
     * Set a value in the matrix
     * @param row of the value
     * @param col of the value
     * @param value (0 or 1) (different from 0)
     * @warning this can be a costly operation
     */
    void set(const u_int32_t row, const u_int32_t col, const u_int8_t value);

    /**
     * Set the value at (row,col) to 1 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation
     */
    void set(const u_int32_t row, const u_int32_t col);

    /**
     * Set the value at (row,col) to 0 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation
     */
    void reset(const u_int32_t row, const u_int32_t col);
    // ~BCSR();
};

std::ostream &operator<<(std::ostream &stream, BCSR &matrix);
std::ostream &operator<<(std::ostream &stream, const BCSR &matrix);

#endif // BCSR_HH