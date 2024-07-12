cmake_minimum_required(VERSION 3.27)

project(aurora_sort_fail_example CXX)

add_subdirectory(kokkos)

add_executable(sort-fail sort-fail.cxx)

target_link_libraries(sort-fail Kokkos::kokkos)
