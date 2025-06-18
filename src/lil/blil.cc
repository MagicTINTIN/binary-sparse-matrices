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
                    fprintf(stderr, "Line %ld columns number %ld and %ld are not strictly ordered: %d >= %d!\n", r, i - 1, i, col[i - 1], col[i]);
                return false;
            }
        }
        r++;
    }
    return true;
}

void BLIL::operationOr(const BLIL &b)
{ // TODO:
    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }
}

BLIL &BLIL::operator|=(const BLIL &b)
{
    operationOr(b);
    return *this;
}

BLIL &BLIL::operator+=(const BLIL &b)
{
    operationOr(b);
    return *this;
}

BLIL BLIL::operator|(const BLIL &b) const
{
    BLIL result(*this);
    result.operationOr(b);
    return result;
}

BLIL BLIL::operator+(const BLIL &b) const
{
    return operator|(b);
}

void BLIL::operationAnd(const BLIL &b)
{ // TODO:
    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }
}

BLIL BLIL::operator&(const BLIL &b) const
{
    BLIL result(*this);
    result.operationAnd(b);
    return result;
}

BLIL &BLIL::operator&=(const BLIL &b)
{
    operationAnd(b);
    return *this;
}

BLIL BLIL::operationTimesMatrix(const BLIL &b) const
{ // TODO:
    if (b._height != _width)
    {
        fprintf(stderr, "Error: dimensions does not match in operationTimesMatrix a<_;%d> != b<%d;_>\n", _width, b._height);
        exit(EXIT_FAILURE);
    }
}

BLIL BLIL::operator*(const BLIL &b) const
{
    return operationTimesMatrix(b);
}

BLIL &BLIL::operator*=(const BLIL &b)
{
    *this = operationTimesMatrix(b);
    return *this;
}

BLIL &BLIL::selfTranspose()
{
    return *this = transpose();
}

BLIL BLIL::transpose() const
{ // TODO:
}

void BLIL::set(const u_int32_t row, const u_int32_t col, const u_int8_t value)
{
    if (value)
        set(row, col);
    else
        reset(row, col);
}

void BLIL::set(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    for (u_int32_t i = 0; i < _rows[row].size(); i++)
    {
        if (col < _rows[row][i])
        {
            _rows[row].insert(_rows[row].begin() + i, col);
            return;
        }
        // if the value was already 1, then there is no change
        else if (col == _rows[row][i])
            return;
    }

    _rows[row].emplace_back(col);
}

void BLIL::reset(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    for (u_int32_t i = 0; i < _rows[row].size(); i++)
    {
        if (col == _rows[row][i])
        {
            _rows[row].erase(_rows[row].begin() + i);
            return;
        }
    }
}
