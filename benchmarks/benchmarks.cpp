// SPDX-FileCopyrightText: © 2025 Javier Jimenez Shaw <https://github.com/jjimenezshaw>
// SPDX-FileCopyrightText: © 2025 Weston James Renoud <https://github.com/wrenoud>

#include <benchmark/benchmark.h>

#include "../library.h"

static void Addition(benchmark::State& state, const Z7::Z7Index& a, const Z7::Z7Index& b)
{
    // Perform setup here

    for (auto _ : state)
    {
        // This code gets timed
        const Z7::Z7Index r = a + b;
        benchmark::DoNotOptimize(r);
    }
}

static void Neighbors(benchmark::State& state, const Z7::Z7Index& a)
{
    // Perform setup here

    for (auto _ : state)
    {
        // This code gets timed
        const auto neighbors = Z7::neighbors(a, {});
        benchmark::DoNotOptimize(neighbors);
    }
}


// Register the function as a benchmark, multiple times to see consistency
BENCHMARK_CAPTURE(Addition, 1101111111111111156435 + 1101111111111111142431, "1101111111111111156435"_Z7, "1101111111111111142431"_Z7);
BENCHMARK_CAPTURE(Addition, 1100000000000000156435 + 1100000000000000142431, "1100000000000000156435"_Z7, "1100000000000000142431"_Z7);
BENCHMARK_CAPTURE(Addition, 0800447777777777777777 + 0800567777777777777777, "080044"_Z7, "080056"_Z7);
BENCHMARK_CAPTURE(Addition, 1100000000000000156435 + 1101111111111111142431, "1100000000000000156435"_Z7, "1101111111111111142431"_Z7);
BENCHMARK_CAPTURE(Addition, 1100000000000000156435 + 1100000000000000142431, "1100000000000000156435"_Z7, "1100000000000000142431"_Z7);
BENCHMARK_CAPTURE(Addition, 0800447777777777777777 + 0800567777777777777777, "080044"_Z7, "080056"_Z7);

BENCHMARK_CAPTURE(Neighbors, neighbors of 1101111111111111156435, "1101111111111111156435"_Z7);
BENCHMARK_CAPTURE(Neighbors, neighbors of 0800447777777777777777, "080044"_Z7);
BENCHMARK_CAPTURE(Neighbors, neighbors of 1101111111111111156435, "1101111111111111156435"_Z7);
BENCHMARK_CAPTURE(Neighbors, neighbors of 0800447777777777777777, "080044"_Z7);

BENCHMARK_MAIN();
