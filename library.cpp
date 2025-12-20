// SPDX-FileCopyrightText: © 2025 Javier Jimenez Shaw <https://github.com/jjimenezshaw>
// SPDX-FileCopyrightText: © 2025 Weston James Renoud <https://github.com/wrenoud>

#include "library.h"

#include <array>
#include <iostream>
#include <sstream>
#include <tuple>

void hello() { std::cout << "Hello, World!" << std::endl; }

// https://en.wikipedia.org/wiki/Generalized_balanced_ternary#Addition_table_2

namespace GBT::Addition {
namespace CW {
constexpr std::array<std::array<uint8_t, 7>, 7> addition_table_0{{{0, 1, 2, 3, 4, 5, 6},
                                                                  {1, 4, 3, 6, 5, 2, 0},
                                                                  {2, 3, 1, 4, 6, 0, 5},
                                                                  {3, 6, 4, 5, 0, 1, 2},
                                                                  {4, 5, 6, 0, 2, 3, 1},
                                                                  {5, 2, 0, 1, 3, 6, 4},
                                                                  {6, 0, 5, 2, 1, 4, 3}}};

// TODO - Should be defined with size 8 for memory padding?
constexpr std::array<std::array<uint8_t, 7>, 7> addition_table_1{{{0, 0, 0, 0, 0, 0, 0},
                                                                  {0, 1, 0, 1, 0, 5, 0},
                                                                  {0, 0, 2, 3, 0, 0, 2},
                                                                  {0, 1, 3, 3, 0, 0, 0},
                                                                  {0, 0, 0, 0, 4, 4, 6},
                                                                  {0, 5, 0, 0, 4, 5, 0},
                                                                  {0, 0, 2, 0, 6, 0, 6}}};

constexpr std::pair<uint8_t, uint8_t> lookup(uint8_t a, uint8_t b) {
    return {addition_table_1[a][b], addition_table_0[a][b]};
}
} // namespace CW

namespace CCW {
// constexpr std::array<std::array<uint8_t, 7>, 7> addition_table_0{{{0, 1, 2, 3, 4, 5, 6},
//                                                                   {1, 2, 3, 4, 5, 6, 0},
//                                                                   {2, 3, 4, 5, 6, 0, 1},
//                                                                   {3, 4, 5, 6, 0, 1, 2},
//                                                                   {4, 5, 6, 0, 1, 2, 3},
//                                                                   {5, 6, 0, 1, 2, 3, 4},
//                                                                   {6, 0, 1, 2, 3, 4, 5}}};

// Instead of 2D array, we can just use mod 7 addition. So where we had addition_table_0[a][b],
// we can instead do mod_7_table[a + b].
constexpr std::array<uint8_t, 14> mod_7_table{0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6};

// TODO - Should be defined with size 8 for memory padding?
constexpr std::array<std::array<uint8_t, 7>, 7> addition_table_1{{{0, 0, 0, 0, 0, 0, 0},
                                                                  {0, 1, 0, 3, 0, 1, 0},
                                                                  {0, 0, 2, 2, 0, 0, 6},
                                                                  {0, 3, 2, 3, 0, 0, 0},
                                                                  {0, 0, 0, 0, 4, 5, 4},
                                                                  {0, 1, 0, 0, 5, 5, 0},
                                                                  {0, 0, 6, 0, 4, 0, 6}}};

constexpr std::pair<uint8_t, uint8_t> lookup(uint8_t a, uint8_t b) {
    return {addition_table_1[a][b], mod_7_table[a + b]};
}
} // namespace CCW
} // namespace GBT::Addition

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
        const bool isEvenResolution = (i % 2 == 0);
        const Z7Index carries_prev(carries_next.index);
        carries_next = Z7Index(std::numeric_limits<uint64_t>::max());
        const auto va = a[i];
        const auto vb = b[i];
        uint8_t r1, r0;
        if (isEvenResolution)
            std::tie(r1, r0) = GBT::Addition::CCW::lookup(va, vb);
        else
            std::tie(r1, r0) = GBT::Addition::CW::lookup(va, vb);
        if (r1 != 0) {
            carries_next[carries_next.resolution() + 1] = r1;
        }
        const auto carry_count = carries_prev.resolution();
        for (int c = 1; c <= carry_count; c++) {
            const auto carry = carries_prev[c];
            if (isEvenResolution)
                std::tie(r1, r0) = GBT::Addition::CCW::lookup(r0, carry);
            else
                std::tie(r1, r0) = GBT::Addition::CW::lookup(r0, carry);
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
Z7_carry neighbor(const Z7Index &ref, size_t resolution) {
    static_assert(0 < N && N < 7, "N must be in 1..6");
    Z7_carry res{ref, 0};

    // Add the direction digit.
    const auto v = ref[resolution];
    uint8_t carry;
    uint8_t r0;
    if (resolution % 2 == 0)
        std::tie(carry, r0) = GBT::Addition::CCW::lookup(v, N);
    else
        std::tie(carry, r0) = GBT::Addition::CW::lookup(v, N);
    res.z7[resolution] = r0;
    if (carry == 0)
        return res;

    // Propagate the carry.
    for (auto i = --resolution; i > 0; --i) {
        const auto v = ref[i];
        if (i % 2 == 0)
            std::tie(carry, r0) = GBT::Addition::CCW::lookup(v, carry);
        else
            std::tie(carry, r0) = GBT::Addition::CW::lookup(v, carry);
        res.z7[i] = r0;
        if (carry == 0)
            return res;
    }

    // If we still have a carry after handling all digits then we're out of bounds.
    if (carry != 0) {
        res.carry = carry;
        // res.hierarchy.base = 15; // invalid base zone. Leave the rest, that can be useful.
    }
    return res;
}

template Z7_carry neighbor<1>(const Z7Index &ref, size_t resolution);
template Z7_carry neighbor<2>(const Z7Index &ref, size_t resolution);
template Z7_carry neighbor<3>(const Z7Index &ref, size_t resolution);
template Z7_carry neighbor<4>(const Z7Index &ref, size_t resolution);
template Z7_carry neighbor<5>(const Z7Index &ref, size_t resolution);
template Z7_carry neighbor<6>(const Z7Index &ref, size_t resolution);

std::array<Z7Index, 6> neighbors(const Z7Index &ref, const Z7Configuration &config) {
    constexpr uint8_t size = 6;

    const auto resolution = ref.resolution();
    const auto exclusion = config.exclusion_zone[ref.hierarchy.base];

    // base only
    if (ref.hierarchy.i01 == 7) {
        // this is a special case. The return depends on the config data.
        // TODO
        std::array<Z7Index, size> result;
        result.fill(Z7Index::invalid());
        return result;
    }

    // create the neighbors
    std::array<Z7_carry, size> result_carry = {neighbor<1>(ref, resolution), neighbor<2>(ref, resolution),
                                               neighbor<3>(ref, resolution), neighbor<4>(ref, resolution),
                                               neighbor<5>(ref, resolution), neighbor<6>(ref, resolution)};
    for (auto &r: result_carry) {
        if (r.carry != 0) {
            r.z7.hierarchy.base = config.neighbor_zones[ref.hierarchy.base][r.carry - 1];
            if (r.z7.hierarchy.base == 0 || r.z7.hierarchy.base == 11) {
                // coming from tropical zone to polar zone 0. Rotate the neighbors
                auto rotations = config.rotations[ref.hierarchy.base];
                if (ref.hierarchy.i01 == 6 || ref.hierarchy.i01 == 1)
                    rotations++;
                for (int j = 0; j < rotations; j++) {
                    for (int i = 1; i <= resolution; i++) {
                        r.z7[i] = (*r.z7[i] * 5) % 7;
                    }
                }
            }
        }
    }
    std::array<Z7Index, size> result{
            result_carry[0].z7, result_carry[1].z7, result_carry[2].z7,
            result_carry[3].z7, result_carry[4].z7, result_carry[5].z7,
    };

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
