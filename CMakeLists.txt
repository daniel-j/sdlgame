cmake_minimum_required(VERSION 2.0)

project(sdltutorial)

add_executable(${PROJECT_NAME} "main.cpp")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
	                ${SDL2_IMAGE_INCLUDE_DIR}
					${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
	                  ${SDL2_LIBRARY}
					  ${SDL2_IMAGE_LIBRARIES}
					  ${SDL2_TTF_LIBRARIES})
