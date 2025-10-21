cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main 
    game/main.cpp
    game/game_world.cpp
    game/loops/game_loop.cpp
    game/entities/player.cpp
    engine/engine.cpp
    engine/camera.cpp
    engine/input.cpp
    engine/entity.cpp
    engine/texture.cpp
)

target_include_directories(main PRIVATE ${SFML_INCLUDE_DIR})

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
