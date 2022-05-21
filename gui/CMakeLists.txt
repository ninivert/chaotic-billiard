cmake_minimum_required(VERSION 3.5)

project(gui)

add_executable(${PROJECT_NAME} src/app.cpp)
# add_executable(${PROJECT_NAME} src/export_world_json.cpp)

target_link_libraries(${PROJECT_NAME}
	physics
	sfml-graphics sfml-window sfml-system GL
)

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/lib
)
