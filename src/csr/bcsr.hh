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

    // or operation with matrices
    /**
     * @note equivalent to operation+= and operation|=
     */
    void operationOr(const BCSR &b);

    /**
     * @note equivalent to operationOr method and operation+=
     */
    BCSR &BCSR::operator|=(const BCSR &b);

    /**
     * @note equivalent to operationOr method and operation|=
     */
    BCSR &BCSR::operator+=(const BCSR &b);
    BCSR BCSR::operator|(const BCSR &b) const;
    // ~BCSR();
};

std::ostream &operator<<(std::ostream &stream, BCSR &matrix);
std::ostream &operator<<(std::ostream &stream, const BCSR &matrix);

#endif // BCSR_HH