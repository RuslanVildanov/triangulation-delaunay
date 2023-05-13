cmake_minimum_required(VERSION 3.19)

set(LIBRARY_TARGET TriangulationLibrary)

add_library(${LIBRARY_TARGET} OBJECT)

include_directories(..)

target_add_headers(${LIBRARY_TARGET}
		coordinates_file_parser.h
		default_writer.h
		file_writer_interface.h
		utils.h
		vertex_3d.h)

target_add_sources(${LIBRARY_TARGET}
		coordinates_file_parser.cpp
		default_writer.cpp
		utils.cpp
		vertex_3d.cpp)
