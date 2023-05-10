cmake_minimum_required(VERSION 3.19)
set(APP_TARGET DelaunayTriangulation)
project(${APP_TARGET})

set(CMAKE_CXX_STANDARD 17)

function(target_add_headers TARGET_NAME)
    foreach(file ${ARGN})
        target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    endforeach()
endfunction()

function(target_add_sources TARGET_NAME)
    foreach(file ${ARGN})
        target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    endforeach()
endfunction()

add_executable(${APP_TARGET} main.cpp)

target_add_sources(${APP_TARGET}
        coordinates_file_parser.cpp
        vertex_3d.cpp)

target_add_headers(${APP_TARGET}
        coordinates_file_parser.h
        vertex_3d.h)

add_subdirectory(delaunay)
add_subdirectory(test)

target_link_libraries(${APP_TARGET} LINK_PRIVATE DelaunayLibrary)
