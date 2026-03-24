add_library(Chip8 Chip8.cpp)
target_include_directories(Chip8 PUBLIC ${PROJECT_SOURCE_DIR})

add_library(Platform Platform.cpp)
target_link_libraries(Platform PRIVATE SDL3::SDL3)
target_include_directories(Platform PUBLIC ${CMAKE_SOURCE_DIR})