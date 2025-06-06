#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>



void insertDn2BCSR(std::vector<u_int32_t> &i_ptr, std::vector<u_int32_t> &indces, u_int32_t width, u_int32_t height, u_int8_t **values)
{
    i_ptr[0] = 0;

    for (u_int32_t row = 0; row < height; ++row)
    {
        for (u_int32_t col = 0; col < width; ++col)
        {
            if (values[row][col])
            {
                indces.push_back(col);
            }
        }
        i_ptr[row + 1] = indces.size();
    }
}

BCSR::BCSR(u_int32_t width, u_int32_t height) : _width(width), _height(height)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
}

BCSR::BCSR(u_int32_t width, u_int32_t height, u_int8_t **values) : _width(width), _height(height)
{
    _index_pointers = std::vector<u_int32_t>(height + 1);
    _indices = std::vector<u_int32_t>();
}
// CSR::~CSR()
// {
// }