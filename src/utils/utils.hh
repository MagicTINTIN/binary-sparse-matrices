#pragma once
#ifndef UTILS_HH
#define UTILS_HH

#include <vector>
#include <sys/types.h>
#include <algorithm>

// bool insertByValue(std::vector<u_int32_t> &vec, const u_int32_t &value);

// /**
//  * @warning no check that begin < end!
//  */
// bool insertByValue(std::vector<u_int32_t> &vec, const u_int32_t &begin, const u_int32_t &end, const u_int32_t &value);

template<typename T>
/**
 * @warning no check that begin < end!
 */
bool insertByValue(std::vector<T> &vec, const size_t &begin, const size_t &end, const T &value)
{
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // already exists
    if (it != (vec.begin() + end) && *it == value)
        return false;
    
    vec.insert(it, value);
    return true;
}

template<typename T>
bool insertByValue(std::vector<T> &vec, const size_t &begin, const T &value) {
    return insertByValue(vec, begin, vec.size(), value);
}

template<typename T>
bool insertByValue(std::vector<T> &vec, const T &value) {
    return insertByValue(vec, 0, vec.size(), value);
}


template<typename T>
bool removeByValue(std::vector<T> &vec,
                   size_t begin,
                   size_t end,
                   const T &value)
{
    // locate the first element >= value
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // if we are at end or *it != value, nothing to erase
    if (it == vec.begin() + end || *it != value)
        return false;

    vec.erase(it);
    return true;
}

template<typename T>
bool removeByValue(std::vector<T> &vec, const T &value)
{
    return removeByValue(vec, 0, vec.size(), value);
}

template<typename T>
bool isValueIn(std::vector<T> const &vec,
                   size_t begin,
                   size_t end,
                   const T &value)
{
    // locate the first element >= value
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // if we are at end or *it != value, not present
    if (it == vec.begin() + end || *it != value)
        return false;
    return true;
}

template<typename T>
bool isValueIn(std::vector<T> const &vec, const T &value)
{
    return isValueIn(vec, 0, vec.size(), value);
}

// template<typename T>
// bool insertByValue(std::vector<T> &vec, const size_t &begin, const size_t &end, const T &value);
// template<typename T>
// bool insertByValue(std::vector<T> &vec, const T &value);

#endif // UTILS_HH