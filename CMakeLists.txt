cmake_minimum_required(VERSION 3.10)
project(CoralisProject)

# C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3")

# Include directories (so all targets can see headers)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Enable testing (CTest)
enable_testing()

# Add source subdirectory (defines coralis_lib and coralis)
add_subdirectory(src)

# Add tests subdirectory
add_subdirectory(tests)