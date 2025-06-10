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


/**
 * @note equivalent to operationOr method and operation+=
 */
BCSR & BCSR::operator|=(const BCSR &b)
{
    operationOr(b);
    return *this;
}

/**
 * @note equivalent to operationOr method and operation|=
 */
BCSR & BCSR::operator+=(const BCSR &b)
{
    operationOr(b);
    return *this;
}


/**
 * @note equivalent to operation+= and operation|=
 */
void BCSR::operationOr(const BCSR &b) {
    if (b._width != _width || b._height != _height) {
        fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
        exit(EXIT_FAILURE);
    }
    
}

/**
 * @returns a dense matrix
 */
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

/**
 * Prints the BCSR matrix
 */
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

/**
 * Prints the BCSR matrix in dense form
 */
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

/**
 * Insert a dense matrix in the current BCSR matrix
 * @param values[] a array-like dense matrix
 */
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

/**
 * Initialise a 0-filled BCSR matrix
 * @param height the matrix height/rows
 * @param width the matrix width/columns
 */
BCSR::BCSR(u_int32_t height, u_int32_t width) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
}

/**
 * Initialise a BCSR matrix using a dense matrix
 * @param height the matrix height/rows
 * @param width the matrix width/columns
 * @param values the dense array-like matrix values
 */
BCSR::BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]) : _height(height), _width(width)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
    insertDn2BCSR(values);
}
// CSR::~CSR()
// {
// }