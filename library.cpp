#include "library.h"

#include <array>
#include <iostream>
#include <sstream>

void hello() { std::cout << "Hello, World!" << std::endl; }

// https://en.wikipedia.org/wiki/Generalized_balanced_ternary#Addition_table_2


static constexpr std::array<uint8_t, 14> mod_7_table{0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6};

// TODO - Should be defined with size 8 for memory padding?
static constexpr std::array<std::array<uint8_t, 7>, 7> addition_table_1{{{0, 0, 0, 0, 0, 0, 0},
                                                                      {0, 1, 0, 3, 0, 1, 0},
                                                                      {0, 0, 2, 2, 0, 0, 6},
                                                                      {0, 3, 2, 3, 0, 0, 0},
                                                                      {0, 0, 0, 0, 4, 5, 4},
                                                                      {0, 1, 0, 0, 5, 5, 0},
                                                                      {0, 0, 6, 0, 4, 0, 6}}};
namespace Z7 {

std::string Z7Index::str() const {
    std::stringstream ss;
    ss << (hierarchy.base < 10 ? "0" : "");
    ss << hierarchy.base;
    for (int i = 1; i <= 20; i++) {
        auto r = this->operator[](i);
        if (r >= 7)
            break;
        ss << r;
    }
    return ss.str();
}

Z7Index operator-(const Z7Index &a) {
    Z7Index res{a.index};
    for (int i = a.resolution(); i >= 1; i--) {
        const auto e = a[i];
        res[i] = (e == 0 ? 0 : 7 - e);
    }
    return res;
}

Z7Index operator+(const Z7Index &a, const Z7Index &b) {
    Z7Index res{std::numeric_limits<uint64_t>::max()};

    if (a.hierarchy.base != b.hierarchy.base) {
        return res; // invalid
    }

    const int resolution = std::max(a.resolution(), b.resolution());
    res.hierarchy.base = a.hierarchy.base;
    // use an index to store the carries.
    Z7Index carries_next{std::numeric_limits<uint64_t>::max()};
    for (int i = resolution; i >= 1; i--) {
        const Z7Index carries_prev(carries_next.index);
        carries_next = Z7Index(std::numeric_limits<uint64_t>::max());
        const auto va = a[i];
        const auto vb = b[i];
        auto r1 = addition_table_1[va][vb];
        auto r0 = mod_7_table[va + vb];
        if (r1 != 0) {
            carries_next[carries_next.resolution() + 1] = r1;
        }
        const auto carry_count = carries_prev.resolution();
        for (int c = 1; c <= carry_count; c++) {
            const auto carry = carries_prev[c];
            r1 = addition_table_1[r0][carry];
            r0 = mod_7_table[r0 + carry];
            if (r1 != 0) {
                carries_next[carries_next.resolution() + 1] = r1;
            }
        }
        res[i] = r0;
    }
    if (carries_next.resolution() > 0) {
        // there is some carry not  included. Return invalid
        res = Z7Index(std::numeric_limits<uint64_t>::max());
    }
    return res;
}

// Because we're adding a single known digit we can optimize the addition. This is just a very specific case of above.
template<size_t N>
Z7Index neighbor(const Z7Index &ref, size_t resolution) {
    Z7Index res{ref};

    // Add the direction digit.
    const auto v = ref[resolution];
    res[resolution] = mod_7_table[v + N];
    auto carry = addition_table_1[v][N];
    if (carry == 0)
        return res;

    // Propagate the carry.
    for (auto i = --resolution; i > 0; --i) {
        const auto v = ref[i];
        res[i] = mod_7_table[v + carry];
        carry = addition_table_1[v][carry];
        if (carry == 0)
            return res;
    }

    // If we still have a carry after handling all digits then we're out of bounds.
    if (carry != 0) {
        return Z7Index(std::numeric_limits<uint64_t>::max());
    }
    return res;
}

template Z7Index neighbor<1>(const Z7Index& ref, size_t resolution);
template Z7Index neighbor<2>(const Z7Index& ref, size_t resolution);
template Z7Index neighbor<3>(const Z7Index& ref, size_t resolution);
template Z7Index neighbor<4>(const Z7Index& ref, size_t resolution);
template Z7Index neighbor<5>(const Z7Index& ref, size_t resolution);
template Z7Index neighbor<6>(const Z7Index& ref, size_t resolution);

constexpr size_t first_non_zero(const Z7Index& f) {
    if (f.hierarchy.i01 == 7)
        return 0;

    // mask out the base and count leading zeros
    constexpr uint64_t base_mask = ~(0b1111ULL << 60);
    return (Utils::countl_zero(f.index & base_mask) - 4) / 3 + 1;
};

std::array<Z7Index, 6> neighbors(const Z7Index &ref, const Z7Configuration &config) {
    constexpr uint8_t size = 6;
    std::array<Z7Index, size> result;
    result.fill(Z7Index::invalid());

    const auto resolution = ref.resolution();
    const auto exclusion = config.exclusion_zone[ref.hierarchy.base];

    // base only
    if (ref.hierarchy.i01 == 7) {
        // this is a special case. The return depends on the config data.
        // TODO
        return result;
    }

    // create the neigbors
    {
        Z7Index direction;
        direction.hierarchy.base = ref.hierarchy.base;
        for (int pos = resolution; pos < 20; pos++)
            direction[pos + 1] = 7; // maybe we can do it without a loop.

        for (uint8_t i = 0; i < size; i++) {
            direction[resolution] = i + 1;
            result[i] = ref + direction;
        }
    }

    // if we are in a penthagon we invalidate one neighbor here.
    const uint64_t data_only = (ref.index & ~(0b1111ULL << (20 * 3))) >> (3 * (20 - resolution));
    if (data_only == 0 && exclusion > 0 && exclusion <= 6) {
        result[exclusion - 1] = Z7Index::invalid();
        return result;
    }


    // move points out of the exclusion zone
    const auto ref_first_non_zero = first_non_zero(ref);
    if (ref_first_non_zero < 1)
        return result; // TODO here we should move to a different face in the dodecahedron, probably earlier in the
                       // code.

    // find out how we should rotate the cells if needed
    const auto reference_zone = ref[ref_first_non_zero];
    uint8_t multiplier = 0;
    if ((reference_zone * 5) % 7 == exclusion) {
        multiplier = 5; // rotate counterclockwise
    } else if ((reference_zone * 3) % 7 == exclusion) {
        multiplier = 3; // rotate clockwise
    }

    // rotate needed cells
    if (multiplier > 0) {
        for (auto &elem: result) {
            const auto to_rotate = first_non_zero(elem);
            if (*elem[to_rotate] == exclusion) {
                for (int i = to_rotate; i <= elem.resolution(); i++) {
                    elem[i] = (*elem[i] * multiplier) % 7;
                }
            }
        }
    }

    return result;
}

} // namespace Z7
