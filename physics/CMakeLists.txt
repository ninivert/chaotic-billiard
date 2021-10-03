cmake_minimum_required(VERSION 3.10)

project(physics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME}
	src/collider.cpp
	src/curve.cpp
	src/globals.cpp
	src/logger.cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)
