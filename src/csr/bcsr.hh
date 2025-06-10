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
    void insertDn2BCSR(u_int8_t values[]);

public:
    BCSR(u_int32_t height, u_int32_t width);
    BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]);
    std::vector<u_int8_t> toDenseVector() const;
    std::string toString() const;
    std::string toDnString() const;
    // ~BCSR();
};

std::ostream &operator<<(std::ostream &stream, BCSR &matrix);
std::ostream &operator<<(std::ostream &stream, const BCSR &matrix);

#endif // BCSR_HH