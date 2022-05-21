cmake_minimum_required(VERSION 3.10)

project(AoC_2019_C C)

include_directories(${AOC_UTIL_INSTALL}/include)

link_directories(${AOC_UTIL_INSTALL}/lib/)

add_executable(
    aoc
    src/main.c
    src/intcode.c
    src/day1.c
    src/day2.c
    src/day3.c
    src/day4.c
    src/day5.c
    src/day6.c
    src/day7.c
    src/day8.c
    src/day9.c
    src/day10.c
    src/day11.c
    src/day12.c
    src/day13.c
    src/day14.c
    src/day15.c
    src/day16.c
    src/day17.c
    src/day18.c
    src/day19.c
    src/day20.c
    # src/day21.c
    # src/day22.c
    # src/day23.c
    # src/day24.c
    # src/day25.c
)

target_link_libraries(aoc aoc_util pthread pcre2-8)

set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
