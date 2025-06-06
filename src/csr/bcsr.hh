#pragma once
#ifndef BCSR_HH
#define BCSR_HH

#include <sys/types.h>
#include <vector>

class BCSR
{
private:
    u_int32_t _width, _height;
    std::vector<u_int32_t> _index_pointers;
    std::vector<u_int32_t> _indices;

public:
    BCSR(u_int32_t width, u_int32_t height);
    BCSR(u_int32_t width, u_int32_t height, u_int8_t **values);
    u_int8_t toDense();
    // ~BCSR();
};

#endif // BCSR_HH