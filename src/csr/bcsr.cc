#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

std::ostream &operator<<(
    std::ostream &stream,
    const BCSR &matrix)
{
    return std::operator<<(stream, matrix.toString());
}

std::ostream &operator<<(
    std::ostream &stream,
    BCSR &matrix)
{
    return std::operator<<(stream, matrix.toString());
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
    for (size_t r = 1; r < _index_pointers.size(); r)
    {
        for (size_t c = b._index_pointers[r - 1]; c < b._index_pointers[r]; c++)
        {
            set(r - 1, b._indices[c]);
        }

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
    for (size_t r = 1; r < _index_pointers.size(); r)
    {
        for (size_t c = b._index_pointers[r - 1]; c < b._index_pointers[r]; c++)
        {
            result.set(r - 1, b._indices[c]);
        }
    }

    return result;
}


void BCSR::set(const u_int32_t row, const u_int32_t col, const u_int8_t value) {
    if (value)
        set(row,col);
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

std::vector<u_int8_t> BCSR::toDenseVector() const
{
    std::vector<u_int8_t> mat(_width * _height);

    for (int i = 0; i < _height; ++i)
    {
        for (int idx = _index_pointers[i]; idx < _index_pointers[i + 1]; ++idx)
        {
            mat[i * _width + _indices[idx]] = 1;
        }
    }

    return mat;
}

std::string BCSR::toString() const
{
    if (_width == 0 || _height == 0)
        return std::string("<0;0>");

    std::string ret("<");
    ret += std::to_string(_height) + ";" + std::to_string(_width) + ">\nIndex Pointers (Rows): [" + std::to_string(_index_pointers[0]);
    for (u_int32_t i = 1; i < _height + 1; i++)
        ret += "|" + std::to_string(_index_pointers[i]);
    ret += "]\nIndices (Columns): [";
    if (_indices.size() > 0)
        ret += std::to_string(_indices[0]);
    for (u_int32_t i = 1; i < _indices.size(); i++)
        ret += "|" + std::to_string(_indices[i]);
    ret += "]";
    return ret;
}

std::string BCSR::toDnString() const
{
    if (_height == 0)
        return std::string("[]");
    if (_width == 0)
    {
        std::string ret("[[]");
        for (size_t i = 1; i < _height; i++)
        {
            ret += ",[]";
        }
        return ret + "]";
    }

    std::string ret("[");
    std::vector<u_int8_t> m = toDenseVector();
    for (size_t line = 0; line < _height; line++)
    {
        if (line == 0)
        {
            ret += "[";
        }
        else
            ret += ",\n [";
        for (size_t col = 0; col < _width; col++)
        {
            if (col == 0)
            {
                ret += std::to_string(m[line * _width + col]);
            }
            else
            {
                ret += "," + std::to_string(m[line * _width + col]);
            }
        }
        ret += "]";
    }
    ret += "]";
    return ret;
}

void BCSR::insertDn2BCSR(u_int8_t values[])
{
    _index_pointers[0] = 0;

    for (u_int32_t row = 0; row < _height; ++row)
    {
        for (u_int32_t col = 0; col < _width; ++col)
        {
            if (values[row * _width + col])
            {
                _indices.push_back(col);
                // printf("adding {%d;%d}\n", row, col);
            }
        }
        // printf("row{%d}=%d\n", row + 1, _indices.size());
        _index_pointers[row + 1] = _indices.size();
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