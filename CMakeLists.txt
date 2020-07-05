cmake_minimum_required (VERSION 2.8)

project (auto-mm)

set(CMAKE_EXE_LINKER_FLAGS "-static -O3")

aux_source_directory(src SRCS)
add_executable(auto-mm ${SRCS})
