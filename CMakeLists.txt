cmake_minimum_required(VERSION 3.28)
project(Chip8)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
