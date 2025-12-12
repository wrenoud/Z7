#ifndef Z7_UTIL_H
#define Z7_UTIL_H

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
} // namespace Z7::Utils

#endif // Z7_UTIL_H
