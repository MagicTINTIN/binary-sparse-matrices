cmake_minimum_required(VERSION 3.10)
project(demo)
set(CMAKE_CXX_STANDARD 17)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Adding sources
set(SOURCES
    src/main.cc
    src/csr/bcsr.cc
    src/csr/bcsr_conversion.cc
    src/csr/bcsr_scipy.cc
    src/lil/blil.cc
    src/lil/blil_conversion.cc
    # src/utils/utils.cc
)

# Binary
add_executable(demo ${SOURCES})
set_property(TARGET demo PROPERTY CXX_STANDARD 17)
target_compile_features(demo PRIVATE cxx_std_17)