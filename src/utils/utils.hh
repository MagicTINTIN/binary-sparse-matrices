#pragma once
#ifndef UTILS_HH
#define UTILS_HH

#include <algorithm>
#include <sys/types.h>
#include <vector>

// bool insertByValue(std::vector<uint32_t> &vec, const uint32_t &value);

// /**
//  * @warning no check that begin < end!
//  */
// bool insertByValue(std::vector<uint32_t> &vec, const uint32_t &begin, const uint32_t &end, const uint32_t &value);

template <typename T>
/**
 * @warning no check that begin < end!
 */
bool insertByValue(std::vector<T> &vec, size_t const &begin, size_t const &end, T const &value) {
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // already exists
    if (it != (vec.begin() + end) && *it == value) return false;

    vec.insert(it, value);
    return true;
}

template <typename T> bool insertByValue(std::vector<T> &vec, size_t const &begin, T const &value) {
    return insertByValue(vec, begin, vec.size(), value);
}

template <typename T> bool insertByValue(std::vector<T> &vec, T const &value) {
    return insertByValue(vec, 0, vec.size(), value);
}


template <typename T> bool removeByValue(std::vector<T> &vec, size_t begin, size_t end, T const &value) {
    // locate the first element >= value
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // if we are at end or *it != value, nothing to erase
    if (it == vec.begin() + end || *it != value) return false;

    vec.erase(it);
    return true;
}

template <typename T> bool removeByValue(std::vector<T> &vec, T const &value) {
    return removeByValue(vec, 0, vec.size(), value);
}

template <typename T> bool isValueIn(std::vector<T> const &vec, size_t begin, size_t end, T const &value) {
    // locate the first element >= value
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // if we are at end or *it != value, not present
    if (it == vec.begin() + end || *it != value) return false;
    return true;
}

template <typename T> bool isValueIn(std::vector<T> const &vec, T const &value) {
    return isValueIn(vec, 0, vec.size(), value);
}

// template<typename T>
// bool insertByValue(std::vector<T> &vec, const size_t &begin, const size_t &end, const T &value);
// template<typename T>
// bool insertByValue(std::vector<T> &vec, const T &value);

#endif // UTILS_HH