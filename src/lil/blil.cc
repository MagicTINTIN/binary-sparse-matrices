#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "../utils/utils.hh"
#include "blil.hh"
#include "../csr/bcsr.hh"

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
{
    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }

    for (size_t r = 0; r < _height; r++)
    {
        size_t Acol_idx = 0;
        size_t Bcol_idx = 0;
        size_t A_col_nb = _rows[r].size();
        size_t B_col_nb = b._rows[r].size();
        while (Acol_idx < A_col_nb && Bcol_idx < B_col_nb)
        {
            if (b._rows[r][Bcol_idx] < _rows[r][Acol_idx])
            {
                _rows[r].insert(_rows[r].begin() + Acol_idx, b._rows[r][Bcol_idx]);
                Acol_idx++;
                A_col_nb++;
                Bcol_idx++;
            }
            else if (b._rows[r][Bcol_idx] > _rows[r][Acol_idx])
            {
                Acol_idx++;
            }
            else
            {
                Acol_idx++;
                Bcol_idx++;
            }
        }
        for (; Bcol_idx < B_col_nb; Bcol_idx++)
        {
            _rows[r].emplace_back(b._rows[r][Bcol_idx]);
        }
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
{
    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }

    for (size_t r = 0; r < _height; r++)
    {
        size_t Acol_idx = 0;
        size_t Bcol_idx = 0;
        size_t A_col_nb = _rows[r].size();
        size_t B_col_nb = b._rows[r].size();
        while (Acol_idx < A_col_nb && Bcol_idx < B_col_nb)
        {
            if (b._rows[r][Bcol_idx] < _rows[r][Acol_idx])
            {
                Bcol_idx++;
            }
            else if (b._rows[r][Bcol_idx] > _rows[r][Acol_idx])
            {
                _rows[r].erase(_rows[r].begin() + Acol_idx);
                A_col_nb--;
            }
            else
            {
                Acol_idx++;
                // Bcol_idx++;
            }
        }

        _rows[r].erase(_rows[r].begin() + Acol_idx, _rows[r].end());
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

BLIL BLIL::transpose_withoutConversion() const
{
    BLIL res(_width, _height);
    for (size_t r = 0; r < _height; r++)
        for (u_int32_t c : _rows[r])
            insertByValue<u_int32_t>(res._rows[c], r);
    return res;
}

BLIL BLIL::transpose() const
{
    return BLIL(BCSR(*this).transpose());
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

    insertByValue(_rows[row], col);
}

void BLIL::reset(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    removeByValue(_rows[row], col);
}
