file(GLOB_RECURSE SOURCES_CXX *.cpp)
list(REMOVE_ITEM SOURCES_CXX "${CMAKE_CURRENT_SOURCE_DIR}/system_movement_test.cpp")

enable_testing()
include(CTest)

if(BUILD_TESTING)
    message(STATUS "Building tests")

    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(googletest)

    add_executable(run_tests ${SOURCES_CXX})

    target_link_libraries(run_tests PRIVATE
        engine
        GTest::gtest
        GTest::gtest_main
    )

    add_test(NAME engine_tests COMMAND run_tests)
endif()
