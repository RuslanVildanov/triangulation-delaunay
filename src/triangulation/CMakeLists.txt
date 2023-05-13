cmake_minimum_required(VERSION 3.19)

set(LIBRARY_TARGET TriangulationLibrary)

add_library(${LIBRARY_TARGET} OBJECT)

include_directories(..)

target_add_headers(${LIBRARY_TARGET}
		coordinates_file_parser.h
		file_writer_interface.h
		vertex_3d.h)

target_add_sources(${LIBRARY_TARGET}
		coordinates_file_parser.cpp
		vertex_3d.cpp)
