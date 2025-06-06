#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>

std::ostream &operator<<(
    std::ostream &stream,
    const BCSR &matrix)
{
    // stream << matrix.toString();
    return std::operator<<(stream, matrix.toString());
}

std::ostream &operator<<(
    std::ostream &stream,
    BCSR &matrix)
{
    // stream << matrix.toString();
    return std::operator<<(stream, matrix.toString());
}

/**
 * @returns a dense matrix
 */
std::vector<std::vector<u_int8_t>> BCSR::toDenseVector() const
{
    std::vector<std::vector<u_int8_t>> mat;

    for (int i = 0; i < _height; ++i)
    {
        for (int idx = _index_pointers[i]; idx < _index_pointers[i + 1]; ++idx)
        {
            mat[i][_indices[idx]] = 1;
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
    std::vector<std::vector<u_int8_t>> m = toDenseVector();
    bool firstLine = true;
    for (std::vector<u_int8_t> r : m)
    {
        if (firstLine)
        {
            ret += "[";
            firstLine = false;
        }
        else
            ret += ",\n [";
        bool firstColumn = true;
        for (u_int8_t c : r)
        {
            if (firstColumn)
            {
                ret += std::to_string(c);
                firstColumn = false;
            }
            else
            {
                ret += "," + std::to_string(c);
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
                printf("adding {%d;%d}\n", row, col);
            }
        }
        printf("row{%d}=%d\n", row + 1, _indices.size());
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