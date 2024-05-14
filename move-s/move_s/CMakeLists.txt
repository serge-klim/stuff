cmake_minimum_required (VERSION 3.8)
find_package( Boost REQUIRED )

add_executable (move_s
        fixeds.hpp
        cool_v1.hpp
        cool_v2.hpp
        cool_v3.hpp
        cool_v4.hpp
        cool_v4.5.hpp
        cool_v5.hpp
        cool_v6.hpp
        xstr.hpp
        move_s.cpp
#        over_match_old.cpp
        over_match.cpp
        )

target_link_libraries(move_s
        Boost::boost
)


#target_compile_options(move_s PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /Ox>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
#)

set_property(TARGET move_s PROPERTY CXX_STANDARD 98)
