cmake_minimum_required(VERSION 3.12)

project(
        labwork12
        VERSION 1.0
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)
