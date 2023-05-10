cmake_minimum_required(VERSION 3.19)

set(LIBRARY_TARGET DelaunayLibrary)

add_library(${LIBRARY_TARGET} OBJECT)

target_add_headers(${LIBRARY_TARGET}
		constants.h
		delaunay_factory.h
		delaunay_service.h
		triangle.h
		triangulation_cache.h
		utils.h
		vertex_2d.h
		vertex_relation.h)

target_add_sources(${LIBRARY_TARGET}
		delaunay_factory.cpp
		delaunay_service.cpp
		triangle.cpp
		triangulation_cache.cpp
		utils.cpp)
