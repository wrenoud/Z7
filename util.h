// SPDX-FileCopyrightText: © 2025 Javier Jimenez Shaw <https://github.com/jjimenezshaw>
// SPDX-FileCopyrightText: © 2025 Weston James Renoud <https://github.com/wrenoud>

#ifndef Z7_UTIL_H
#define Z7_UTIL_H

#include <cstddef>
#include <cstdint>
#if __cplusplus >= 202002L
#include <bit>
#elif defined(_MSC_VER)
#include <intrin.h>
#endif

namespace Z7::Utils {
constexpr int bit_width(uint64_t x) {
    // For multiple reasons we need to handle zero explicitly.
    if (x == 0)
        return 0;
#if __cplusplus >= 202002L
    // Use the standard library function if available.
    return std::bit_width(x);
#elif defined(_MSC_VER)
    // Fallback to MSVC intrinsic if available.
    unsigned long index = 0;
    _BitScanReverse64(&index, x);
    return static_cast<int>(index) + 1;
#else
    // Fallback to GCC/Clang built-in function.
    return 64 - __builtin_clzll(x);
#endif
}

constexpr size_t countl_zero(uint64_t x) {
    // For multiple reasons we need to handle zero explicitly.
    if (x == 0)
        return 64;
#if __cplusplus >= 202002L
    // Use the standard library function if available.
    return std::countl_zero(x);
#elif defined(_MSC_VER)
    // Fallback to MSVC intrinsic if available.
    unsigned long index = 0;
    _BitScanReverse64(&index, x);
    return 63 - index;
#else
    // Fallback to GCC/Clang built-in function.
    return __builtin_clzll(x);
#endif
}

constexpr size_t countr_zero(uint64_t x) {
    // For multiple reasons we need to handle zero explicitly.
    if (x == 0)
        return 64;
#if __cplusplus >= 202002L
    // Use the standard library function if available.
    return std::countr_zero(x);
#elif defined(_MSC_VER)
    // Fallback to MSVC intrinsic if available.
    unsigned long index = 0;
    _BitScanForward64(&index, x);
    return index;
#else
    // Fallback to GCC/Clang built-in function.
    return __builtin_ctzll(x);
#endif
}

constexpr size_t countr_one(uint64_t x) {
    return countr_zero(~x);
}

} // namespace Z7::Utils

#endif // Z7_UTIL_H
