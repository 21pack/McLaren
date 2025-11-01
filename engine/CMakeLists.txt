file(GLOB_RECURSE SOURCES_CXX *.cpp)
list(FILTER SOURCES_CXX EXCLUDE REGEX ".*/tests/.*")

add_library(engine ${SOURCES_CXX})
target_link_libraries(engine PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    EnTT::EnTT
    cereal)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
