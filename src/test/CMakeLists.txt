cmake_minimum_required(VERSION 3.19)

set(TEST_TARGET DelaunayTest)

add_executable(${TEST_TARGET} main.cpp)

target_include_directories(${TEST_TARGET} PRIVATE "..")
target_link_libraries(${TEST_TARGET} LINK_PRIVATE DelaunayLibrary)

target_add_headers(${TEST_TARGET}
		delaunay_service_test.h
		test.h
		triangle_test.h
		triangulation_cache_test.h
		utils_test.h)

target_add_sources(${TEST_TARGET}
		delaunay_service_test.cpp
		triangle_test.cpp
		triangulation_cache_test.cpp
		utils_test.cpp)
