cmake_minimum_required(VERSION 3.28)
project(Crazy-Eights LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(CrazyEights src/main.cpp include/Random.hpp)

add_custom_command(TARGET CrazyEights POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_directory
       ${CMAKE_SOURCE_DIR}/Card\ Asset $<TARGET_FILE_DIR:CrazyEights>/Card\ Asset)

target_compile_features(CrazyEights PRIVATE cxx_std_17)
target_link_libraries(CrazyEights PRIVATE SFML::Graphics)
