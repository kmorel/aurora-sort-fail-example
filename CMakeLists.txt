cmake_minimum_required(VERSION 3.27)

project(aurora_sort_fail_example CXX)

find_package(Kokkos REQUIRED)

add_executable(sort-fail sort-fail.cxx)

target_link_libraries(sort-fail Kokkos::kokkos)
