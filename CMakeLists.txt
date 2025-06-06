cmake_minimum_required(VERSION 3.10)
project(demo)
set(CMAKE_CXX_STANDARD 17)

# Adding sources
set(SOURCES
    src/main.cc
    src/csr/bcsr.cc
)

# Binary
add_executable(demo ${SOURCES})
set_property(TARGET demo PROPERTY CXX_STANDARD 17)
target_compile_features(demo PRIVATE cxx_std_17)