cmake_minimum_required(VERSION 3.15)
project(happygb VERSION 1.0
DESCRIPTION "gameboy emulator"
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # Or 14, 17, etc., depending on what you need
set(CMAKE_CXX_STANDARD_REQUIRED True)
#find_package(SDL2 REQUIRED)

file(GLOB SOURCES "src/*.cpp")


include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.0  # Specify the version of SDL2 you want to use
)
FetchContent_MakeAvailable(SDL2)

add_executable(happygb ${SOURCES})
#add_executable(tests tests/json_test.cpp)
target_include_directories(happygb PUBLIC include) #include the include folder

target_link_libraries(happygb SDL2::SDL2)
#target_link_libraries(tests Catch2::Catch2)#put after all executables and libs so they know all components known when running



