cmake_minimum_required(VERSION 2.0)

project(sdlgame)

add_executable(
    ${PROJECT_NAME}
    "src/main.cpp"
    "src/renderdevice.cpp"
    "src/fpscounter.cpp"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)
target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

file(GLOB_RECURSE srcs
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.h)

# C++ Linting. Produces warnings but doesn't prevent compiling
# https://github.com/google/styleguide/tree/gh-pages/cpplint
find_program(CPPLINT cpplint)
if(CPPLINT)
    add_custom_target(GoogleStyle ALL
        cpplint
        --output=eclipse
        --filter=-legal/copyright,-build/include,-build/header_guard
        --linelength=100
        ${srcs} || true)
endif()
