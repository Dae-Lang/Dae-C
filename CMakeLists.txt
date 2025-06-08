cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME dac)

project(${PROJECT_NAME}
    LANGUAGES
    C)

if(DEFINED ENV{INSTALL})
  set(INSTALL TRUE)
else()
  set(INSTALL FALSE)
endif()

if(INSTALL)
  include(GNUInstallDirs)
endif()


set(SOURCES
    src/error.c
    src/main.c
    src/file.c
    src/interpreter.c
    src/hashmap.c
    src/lexer.c
    src/node.c
    src/parser.c
    src/string.c
    src/vector.c)
set(HEADERS
    include/dac/bool.h
    include/dac/error.h
    include/dac/file.h
    include/dac/hashmap.h
    include/dac/inline.h
    include/dac/interpreter.h
    include/dac/lexer.h
    include/dac/node.h
    include/dac/parser.h
    include/dac/string.h
    include/dac/vector.h)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(INSTALL)
  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h")
endif()