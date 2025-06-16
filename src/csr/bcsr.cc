#include "bcsr.hh"

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

    // FIXME: (opti?) _index_pointers.size()  -> _height + 1
    // for (size_t r = 1; r < _index_pointers.size(); r)
    // {
    //     for (size_t c = b._index_pointers[r - 1]; c < b._index_pointers[r]; c++)
    //     {
    //         set(r - 1, b._indices[c]);
    //     }
    // }

    // for each row after an insertion, propagate the information with the carry
    u_int32_t carry = 0;
    for (size_t r = 1; r <= _height; r++) //< _index_pointers.size()
    {
        // if the line contains a non zero value.
        if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
        {
            u_int32_t col_index = _index_pointers[r - 1];
            u_int32_t b_index_pointer = b._index_pointers[r - 1];
            // for each column check if there is an insertion to do before it
            for (; col_index < _index_pointers[r] + carry; col_index++)
            {
                // add every non zero indice (column) before this column
                for (; b._indices[b_index_pointer] <= _indices[col_index] && b_index_pointer < b._index_pointers[r]; b_index_pointer++)
                {
                    // if the value was already 1, then there is no change for this column
                    if (b._indices[b_index_pointer] < _indices[col_index])
                    {
                        _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
                        // _nz_number++;
                        carry++;
                        col_index++;
                    }
                }
            }
            // for every other non-zero in the line that has not been added
            for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
            {
                _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
                // _nz_number++;
                carry++;
            }
        }
        _index_pointers[r] += carry;
    }
}

BCSR BCSR::operator|(const BCSR &b) const
{

    if (b._width != _width || b._height != _height)
    {
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }

    BCSR result(*this);

    // FIXME: (opti?) _index_pointers.size()  -> _height + 1
    // for (size_t r = 1; r < _index_pointers.size(); r)
    // {
    //     for (size_t c = b._index_pointers[r - 1]; c < b._index_pointers[r]; c++)
    //     {
    //         result.set(r - 1, b._indices[c]);
    //     }
    // }

    /*
    u_int32_t carry = 0;
    for (size_t r = 1; r < result._index_pointers.size(); r++)
    {
        // if the line contains a non zero value.
        if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
        {
            u_int32_t col_index = result._index_pointers[r - 1];
            u_int32_t b_index_pointer = b._index_pointers[r - 1];
            // for each column check if there is an insertion to do before it
            for (; col_index < _index_pointers[r] + carry; col_index++)
            {
                // add every non zero indice (column) before this column
                for (; b._indices[b_index_pointer] <= result._indices[col_index] && b_index_pointer < b._index_pointers[r]; b_index_pointer++)
                {
                    // if the value was already 1, then there is no change for this column
                    if (b._indices[b_index_pointer] < result._indices[col_index])
                    {
                        result._indices.insert(result._indices.begin() + col_index, b._indices[b_index_pointer]);
                        // result._nz_number++;
                        carry++;
                        col_index++;
                    }
                }
            }
            // for every other non-zero in the line that has not been added
            for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
            {
                result._indices.insert(result._indices.begin() + col_index, b._indices[b_index_pointer]);
                // result._nz_number++;
                carry++;
            }
        }
        result._index_pointers[r] += carry;
    }
    */
    result.operationOr(b);
    return result;
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

    size_t nbLoop = 0;
    size_t nbOp = 0;

    BCSR result(_height, b._width), bT(b.transpose());
    u_int32_t non_zero_count = 0;
    for (size_t rowA = 1; rowA <= _height; rowA++)
    {
        nbLoop++;
        // if a line only contains zeros, then the output line will only contains 0 too (skip n useless check loops)
        nbOp+=3;
        if (_index_pointers[rowA] - _index_pointers[rowA - 1] > 0)
        {
            for (size_t rowB = 0; rowB < bT._height; rowB++)
            {
                nbLoop++;
                // will skip the row if B current row is empty
                u_int32_t colA_ptr(_index_pointers[rowA - 1]), colB_ptr(bT._index_pointers[rowB]);
                nbOp+=4;
                while (colA_ptr < _index_pointers[rowA] && colB_ptr < bT._index_pointers[rowB + 1])
                {
                    nbLoop++;
                    nbOp+=2;
                    if (_indices[colA_ptr] < bT._indices[colB_ptr])
                        colA_ptr++;
                    else if (_indices[colA_ptr] > bT._indices[colB_ptr])
                        colB_ptr++;
                    else
                    {
                        // result._indices.emplace_back(rowB);
                        result._indices[non_zero_count] = rowB;
                        non_zero_count++;
                        break; // if we have a 1 then we know it will be there
                    }
                }
            }
        }
        result._index_pointers[rowA] = non_zero_count;
    }

    return result;
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

// BCSR BCSR::transpose() const
// {
//     BCSR result(_width, _height, _index_pointers[_height]);

//     // count number of values in each column
//     for (u_int32_t col_idx = 0; col_idx < _index_pointers[_height]; col_idx++)
//     {
//         result._index_pointers[_indices[col_idx] + 1]++;
//     }

//     // calculate cumulative sums
//     for (u_int32_t c = 1; c <= result._height; c++)
//         result._index_pointers[c] += result._index_pointers[c - 1];

//     for (u_int32_t row = 0; row < _height; row++)
//     {
//         for (u_int32_t col_index = _index_pointers[row]; col_index < _index_pointers[row + 1]; col_index++)
//         {
//             // TODO: simplify
//             u_int32_t col = _indices[col_index];
//             u_int32_t dest = result._index_pointers[col];
//             result._indices[dest] = row;
//             result._index_pointers[col]++;
//         }
//     }

//     for (u_int32_t col_idx = result._height; col_idx > 0; col_idx--)
//     {
//         result._index_pointers[col_idx] = result._index_pointers[col_idx - 1];
//     }
//     result._index_pointers[0] = 0;

//     return result;
// }

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
            // TODO: simplify
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

    bool inserted = false;
    // for each row after the insertion, propagate the information
    for (size_t r = row + 1; r <= _height; r++) // < _index_pointers.size()
    {
        if (!inserted)
        {
            for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
            {
                if (col < _indices[i])
                {
                    _indices.insert(_indices.begin() + i, col);
                    // _nz_number++;
                    inserted = true;
                    // TODO://
                    break;
                }
                // if the value was already 1, then there is no change
                else if (col == _indices[i])
                    return;
            }
            if (!inserted)
            {
                _indices.insert(_indices.begin() + _index_pointers[r], col);
                // _nz_number++;
                inserted = true;
            }
        }
        _index_pointers[r] += 1;
    }
}

void BCSR::reset(const u_int32_t row, const u_int32_t col)
{
    if (col >= _width || row >= _height)
    {
        fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
        exit(EXIT_FAILURE);
    }

    bool removed = false;
    // for each row after the removal, propagate the information
    for (size_t r = row + 1; r <= _height; r++) //< _index_pointers.size()
    {
        if (!removed)
        {
            for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
            {
                if (col == _indices[i])
                {
                    _indices.erase(_indices.begin() + i);
                    // _nz_number--;
                    removed = true;
                    // the column has been deleted, there is nothing more to check, if we wanted to, think to add i--;
                    break;
                }
            }
            if (!removed)
            {
                // if nothing has been removed on the affected row, then there is nothing to update
                return;
            }
        }
        _index_pointers[r] -= 1;
    }
}

BCSR::BCSR(u_int32_t height, u_int32_t width) : _height(height), _width(width) //, _nz_number(0)
{
    _index_pointers = std::vector<u_int32_t>(height + 1, 0);
    _indices = std::vector<u_int32_t>(13000);
    // TODO: change with an heuristic
    // _indices.reserve(13000);
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int32_t index_pointers[], u_int32_t indices[]) : _height(height), _width(width)
{
    // _nz_number = 0;
    _index_pointers = std::vector<u_int32_t>(index_pointers, index_pointers + height + 1);
    _indices = std::vector<u_int32_t>(indices, indices + index_pointers[height]);
    printf("%ld %ld\n", _index_pointers.size(), _indices.size());
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]) : _height(height), _width(width)
{
    // _nz_number = 0;
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
    insertDn2BCSR(values);
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int32_t nz_number) : _height(height), _width(width) //, _nz_number(nz_number)
{
    _index_pointers = std::vector<u_int32_t>(height + 1, 0);
    _indices = std::vector<u_int32_t>(nz_number, 0);
    // _index_pointers = std::vector<u_int32_t>(height + 1);
    // _indices = std::vector<u_int32_t>(nz_number);
}
// CSR::~CSR()
// {
// }
