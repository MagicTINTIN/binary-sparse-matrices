#include "blil.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

std::ostream &operator<<(
    std::ostream &stream,
    const BLIL &matrix)
{
    return std::operator<<(stream, matrix.toString());
}

std::ostream &operator<<(
    std::ostream &stream,
    BLIL &matrix)
{
    return std::operator<<(stream, matrix.toString());
}

std::vector<u_int8_t> BLIL::toDenseMatrix() const
{
    std::vector<u_int8_t> mat(_width * _height);

    for (int i = 0; i < _height; ++i)
    {
        // TODO:
    }

    return mat;
}

std::string BLIL::toString() const
{
    if (_width == 0 || _height == 0) 
        return "<0;0>";

        // TODO:
    std::ostringstream oss;
    // oss << "<" << _height << ";" << _width << "> ("
    //     << _index_pointers[_height] << " ones / "
    //     << (_width * _height)
    //     << ", sparsity: "
    //     << float(100.0 * (_width*_height - _index_pointers[_height]) 
    //              / (_width*_height))
    //     << "%)\nIndex Pointers (Rows): [";

    // for (size_t i = 0; i <= _height; ++i) {
    //     if (i) oss << "|";
    //     oss << _index_pointers[i];
    // }

    // oss << "]\nIndices (Columns): [";
    // for (size_t i = 0; i < _index_pointers[_height]; ++i) {
    //     if (i) oss << "|";
    //     oss << _indices[i];
    // }
    // oss << "]";

    return oss.str();
}

std::string BLIL::toCondensedString() const
{
    return toCondensedString('|');
}

std::string BLIL::toCondensedString(char const separator) const
{
    if (_width == 0 || _height == 0) 
        return "(1)[0]\n(1)[0]";

    std::ostringstream oss;
    oss << "("<< _height + 1<<")[";

    for (size_t i = 0; i <= _height; ++i) {
        if (i) oss << separator;
        oss << _index_pointers[i];
    }

    oss << "]\n(" << _index_pointers[_height] << ")[";
    for (size_t i = 0; i < _index_pointers[_height]; ++i) {
        if (i) oss << separator;
        oss << _indices[i];
    }
    oss << "]";

    return oss.str();
}

std::string BLIL::toDnString() const
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

