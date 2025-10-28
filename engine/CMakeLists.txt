file(GLOB SOURCES_CXX *.cpp)

add_library(engine ${SOURCES_CXX})
target_link_libraries(engine PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    EnTT::EnTT
    cereal)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add_subdirectory(tests)
