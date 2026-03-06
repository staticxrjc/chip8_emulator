set(LIBRARY_NAME_SOURCES
        "library.cpp")
set(LIBRARY_NAME_HEADERS
        "library.h")

add_library(simple_library STATIC library.cpp)

target_include_directories(simple_library PUBLIC "./")
