cmake_minimum_required (VERSION 3.8)


find_package(benchmark REQUIRED)

add_executable (benchmark
        benchmark.cpp
        fixeds.hpp
        cool_v4.hpp
        )

target_link_libraries(benchmark
        benchmark::benchmark
        )

#target_compile_options(benchmark PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /Ox>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
#)
