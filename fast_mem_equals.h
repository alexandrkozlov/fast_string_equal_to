#pragma once

#include <cstdint>
#include <string>

/*
 * Uses SSE intrinsics to test string equality.
 */
bool fast_mem_equals(const uint8_t* pStr1, const uint8_t *pStr2, std::size_t count);

inline bool fast_is_equals(const std::string& s1, const std::string& s2)
{
    return s1.size() == s2.size() && fast_mem_equals((const uint8_t*)s1.c_str(), (const uint8_t*)s2.c_str(), s1.size());
}

/*
 * Fast "equal_to" comparator for std::string
 */
struct fast_string_equal_to
{
    inline bool operator()(const std::string& _Left, const std::string& _Right) const
    {
        return fast_is_equals(_Left, _Right);
    }
};
