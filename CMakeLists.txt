cmake_minimum_required(VERSION 3.8)

set(APP_NAME "ArgParse")

set (CMAKE_CXX_STANDARD 20)

project(${APP_NAME})

FILE(GLOB_RECURSE headers "*.h")
FILE(GLOB_RECURSE sources "*.cpp")

add_executable(${APP_NAME} ${sources} ${headers})


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}  FILES ${sources} ${headers})
