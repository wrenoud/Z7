#ifndef Z7_LIBRARY_H
#define Z7_LIBRARY_H

#include <cstdint>
#include <random>
#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace Z7 {

namespace Utils {
constexpr int bit_width(uint64_t x) {
    if (x == 0)
        return 0;
#if defined(_MSC_VER)
    unsigned long index = 0;
    _BitScanReverse64(&index, x);
    return static_cast<int>(index) + 1;
#else
    return 64 - __builtin_clzll(x);
#endif
}
} // namespace Utils
struct Z7Index {
    union {
        uint64_t index;
        struct {
            uint64_t i20 : 3; // 3 bits
            uint64_t i19 : 3; // 6 bits
            uint64_t i18 : 3; // 9 bits
            uint64_t i17 : 3; // 12 bits
            uint64_t i16 : 3; // 15 bits
            uint64_t i15 : 3; // 18 bits
            uint64_t i14 : 3; // 21 bits
            uint64_t i13 : 3; // 24 bits
            uint64_t i12 : 3; // 27 bits
            uint64_t i11 : 3; // 30 bits
            uint64_t i10 : 3; // 33 bits
            uint64_t i09 : 3; // 36 bits
            uint64_t i08 : 3; // 39 bits
            uint64_t i07 : 3; // 42 bits
            uint64_t i06 : 3; // 45 bits
            uint64_t i05 : 3; // 48 bits
            uint64_t i04 : 3; // 51 bits
            uint64_t i03 : 3; // 54 bits
            uint64_t i02 : 3; // 57 bits
            uint64_t i01 : 3; // 60 bits
            uint64_t base : 4;
        } hierarchy;
    };

    constexpr Z7Index() : index(0) {}

    constexpr explicit Z7Index(uint64_t idx) : index(idx) {}

    // Calculate the bit shift needed to extract the 3-bit value for a given resolution.
    static constexpr uint64_t resolution_shift(uint64_t res) { return (20 - res) * 3; }

    // Extract the 3-bit index value for the specified resolution.
    constexpr uint64_t operator[](uint64_t res) const { return (index >> resolution_shift(res)) & 0b111; }

    struct ResolutionProxy {
        uint64_t &index;
        uint64_t resolution_shift; ///< Bit shift for the specified resolution. This is a multiple of 3, and is in effect a relative offset for the 3 bits.

        // Assign a new 3-bit value for the specified resolution.
        constexpr ResolutionProxy &operator=(uint64_t value) {
            index &= ~(0b111ULL << resolution_shift); // Clear existing bits
            index |= (value & 0b111) << resolution_shift; // Set new bits (being defensive with the mask)
            return *this;
        }

        // Dereference to get the current 3-bit value for the specified resolution.
        constexpr uint64_t operator*() const { return (index >> resolution_shift) & 0b111; }
    };

    constexpr ResolutionProxy operator[](uint64_t res) { return {index, resolution_shift(res)}; }

    // Determine the resolution based on the index. Because unused hierarchy levels are filled, we look for the first
    // zero.
    constexpr int resolution() const { return 20 - ((Utils::bit_width(~index & -~index) - 1) / 3); }
};

} // namespace Z7

inline constexpr Z7::Z7Index operator""_Z7(const char *str, std::size_t) {
    Z7::Z7Index v{std::numeric_limits<uint64_t>::max()};
    const char *p = str;
    if (*p != '\0') v.hierarchy.base = (*p - '0') * 10, ++p;
    if (*p != '\0') v.hierarchy.base += (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i01 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i02 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i03 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i04 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i05 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i06 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i07 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i08 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i09 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i10 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i11 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i12 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i13 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i14 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i15 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i16 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i17 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i18 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i19 = (*p - '0'), ++p;
    if (*p != '\0') v.hierarchy.i20 = (*p - '0'), ++p;

    return v;
}

#endif // Z7_LIBRARY_H
