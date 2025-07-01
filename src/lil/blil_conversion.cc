#include "blil.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "../csr/bcsr.hh"
#include <algorithm>

#define NORMAL "\e[0m"
#define BOLD "\e[1m"
#define BLACK_NORMAL_COLOR "\e[30m"
#define WHITE_NORMAL_BACKGROUND "\e[47m"
#define WHITE_DESAT_BACKGROUND "\e[107m"
#define BLACK_DESAT_BACKGROUND "\e[100m"

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

BLIL::BLIL(BCSR matrix) : _width(matrix._width), _height(matrix._height)
{
    _rows = std::vector<std::vector<u_int32_t>>(_height);
    for (size_t r = 0; r < _height; r++)
    {
        u_int32_t r_start = matrix._index_pointers[r];
        u_int32_t r_stop = matrix._index_pointers[r + 1];
        std::vector<u_int32_t> line(r_stop - r_start);
        for (u_int32_t c = r_start, cc = 0; c < r_stop; c++, cc++)
        {
            line[cc] = matrix._indices[c];
        }
        _rows[r] = line;
    }
}

BCSR BLIL::toBCSR() const
{
    return BCSR(*this);
}

void BLIL::insertDn2BLIL(u_int8_t values[])
{
    for (u_int32_t row = 0; row < _height; ++row)
    {
        for (u_int32_t col = 0; col < _width; ++col)
        {
            if (values[row * _width + col])
            {
                _rows[row].emplace_back(col);
            }
        }
    }
}

std::vector<u_int8_t> BLIL::toDenseMatrix() const
{
    std::vector<u_int8_t> mat(_width * _height);

    for (int r = 0; r < _height; ++r)
    {
        for (u_int32_t c : _rows[r])
            mat[r * _width + c] = 1;
    }

    return mat;
}

std::string BLIL::toString() const
{
    if (_width == 0 || _height == 0)
        return "<0;0>";

    size_t nnz = 0;
    std::vector<u_int32_t> nbPerLine(_height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (size_t i = 0; i < _height; i++)
    {
        u_int32_t ones = (u_int32_t)_rows[i].size();
        nnz += ones;
        nbPerLine[i] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[_height / 2];

    std::ostringstream oss;
    oss << "<" << _height << ";" << _width << "> ("
        << nnz << " ones / "
        << (_width * _height)
        << ", sparsity: "
        << float(100.0 * (_width * _height - nnz) / (_width * _height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\n";

    for (size_t r = 0; r < _height; r++)
    {
        oss << r << ": [";
        for (size_t c = 0; c < _rows[r].size(); c++)
        {
            if (c)
                oss << "|";
            oss << _rows[r][c];
        }
        oss << "]\n";
    }

    return oss.str();
}

std::string BLIL::info() const
{
    if (_width == 0 || _height == 0)
        return "<0;0>";

    size_t nnz = 0;
    std::vector<u_int32_t> nbPerLine(_height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (size_t i = 0; i < _height; i++)
    {
        u_int32_t ones = (u_int32_t)_rows[i].size();
        nnz += ones;
        nbPerLine[i] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[_height / 2];

    std::ostringstream oss;
    oss << "<" << _height << ";" << _width << "> ("
        << nnz << " ones / "
        << (_width * _height)
        << ", sparsity: "
        << float(100.0 * (_width * _height - nnz) / (_width * _height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\n";
    return oss.str();
}

std::string BLIL::toCondensedString() const
{
    return toCondensedString(',');
}

std::string BLIL::toCondensedString(char const separator) const
{
    if (_height == 0)
        return "nothing. []";

    std::ostringstream oss;
    oss << "{";
    for (size_t r = 0; r < _height; r++)
    {
        if (r)
            oss << ",\n {";
        else
            oss << "{";
        for (size_t c = 0; c < _rows[r].size(); c++)
        {
            if (c)
                oss << separator;
            oss << _rows[r][c];
        }
        oss << "}";
    }
    oss << "}";

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

    std::vector<u_int8_t> m = toDenseMatrix();

    return denseMatrixPrinter(m, _height, _width);
}

std::string centerString(u_int8_t width, const std::string &str)
{
    u_int8_t len = str.length();
    if (width < len)
    {
        return str;
    }

    u_int8_t diff = width - len;
    u_int8_t pad1 = diff / 2;
    u_int8_t pad2 = diff - pad1;
    return std::string(pad1, ' ') + str + std::string(pad2, ' ');
}

void addNChars(std::ostringstream &oss, const std::string &str, const u_int32_t times)
{
    for (size_t i = 0; i < times; i++)
    {
        oss << str;
    }
}

std::string nTimesString(const std::string &str, const u_int32_t times)
{
    std::ostringstream oss;
    for (size_t i = 0; i < times; i++)
    {
        oss << str;
    }
    return oss.str();
}

void addNumber(std::ostringstream &oss, const u_int32_t &number, const u_int8_t size)
{
    oss << centerString(size, std::to_string(number));
}

void addRightNumber(std::ostringstream &oss, const u_int32_t &number, const u_int8_t size)
{
    oss << std::setw(size) << number;
}

std::string BLIL::toSpreadsheet() const
{
    std::vector<std::string> d(_height);
    return toSpreadsheet(d);
}

std::string BLIL::toSpreadsheet(std::vector<std::string> linesDescription) const
{
    std::vector<u_int8_t> m = toDenseMatrix();
    return spreadsheetPrinter(m, _height, _width, linesDescription);
}

std::string denseMatrixPrinter(std::vector<u_int8_t> m, u_int32_t height, u_int32_t width)
{
    std::string ret("[");
    for (size_t line = 0; line < height; line++)
    {
        if (line == 0)
        {
            ret += "[";
        }
        else
            ret += ",\n [";
        for (size_t col = 0; col < width; col++)
        {
            if (col == 0)
            {
                ret += std::to_string(m[line * width + col]);
            }
            else
            {
                ret += "," + std::to_string(m[line * width + col]);
            }
        }
        ret += "]";
    }
    return ret + "]";
}

std::string spreadsheetPrinter(std::vector<u_int8_t> m, u_int32_t height, u_int32_t width, std::vector<std::string> linesDescription)
{
    if (linesDescription.size() != height)
        return "ERROR: height != linesDescription";

    u_int8_t h_indicesSize = 1;
    for (u_int32_t i = height; i >= 10; i /= 10)
        h_indicesSize++;
    u_int8_t w_indicesSize = 1;
    for (u_int32_t i = width; i >= 10; i /= 10)
        w_indicesSize++;

    std::ostringstream oss;
    addNChars(oss, " ", h_indicesSize);
    if (height == 0)
    {
        for (size_t i = 0; i < width; i++)
        {
            oss << "┃";
            addNumber(oss, i, w_indicesSize);
        }
        return oss.str();
    }
    if (width == 0)
    {
        for (size_t i = 0; i < height; i++)
        {
            oss << "\n";
            addNChars(oss, "━", h_indicesSize);
            oss << "\n";
            addRightNumber(oss, i, h_indicesSize);
        }
        return oss.str();
    }

    for (size_t i = 0; i < width; i++)
    {
        oss << "┃";
        if (i % 2)
            oss << BOLD;
        addNumber(oss, i, w_indicesSize);
        if (i % 2)
            oss << NORMAL;
    }

    for (size_t line = 0; line < height; line++)
    {
        std::string currentLineBG = line % 2 ? NORMAL : BLACK_DESAT_BACKGROUND;
        oss << "\n";
        addNChars(oss, "━", h_indicesSize);
        if (line == 0)
        {
            oss << "╋";
            addNChars(oss, "━", w_indicesSize);
            addNChars(oss, "╇" + nTimesString("━", w_indicesSize), width - 1);
        }
        else
        {
            oss << "╉";
            addNChars(oss, "─", w_indicesSize);
            addNChars(oss, "┼" + nTimesString("─", w_indicesSize), width - 1);
        }
        oss << "\n"
            << currentLineBG;
        addRightNumber(oss, line, h_indicesSize);

        for (size_t col = 0; col < width; col++)
        {
            if (col == 0)
                oss << "┃";
            else
                oss << "│";
            if (m[line * width + col])
                oss << WHITE_DESAT_BACKGROUND << BLACK_NORMAL_COLOR;
            if (col % 2)
                oss << BOLD;
            addNumber(oss, m[line * width + col], w_indicesSize);

            oss << NORMAL << currentLineBG;
        }

        if (linesDescription[line].size())
            oss << " -> " << linesDescription[line];
        oss << NORMAL;
    }
    return oss.str();
}