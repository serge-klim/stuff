#include "fixeds.hpp"
#include "cool_v4.hpp"
#include <benchmark/benchmark.h>
#include <vector>

template<typename String>
void emplace_in_vector(benchmark::State& state)
{
   std::vector<String> strings;
   strings.reserve(1000);
   for (auto _ : state) {
      strings.emplace_back("test_string");
   }
}


BENCHMARK_TEMPLATE(emplace_in_vector, FixedSizeString)->Iterations(1000);
BENCHMARK_TEMPLATE(emplace_in_vector, cool::Shtring)->Iterations(1000);

BENCHMARK_MAIN();