#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

#include "blil.hh"

BLIL::BLIL(u_int32_t height, u_int32_t width) : _height(height), _width(width)
{
    _rows = std::vector<std::vector<u_int32_t>>(_height);
}

BLIL::BLIL(u_int32_t height, u_int32_t width, std::vector<std::vector<u_int32_t>> &rows) : _height(height), _width(width)
{
    _rows = rows;
}

BLIL::BLIL(u_int32_t height, u_int32_t width, u_int8_t values[]) : _height(height), _width(width)
{
    _rows = std::vector<std::vector<u_int32_t>>(_height);
    insertDn2BLIL(values);
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
            fprintf(stderr, "Height (%d) is not equal to number of rows (%ld)!\n", _height, _rows.size());
        return false;
    }

    size_t r = 0;
    for (std::vector<u_int32_t> col : _rows)
    {
        if (col.size() > 0 && _width <= col[0])
        {
            if (verbose)
                fprintf(stderr, "Width (%d) is not equal to column number (%d) in line %ld!\n", _width, col[0], r);
            return false;
        }
        for (size_t i = 1; i < col.size(); i++)
        {
            if (_width <= col[i])
            {
                if (verbose)
                    fprintf(stderr, "Width (%d) is not equal to column number (%d) in line %ld!\n", _width, col[i], r);
                return false;
            }
            if (col[i] <= col[i - 1])
            {
                if (verbose)
                    fprintf(stderr, "Line %ld columns number %ld and %ld are not strictly ordered: %d >= %d!\n", r, i - 1, i,  col[i - 1], col[i]);
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
