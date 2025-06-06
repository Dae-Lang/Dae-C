cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME DAC)
project(${PROJECT_NAME})

set(SOURCES src/main.c)
set(HEADERS) # nothing for now

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)