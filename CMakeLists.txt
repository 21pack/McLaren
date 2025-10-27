cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()
include(CTest)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.0 
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(EnTT)

add_library(engine STATIC 
    engine/engine.cpp
    engine/render.cpp
    engine/camera.cpp
    engine/input.cpp
    engine/image_manager.cpp
    engine/systems.cpp
)

target_include_directories(engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SFML_INCLUDE_DIR}
)

target_compile_features(engine PRIVATE cxx_std_17)
target_link_libraries(engine PRIVATE SFML::Graphics EnTT::EnTT)

add_executable(main 
    game/main.cpp
    game/loops/game_loop.cpp
)

target_link_libraries(main PRIVATE engine SFML::Graphics EnTT::EnTT)

if(BUILD_TESTING)
    message(STATUS "Building tests")

    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG    v1.17.0 
        GIT_SHALLOW  ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(run_tests
        engine/tests/camera_test.cpp
    )

    target_link_libraries(run_tests PRIVATE
        engine
        GTest::gtest
        GTest::gtest_main
        SFML::Graphics EnTT::EnTT
    )
    
    add_test(NAME engine_tests COMMAND run_tests)

endif()
