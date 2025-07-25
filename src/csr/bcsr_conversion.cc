#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

#include "../lil/blil.hh"

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

BCSR::BCSR(BLIL matrix) : _width(matrix._width), _height(matrix._height)
{
    size_t nnz = 0;
    for (size_t i = 0; i < _height; i++)
    {
        nnz += matrix._rows[i].size();
    }
    _indices = std::vector<u_int32_t>(nnz);
    _index_pointers = std::vector<u_int32_t>(_height + 1, 0);
    nnz = 0;
    for (size_t r = 0; r < _height; r++)
    {
        _index_pointers[r] = nnz;
        for (size_t c = 0; c < matrix._rows[r].size(); c++)
        {
            _indices[nnz++] = matrix._rows[r][c];
        }
    }
    _index_pointers[_height] = nnz;
}

BLIL BCSR::toBLIL() const
{
    return BLIL(*this);
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
        return "<0;0>";

    std::vector<u_int32_t> nbPerLine(_height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (u_int32_t r = 0; r < _height; r++)
    {
        u_int32_t ones = _index_pointers[r + 1] - _index_pointers[r];
        nbPerLine[r] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[_height / 2];

    std::ostringstream oss;
    oss << "<" << _height << ";" << _width << "> ("
        << _index_pointers[_height] << " ones / "
        << (_width * _height)
        << ", sparsity: "
        << float(100.0 * (_width * _height - _index_pointers[_height]) / (_width * _height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\nIndex Pointers (Rows): [";

    for (size_t i = 0; i <= _height; ++i)
    {
        if (i)
            oss << "|";
        oss << _index_pointers[i];
    }

    oss << "]\nIndices (Columns): [";
    for (size_t i = 0; i < _index_pointers[_height]; ++i)
    {
        if (i)
            oss << "|";
        oss << _indices[i];
    }
    oss << "]";

    return oss.str();
}

std::string BCSR::toCondensedString() const
{
    return toCondensedString('|');
}

std::string BCSR::info() const
{
    std::ostringstream oss;
    if (_width == 0 || _height == 0)
        return "<0;0>";

    std::vector<u_int32_t> nbPerLine(_height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (u_int32_t r = 0; r < _height; r++)
    {
        u_int32_t ones = _index_pointers[r + 1] - _index_pointers[r];
        nbPerLine[r] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[_height / 2];

    oss << "<" << _height << ";" << _width << "> ("
        << _index_pointers[_height] << " ones / "
        << (_width * _height)
        << ", sparsity: "
        << float(100.0 * (_width * _height - _index_pointers[_height]) / (_width * _height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\nIndex Pointers (Rows) size: " << _height + 1 << "\n"
        << "Indices (Columns) size: " << _index_pointers[_height];
    return oss.str();
}

std::string BCSR::toCondensedString(char const separator) const
{
    if (_width == 0 || _height == 0)
        return "(1)[0]\n(0)[]";

    std::ostringstream oss;
    oss << "(" << _height + 1 << ")[";

    for (size_t i = 0; i <= _height; ++i)
    {
        if (i)
            oss << separator;
        oss << _index_pointers[i];
    }

    oss << "]\n(" << _index_pointers[_height] << ")[";
    for (size_t i = 0; i < _index_pointers[_height]; ++i)
    {
        if (i)
            oss << separator;
        oss << _indices[i];
    }
    oss << "]";

    return oss.str();
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

    std::vector<u_int8_t> m = toDenseMatrix();
    
    return denseMatrixPrinter(m,_height,_width);
}

std::string BCSR::toSpreadsheet() const
{
    std::vector<std::string> d(_height);
    return toSpreadsheet(d);
}

std::string BCSR::toSpreadsheet(std::vector<std::string> linesDescription) const
{
    std::vector<u_int8_t> m = toDenseMatrix();
    return spreadsheetPrinter(m, _height, _width, linesDescription);
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
                _indices.emplace_back(col);
                // _nz_number++;
                // printf("adding {%d;%d}\n", row, col);
            }
        }
        // printf("row{%d}=%d\n", row + 1, _indices.size());
        _index_pointers[row + 1] = _indices.size();
    }
}