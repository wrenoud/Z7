#include "library.h"

#include <array>
#include <iostream>
#include <sstream>

void hello() { std::cout << "Hello, World!" << std::endl; }

// https://en.wikipedia.org/wiki/Generalized_balanced_ternary#Addition_table_2

// Should be defined with size 8 for memory padding?
static inline std::array<std::array<uint8_t, 7>, 7> addition_table_0{{{0, 1, 2, 3, 4, 5, 6},
                                                                      {1, 2, 3, 4, 5, 6, 0},
                                                                      {2, 3, 4, 5, 6, 0, 1},
                                                                      {3, 4, 5, 6, 0, 1, 2},
                                                                      {4, 5, 6, 0, 1, 2, 3},
                                                                      {5, 6, 0, 1, 2, 3, 4},
                                                                      {6, 0, 1, 2, 3, 4, 5}}};

static inline std::array<std::array<uint8_t, 7>, 7> addition_table_1{{{0, 0, 0, 0, 0, 0, 0},
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
        auto r0 = addition_table_0[va][vb];
        if (r1 != 0) {
            carries_next[carries_next.resolution() + 1] = r1;
        }
        for (int c = 1; c <= carries_prev.resolution(); c++) {
            const auto carry = carries_prev[c];
            r1 = addition_table_1[r0][carry];
            r0 = addition_table_0[r0][carry];
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
} // namespace Z7
