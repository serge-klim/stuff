#include "fixeds.hpp"
#include "cool_v6.hpp"
#include "xstr.hpp"
#include <benchmark/benchmark.h>
#include <vector>
#include <string>
static constexpr auto n_itterations = 1000000;

template<typename String>
void emplace_in_vector(benchmark::State& state, char const* string)
{
   std::vector<String> strings;
  // strings.reserve(n_itterations);
   for (auto _ : state) {
      benchmark::DoNotOptimize(strings.emplace_back(string));
   }
}

template<typename String>
void emplace_in_vector_test_string(benchmark::State& state)
{
    emplace_in_vector<String>(state, "test_string");
}

template<typename String>
void emplace_in_vector_test_long_string(benchmark::State& state)
{
    emplace_in_vector<String>(state, "test_longgggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg_string");
}


BENCHMARK_TEMPLATE(emplace_in_vector_test_string, FixedSizeString)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_string, cool::Shtring)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_string, std::string)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_string, XString)->Iterations(n_itterations);

//BENCHMARK_TEMPLATE(emplace_in_vector_test_long_string, FixedSizeString)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_long_string, cool::Shtring)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_long_string, std::string)->Iterations(n_itterations);
BENCHMARK_TEMPLATE(emplace_in_vector_test_long_string, XString)->Iterations(n_itterations);

BENCHMARK_MAIN();