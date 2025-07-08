#include "bcsr.hh"
#include "../utils/utils.hh"
#include "../lil/blil.hh"
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

void bcsr_canonicalize(const u_int32_t n_row,
                       u_int32_t Ap[],
                       u_int32_t Aj[])
{
    for (size_t r = 0; r < n_row; r++)
    {
        std::sort(Aj + Ap[r], Aj + Ap[r + 1]);
    }
}

void bcsr_canonicalize(const u_int32_t n_row,
                       std::vector<u_int32_t> &Ap,
                       std::vector<u_int32_t> &Aj)
{
    for (size_t r = 0; r < n_row; r++)
    {
        std::sort(Aj.begin() + Ap[r], Aj.begin() + Ap[r + 1]);
    }
}

bool BCSR::checkOrder(bool verbose) const
{
    if (_indices.size() != _index_pointers[_index_pointers.size() - 1])
    {
        if (_index_pointers[_index_pointers.size() - 1] > _indices.size())
        {
            if (verbose)
                fprintf(stderr, "Size of indices (=%ld) not equal (<) to last index pointer (=%d)!\n", _indices.size(), _index_pointers[_index_pointers.size() - 1]);

            return false;
        }
        for (size_t i = _index_pointers[_height]; i < _indices.size(); i++)
        {
            if (_indices[i] > 0)
            {
                if (verbose)
                    fprintf(stderr, "Size of indices (=%ld) not equal (>) to last index pointer (=%d)!\n", _indices.size(), _index_pointers[_index_pointers.size() - 1]);

                return false;
            }
        }

        if (verbose)
            fprintf(stderr, "(WARNING) Size of indices (=%ld) not equal to last index pointer (=%d)!\n", _indices.size(), _index_pointers[_index_pointers.size() - 1]);
        // return false;
    }

    for (int i = 0; i < _height; ++i)
    {
        u_int8_t row_min_col = 0;
        bool first = true;
        for (int idx = _index_pointers[i]; idx < _index_pointers[i + 1]; ++idx)
        {
            if (_indices[idx] >= _width)
            {
                if (verbose)
                    fprintf(stderr, "Width (%d) is not equal to column number (%d) in line %d!\n", _width, _indices[idx], i);
                return false;
            }
            if (row_min_col >= _indices[idx] && !first)
            {
                if (verbose)
                    fprintf(stderr, "Line %d (diff %d-%d) columns number are not strictly ordered: %d >= %d!\n", i, i + 1, i, row_min_col, _indices[idx]);
                return false;
            }
            row_min_col = _indices[idx];
            first = false;
        }
    }

    return true;
}

bool BCSR::operator==(const BCSR &b)
{
    return _height == b._height &&
           _width == b._width &&
           _index_pointers == b._index_pointers &&
           _indices == b._indices;
}

bool BCSR::checkOrder() const
{
    return checkOrder(false);
}

BCSR &BCSR::operator|=(const BCSR &b)
{
    operationOr(b);
    return *this;
}

BCSR &BCSR::operator+=(const BCSR &b)
{
    operationOr(b);
    return *this;
}

void BCSR::operationOr(const BCSR &b)
{
    // *this = *this | b;
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationOr a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        // exit(EXIT_FAILURE);
    }

    *this = BCSR(BLIL(*this) | BLIL(b));
}

BCSR BCSR::operator|(const BCSR &b) const
{
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationOr a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        // exit(EXIT_FAILURE);
    }
    return BCSR(BLIL(*this) | BLIL(b));
}

BCSR BCSR::operator+(const BCSR &b) const
{
    return operator|(b);
}

void BCSR::operationAnd(const BCSR &b)
{
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationAnd a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        // exit(EXIT_FAILURE);
    }

    *this = BCSR(BLIL(*this) | BLIL(b));
}

BCSR BCSR::operator&(const BCSR &b) const
{
    if (b._width != _width || b._height != _height)
    {
        throw std::range_error("Error: dimensions does not match in operationAnd a<" + std::to_string(_height) + ";" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";" + std::to_string(b._width) + ">\n");
        // fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        // exit(EXIT_FAILURE);
    }

    return BCSR(BLIL(*this) & BLIL(b));
}

BCSR &BCSR::operator&=(const BCSR &b)
{
    operationAnd(b);
    return *this;
}

BCSR BCSR::operationTimesMatrix(const BCSR &b) const
{
    if (b._height != _width)
    {
        throw std::range_error("Error: dimensions does not match in operationTimesMatrix a<_;" + std::to_string(_width) + "> != b<" + std::to_string(b._height) + ";_>\n");
        // fprintf(stderr, "Error: dimensions does not match in operationTimesMatrix a<_;%d> != b<%d;_>\n", _width, b._height);
        // exit(EXIT_FAILURE);
    }

    BCSR res(_height, b._width);

    std::vector<u_int32_t> next(b._width, -1);

    u_int32_t nnz = 0;

    // std::vector<u_int32_t> index_pointers(_height + 1);
    // std::vector<u_int32_t> indices(0);
    // res._indices.reserve(13000);

    for (u_int32_t i = 0; i < _height; i++)
    {
        res._index_pointers[i] = nnz;
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = _index_pointers[i];
        u_int32_t jj_end = _index_pointers[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            u_int32_t j = _indices[jj]; // column index j in A

            u_int32_t kk_start = b._index_pointers[j];
            u_int32_t kk_end = b._index_pointers[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                u_int32_t k = b._indices[kk];

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
            res._indices.emplace_back(head);
            // Cj[nnz] = head;
            nnz++;
            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
        }
        std::sort(res._indices.begin() + res._index_pointers[i], res._indices.begin() + nnz);

        // Cp[i + 1] = nnz;
    }
    res._index_pointers[_height] = nnz;
    // bcsr_canonicalize(_height, res._index_pointers, res._indices);
    return res;
}

BCSR BCSR::operator*(const BCSR &b) const
{
    return operationTimesMatrix(b);
}

BCSR &BCSR::operator*=(const BCSR &b)
{
    *this = operationTimesMatrix(b);
    return *this;
}

BCSR &BCSR::selfTranspose()
{
    return *this = transpose();
}

BCSR BCSR::transpose() const
{
    BCSR result(_width, _height, _index_pointers[_height]);
    const u_int32_t nnz = _index_pointers[_height];

    // count number of values in each column
    for (u_int32_t col_idx = 0; col_idx < _index_pointers[_height]; col_idx++)
    {
        result._index_pointers[_indices[col_idx]]++;
    }

    // calculate cumulative sums
    for (u_int32_t col = 0, cumsum = 0; col < _width; col++)
    {
        u_int32_t temp = result._index_pointers[col];
        result._index_pointers[col] = cumsum;
        cumsum += temp;
    }
    result._index_pointers[_width] = nnz;

    for (u_int32_t row = 0; row < _height; row++)
    {
        for (u_int32_t col_index = _index_pointers[row]; col_index < _index_pointers[row + 1]; col_index++)
        {
            u_int32_t col = _indices[col_index];
            u_int32_t dest = result._index_pointers[col];
            result._indices[dest] = row;
            result._index_pointers[col]++;
        }
    }

    for (u_int32_t col = 0, last = 0; col <= _width; col++)
    {
        u_int32_t temp = result._index_pointers[col];
        result._index_pointers[col] = last;
        last = temp;
    }

    return result;
}

void BCSR::set(const u_int32_t row, const u_int32_t col, const u_int8_t value)
{
    if (value)
        set(row, col);
    else
        reset(row, col);
}

void BCSR::set(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        throw std::out_of_range("Error: position does not match in set method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        // exit(EXIT_FAILURE);
    }

    if (!insertByValue(_indices, _index_pointers[row], _index_pointers[row + 1], col))
        return;
    // for each row after the insertion, propagate the information
    for (size_t r = row + 1; r <= _height; r++)
        _index_pointers[r]++;
}

void BCSR::reset(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        throw std::out_of_range("Error: position does not match in reset method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in reset method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        // exit(EXIT_FAILURE);
    }

    if (!removeByValue(_indices, _index_pointers[row], _index_pointers[row + 1], col))
        return;
    // for each row after the removal, propagate the information
    for (size_t r = row + 1; r <= _height; r++)
    {
        _index_pointers[r]--;
    }
}

bool BCSR::get(const u_int32_t row, const u_int32_t col) const
{
    if (col >= _width || row >= _height)
    {
        throw std::out_of_range("Error: position does not match in get method, accessing M<" + std::to_string(_height) + ";" + std::to_string(_width) + ">(" + std::to_string(row) + "," + std::to_string(col) + ")\n");
        // fprintf(stderr, "Error: position does not match in get method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        // exit(EXIT_FAILURE);
    }
    return isValueIn(_indices, _index_pointers[row], _index_pointers[row + 1], col);
}

void BCSR::addDimension()
{
    addRow();
    addColumn();
}

void BCSR::addColumn()
{
    _width++;
}

void BCSR::addRow()
{
    _index_pointers.emplace_back(_index_pointers[_height]);
    _height++;
}

std::vector<u_int32_t> BCSR::getRow(u_int32_t row) const
{
    return std::vector(_indices.begin() + _index_pointers[row],
                       _indices.begin() + _index_pointers[row + 1]);
}

BCSR::BCSR(u_int32_t height, u_int32_t width) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1, 0);
    _indices = std::vector<u_int32_t>();
    // TODO: change with an heuristic
    _indices.reserve(13000);
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int32_t index_pointers[], u_int32_t indices[]) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(index_pointers, index_pointers + height + 1);
    _indices = std::vector<u_int32_t>(indices, indices + index_pointers[height]);
    // printf("%ld %ld\n", _index_pointers.size(), _indices.size());
}

BCSR::BCSR(u_int32_t height, u_int32_t width, std::vector<u_int32_t> &index_pointers, std::vector<u_int32_t> &indices) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(index_pointers.begin(), index_pointers.begin() + height + 1);
    _indices = std::vector<u_int32_t>(indices.begin(), indices.begin() + index_pointers[height]);
    // printf("%ld %ld\n", _index_pointers.size(), _indices.size());
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
    insertDn2BCSR(values);
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int32_t nz_number) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1, 0);
    _indices = std::vector<u_int32_t>(nz_number, 0);
    // _index_pointers = std::vector<u_int32_t>(height + 1);
    // _indices = std::vector<u_int32_t>(nz_number);
}

BCSR::BCSR() : _height(0), _width(0)
{
    _index_pointers = std::vector<u_int32_t>(1, 0);
    _indices = std::vector<u_int32_t>(0);
}
// CSR::~CSR()
// {
// }
