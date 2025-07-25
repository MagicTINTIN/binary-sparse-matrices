#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "../utils/utils.hh"
#include "blil.hh"
#include "../csr/bcsr.hh"
#include <stdexcept>

BLIL::BLIL() : _height(0), _width(0)
{
    _rows = std::vector<std::vector<u_int32_t>>(0);
}

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

bool BLIL::operator==(const BLIL &b)
{
    return _height == b._height &&
           _width == b._width &&
           _rows == b._rows;
}

bool BLIL::operationOr(const BLIL &b)
{
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationOr a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }

    bool change = false;

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
                change = true;
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
            change = true;
        }
    }
    return change;
}

void BLIL::operationOr2(const BLIL &b)
{
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationOr a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
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

bool BLIL::operator|=(const BLIL &b)
{
    return operationOr(b);
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
        throw std::range_error("Error: dimensions does not match in operationAnd a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
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
{
    if (b._height != _width)
    {
        throw std::range_error("Error: dimensions does not match in operationTimesMatrix a<_;" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";_>\n");
        // fprintf(stderr, "Error: dimensions does not match in operationTimesMatrix a<_;%d> != b<%d;_>\n", _width, b._height);
        exit(EXIT_FAILURE);
    }

    BLIL res(_height, b._width);

    std::vector<u_int32_t> next(b._width, -1);

    for (u_int32_t i = 0; i < _height; i++)
    {
        u_int32_t head = -2;
        u_int32_t length = 0;

        for (u_int32_t j : _rows[i])
        {

            for (u_int32_t k : b._rows[j])
            {
                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            res._rows[i].emplace_back(head);

            // auto it = std::lower_bound(res._rows[i].begin(), res._rows[i].end(), head);
            // res._rows[i].insert(it, head);

            // insertByValue(res._rows[i], head);
            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
        }
        std::sort(res._rows[i].begin(), res._rows[i].end()); // emplacing back and then sorting is quicker than inserting !
    }
    return res;
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
        throw std::out_of_range("Error: position does not match in set method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    insertByValue(_rows[row], col);
}

void BLIL::reset(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        throw std::out_of_range("Error: position does not match in reset method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in reset method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    removeByValue(_rows[row], col);
}

bool BLIL::get(const u_int32_t row, const u_int32_t col) const
{
    if (col >= _width || row >= _height)
    {
        throw std::out_of_range("Error: position does not match in get method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in get method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }
    return isValueIn(_rows[row], col);
}

void BLIL::addDimension()
{
    addRow();
    addColumn();
}

void BLIL::addColumn()
{
    _width++;
}

void BLIL::addRow()
{
    _rows.emplace_back();
    _height++;
}

void BLIL::addDimensionNZC(u_int32_t nonzero_column)
{
    addDimension();
    _rows[_height - 1].emplace_back(nonzero_column);
}

void BLIL::addDimensionNZR(u_int32_t nonzero_row)
{
    addDimension();
    _rows[nonzero_row].emplace_back(_width - 1);
}

void BLIL::addDimension(u_int32_t nonzero_column, u_int32_t nonzero_row)
{
    addDimension();

    _rows[_height - 1].emplace_back(nonzero_column);
    if (nonzero_row < _height - 1)
        _rows[nonzero_row].emplace_back(_width - 1);
}

BLIL::stats BLIL::getStats()
{
    if (_width == 0 || _height == 0)
        return stats{
            height : 0,
            width : 0,
            min : 0,
            med : 0,
            max : 0,
            nnz: 0,
            sparsity : 0
        };

    size_t nnz = 0;
    std::vector<u_int32_t> nbPerLine(_height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (size_t i = 0; i < _height; i++)
    {
        u_int32_t ones = (u_int32_t)_rows[i].size();
        nnz += ones;
        nbPerLine[i] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[_height / 2];

    return stats{
        height : _height,
        width : _width,
        min : minNbPerLine,
        med : medNbPerLine,
        max : maxNbPerLine,
        nnz: nnz,
        sparsity : float(100.0 * (_width * _height - nnz) / (_width * _height))
    };
}

std::vector<u_int32_t> BLIL::getRow(u_int32_t row) const
{
    return _rows[row];
}

const std::vector<std::vector<u_int32_t>> &BLIL::getRows() const
{
    return _rows;
}
