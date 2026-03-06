set(TEST_SUITE_NAME tests)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${TEST_SUITE_NAME} test.cpp)
target_link_libraries(${TEST_SUITE_NAME} PUBLIC GTest::gtest_main simple_library)

include(GoogleTest)
gtest_discover_tests(${TEST_SUITE_NAME})
