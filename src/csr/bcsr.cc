#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

bool BCSR::checkOrder(bool verbose) const
{
    if (_indices.size() != _index_pointers[_index_pointers.size() - 1])
    {
        if (verbose)
            fprintf(stderr, "Size of indices (=%ld) not equal to last index pointer (=%d)!\n", _indices.size(), _index_pointers[_index_pointers.size() - 1]);
        return false;
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
        // if the line contains a non zero value.
        if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
        {
            u_int32_t col_index = _index_pointers[r - 1];
            u_int32_t b_index_pointer = b._index_pointers[r - 1];
            // for each column check if there is an insertion to do before it
            for (; b_index_pointer < b._index_pointers[r] + carry; col_index++)
            {
                // add every non zero indice (column) before this column
                for (; _indices[col_index] <= b._indices[b_index_pointer] && col_index < _index_pointers[r]; col_index++) //FIXME:
                {
                    // if the value was already 1, then there is no change for this column
                    if (b._indices[b_index_pointer] < _indices[col_index])
                    {
                        _indices.erase(_indices.begin() + col_index);
                        carry++;
                    }
                    else if (b._indices[b_index_pointer] == _indices[col_index])
                    {
                        col_index++;
                        break;
                    }
                    else
                    {
                        // our current column is higher than the b's current column
                        break;
                    }
                }
            }
            // for every other 1 -> they are now 0
            for (size_t idx_ptr = col_index; idx_ptr < _index_pointers[r] - carry; idx_ptr++)
            {
                _indices.erase(_indices.begin() + idx_ptr);
                carry++;
            }
            // for every other non-zero in the line that has not been added
            // for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
            // {
            //     _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
            //     carry++;
            // }
        }
        // if we have non-zeros on this line, then remove all of them
        else if (_index_pointers[r] - _index_pointers[r - 1] > 0)
        {
            for (size_t idx_ptr = _index_pointers[r - 1]; idx_ptr < _index_pointers[r] - carry; idx_ptr++)
            {
                _indices.erase(_indices.begin() + idx_ptr);
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

BCSR &BCSR::selfTranspose()
{
    return *this = this->transpose();
}

BCSR BCSR::transpose() const
{
    BCSR result(_width, _height, _index_pointers[_height]);

    // count number of values in each column
    // std::vector<u_int32_t> col_count(_width + 1, 0); //TODO: remove
    for (u_int8_t col : _indices)
    {
        // col_count[col+1]++;//TODO: remove
        result._index_pointers[col + 1]++;
    }

    // calculate cumulative sums
    for (u_int8_t c = 1; c <= result._height; c++)
        result._index_pointers[c] += result._index_pointers[c - 1];

    for (u_int32_t row = 0; row < _height; row++)
    {
        for (u_int32_t col_index = _index_pointers[row]; col_index < _index_pointers[row + 1]; col_index++)
        {
            // TODO: simplify
            u_int8_t col = _indices[col_index];
            u_int8_t dest = result._index_pointers[col];
            result._indices[dest] = row;
            result._index_pointers[col]++;
        }
    }
    for (u_int32_t col_idx = result._height; col_idx > 0; col_idx--)
    {
        result._index_pointers[col_idx] = result._index_pointers[col_idx - 1];
    }
    result._index_pointers[0] = 0;

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
    _indices = std::vector<u_int32_t>();
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
}
// CSR::~CSR()
// {
// }