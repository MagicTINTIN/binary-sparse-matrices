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

std::vector<u_int8_t> BCSR::toDenseMatrix() const
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
    ret += std::to_string(_height) + ";" + std::to_string(_width);
    ret += "> (" + std::to_string(_index_pointers[_height]);
    ret += " ones / ";
    ret += std::to_string(_width*_height);
    ret += ", sparsity: ";
    ret += std::to_string((float)100*(_width*_height-_index_pointers[_height])/(_width*_height));

    ret += "%)\nIndex Pointers (Rows): [" + std::to_string(_index_pointers[0]);
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
    std::vector<u_int8_t> m = toDenseMatrix();
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
                // _nz_number++;
                // printf("adding {%d;%d}\n", row, col);
            }
        }
        // printf("row{%d}=%d\n", row + 1, _indices.size());
        _index_pointers[row + 1] = _indices.size();
    }
}