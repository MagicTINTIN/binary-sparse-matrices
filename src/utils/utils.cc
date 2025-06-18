#include <algorithm>
#include <vector>

#include "utils.hh"

// FIXME: classic cpp template moment
// template<typename T>
// bool insertByValue(std::vector<T> &vec, const size_t &begin, const size_t &end, const T &value)
// {
//     auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

//     // already exists
//     if (it != (vec.begin() + end) && *it == value)
//         return false;
    
//     vec.insert(it, value);
//     return true;
// }


// template<typename T>
// bool insertByValue(std::vector<T> &vec, const T &value) {
//     return insertByValue<T>(vec, (size_t)0, (size_t)vec.size(), value);
// }

bool insertByValue(std::vector<u_int32_t> &vec, const u_int32_t &value) {
    return insertByValue(vec, 0, (u_int32_t) vec.size(), value);
}

bool insertByValue(std::vector<u_int32_t> &vec, const u_int32_t &begin, const u_int32_t &end, const u_int32_t &value)
{
    auto it = std::lower_bound(vec.begin() + begin, vec.begin() + end, value);

    // already exists
    if (it != vec.end() && *it == value)
        return false;
    
    vec.insert(it, value);
    return true;
}