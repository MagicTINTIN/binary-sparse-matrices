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
                    fprintf(stderr, "Line %d (diff %d-%d) columns number are not strictly ordered: %d >= %d!\n", i, i+1, i, row_min_col, _indices[idx]);
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
    for (size_t r = 1; r < _index_pointers.size(); r++)
    {
        // if the line contains a non zero value.
        if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
        {
            u_int8_t addedInRow = 0;
            u_int32_t b_index_pointer = b._index_pointers[r - 1];
            // for each column check if there is an insertion to do before it
            for (u_int32_t col_index = _index_pointers[r - 1]; col_index < _index_pointers[r] + carry; col_index++)
            {
                // add every non zero indice (column) before this column
                for (; b._indices[b_index_pointer] <= _indices[col_index] && b_index_pointer < b._index_pointers[r]; b_index_pointer++)
                {
                    // if the value was already 1, then there is no change for this column
                    if (b._indices[b_index_pointer] < _indices[col_index])
                    {
                        _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
                        addedInRow++;
                        carry++;
                        col_index++;
                    }
                }
            }
            // for every other non-zero in the line that has not been added
            for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
            {
                _indices.insert(_indices.begin() + _index_pointers[r] + addedInRow, b._indices[b_index_pointer]);
                carry++;
                addedInRow++;
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
    for (size_t r = row + 1; r < _index_pointers.size(); r++)
    {
        if (!inserted)
        {
            for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
            {
                if (col < _indices[i])
                {
                    _indices.insert(_indices.begin() + i, col);
                    inserted = true;
                }
                // if the value was already 1, then there is no change
                else if (col == _indices[i])
                    return;
            }
            if (!inserted)
            {
                _indices.insert(_indices.begin() + _index_pointers[r], col);
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
    for (size_t r = row + 1; r < _index_pointers.size(); r++)
    {
        if (!removed)
        {
            for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
            {
                if (col == _indices[i])
                {
                    _indices.erase(_indices.begin() + i);
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

BCSR::BCSR(u_int32_t height, u_int32_t width) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
}

BCSR::BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
    insertDn2BCSR(values);
}
// CSR::~CSR()
// {
// }