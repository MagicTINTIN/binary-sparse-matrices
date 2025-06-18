#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

#include "blil.hh"
#include "bcsr.hh"

void BLIL::insertDn2BLIL(u_int8_t values[])
{
}

BLIL::BLIL(u_int32_t height, u_int32_t width)
{
}

BLIL::BLIL(u_int32_t height, u_int32_t width, std::vector<std::vector<u_int32_t>> &rows)
{
}

BLIL::BLIL(u_int32_t height, u_int32_t width, u_int8_t values[])
{
}

BLIL::BLIL(BCSR matrix)
{
}

bool BLIL::checkOrder() const
{
    return checkOrder(false);
}

bool BLIL::checkOrder(bool verbose) const
{
    if (_height != _rows.size())
    {
        if (verbose)
            fprintf(stderr, "Height (%d) is not equal to number of rows (%d)!\n", _height, _rows.size());
        return false;
    }

    size_t r = 0;
    for (std::vector<u_int32_t> col : _rows)
    {
        if (col.size() > 0 && _width <= col[0])
        {
            if (verbose)
                fprintf(stderr, "Width (%d) is not equal to column number (%d) in line %d!\n", _width, col[0], r);
            return false;
        }
        for (size_t i = 1; i < col.size(); i++)
        {
            if (_width <= col[i])
            {
                if (verbose)
                    fprintf(stderr, "Width (%d) is not equal to column number (%d) in line %d!\n", _width, col[i], r);
                return false;
            }
            if (col[i] <= col[i - 1])
            {
                if (verbose)
                    fprintf(stderr, "Line %d columns number %d and %d are not strictly ordered: %d >= %d!\n", r, i - 1, i, i, col[i - 1], col[i]);
                return false;
            }
        }
        r++;
    }
    return true;
}

void BLIL::operationOr(const BLIL &b) {}

BLIL &BLIL::operator|=(const BLIL &b) {}

BLIL &BLIL::operator+=(const BLIL &b) {}

BLIL BLIL::operator|(const BLIL &b) const {}

BLIL BLIL::operator+(const BLIL &b) const {}

void BLIL::operationAnd(const BLIL &b) {}

BLIL BLIL::operator&(const BLIL &b) const {}

BLIL &BLIL::operator&=(const BLIL &b) {}

BLIL BLIL::operationTimesMatrix(const BLIL &b) const {}

BLIL BLIL::operator*(const BLIL &b) const {}

BLIL &BLIL::operator*=(const BLIL &b) {}

BLIL &BLIL::selfTranspose() {}

BLIL BLIL::transpose() const {}

void BLIL::set(const u_int32_t row, const u_int32_t col, const u_int8_t value) {}

void BLIL::set(const u_int32_t row, const u_int32_t col) {}

void BLIL::reset(const u_int32_t row, const u_int32_t col) {}
