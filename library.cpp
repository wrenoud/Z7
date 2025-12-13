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
        for (int c = 1; c <= carries_prev.resolution(); c++) {
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

constexpr static inline int first_non_zero(Z7Index f) {
    if (f.hierarchy.i01 == 7)
        return -1;
    // This can be done with bit arithmetics.
    int i = 1;
    for (; i <= 20; i++) {
        if (*f[i] != 0)
            break;
    }
    return i;
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
