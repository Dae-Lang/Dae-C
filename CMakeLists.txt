cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME DAC)
project(${PROJECT_NAME})

set(SOURCES
    src/error.c
    src/main.c
    src/file.c
    src/string.c
    src/tokenizer.c
    src/vector.c)
set(HEADERS
    include/dac/bool.h
    include/dac/error.h
    include/dac/file.h
    include/dac/string.h
    include/dac/tokenizer.h
    include/dac/vector.h)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)