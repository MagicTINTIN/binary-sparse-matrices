#include "bcsr.hh"
#include "../utils/utils.hh"
#include "../lil/blil.hh"
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

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
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }
    
    *this = BCSR(BLIL(*this) | BLIL(b));
}

BCSR BCSR::operator|(const BCSR &b) const
{
    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }

    // for each row after an removal, propagate the information with the carry
    u_int32_t carry = 0;
    for (size_t r = 1; r <= _height; r++)
    {
        if (_index_pointers[r] - _index_pointers[r - 1] == 0)
        {
        }
        // if the line pf b contains a non zero value.
        else if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
        {
            u_int32_t col_index = _index_pointers[r - 1];
            u_int32_t b_index_pointer = b._index_pointers[r - 1];
            // for each column of b
            for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
            {
                // cehck if the current matrix has non-zero that has the same columns
                for (; _indices[col_index] <= b._indices[b_index_pointer] && col_index < _index_pointers[r] - carry; col_index++)
                {
                    // if the value of a previous column is a 1, we need to delete it
                    if (_indices[col_index] < b._indices[b_index_pointer])
                    {
                        _indices.erase(_indices.begin() + col_index);
                        col_index--;
                        carry++;
                    }
                    // else we just continue looking in the line for the other columns of b
                }
            }
            // for every other 1 -> they are now 0
            for (size_t idx_ptr = col_index; idx_ptr < _index_pointers[r] - carry; idx_ptr++)
            {
                _indices.erase(_indices.begin() + idx_ptr);
                carry++;
            }
        }
        // if we still have non-zeros left on this line, then remove all of them
        else // if (_index_pointers[r] - _index_pointers[r - 1] > 0) non necessary now
        {
            for (size_t idx_ptr = _index_pointers[r - 1]; idx_ptr < _index_pointers[r] - carry; idx_ptr++)
            {
                _indices.erase(_indices.begin() + idx_ptr);
                idx_ptr--;
                carry++;
            }
        }
        _index_pointers[r] -= carry;
    }
}

BCSR BCSR::operator&(const BCSR &b) const
{
    BCSR result(*this);
    result.operationAnd(b);
    return result;
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
        fprintf(stderr, "Error: dimensions does not match in operationTimesMatrix a<_;%d> != b<%d;_>\n", _width, b._height);
        exit(EXIT_FAILURE);
    }

    BCSR res(_height,b._width);

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

        // Cp[i + 1] = nnz;
    }
    res._index_pointers[_height] = nnz;
    scipy_canonicalize(_height, res._index_pointers, res._indices);
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
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    if (!removeByValue(_indices, _index_pointers[row], _index_pointers[row + 1], col))
        return;
    // for each row after the removal, propagate the information
    for (size_t r = row + 1; r <= _height; r++)
    {
        _index_pointers[r]--;
    }
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
// CSR::~CSR()
// {
// }
