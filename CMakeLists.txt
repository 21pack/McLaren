cmake_minimum_required(VERSION 3.28)
project(McLaren CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.0
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.2
    GIT_SHALLOW ON)
FetchContent_GetProperties(cereal)
if(NOT cereal_POPULATED)
    FetchContent_Populate(cereal)
endif()

add_library(cereal INTERFACE)
target_include_directories(cereal INTERFACE ${cereal_SOURCE_DIR}/include)

add_subdirectory(engine)
add_subdirectory(game)

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build PROPERTY
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/WX->;$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wno-error>"
)
