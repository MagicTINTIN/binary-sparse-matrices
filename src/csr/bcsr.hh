#pragma once
#ifndef BCSR_HH
#define BCSR_HH

#include <sys/types.h>
#include <vector>
#include <string>

class BCSR
{
private:
    u_int32_t _width, _height;
    std::vector<u_int32_t> _index_pointers;
    std::vector<u_int32_t> _indices;

public:
    BCSR(u_int32_t height, u_int32_t width);
    BCSR(u_int32_t height, u_int32_t width, u_int8_t **values);
    std::vector<std::vector<u_int8_t>> toDenseVector() const;
    std::string toString() const;
    std::string toDnString() const;
    // ~BCSR();
};

#endif // BCSR_HH