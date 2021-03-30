cmake_minimum_required(VERSION "3.17.0")

project("Fourier_drawing")

# Set language version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include("${PROJECT_SOURCE_DIR}/cmake/standard_warnings_and_optimizations.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/copy_resources_to_binary_directory.cmake")

find_package(wxWidgets REQUIRED xrc html xml core base)

add_subdirectory("extlibs")

add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")

if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE STREQUAL Debug))
	add_definitions("-D_GLIBCXX_DEBUG")
endif()

file(GLOB_RECURSE CXX_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

if((CMAKE_BUILD_TYPE STREQUAL Debug) OR (NOT WIN32))
	add_executable(run "${CXX_FILES}")
else()
	add_executable(run WIN32 "${CXX_FILES}")
endif()

target_precompile_headers(
	run
	PRIVATE
	"${PROJECT_SOURCE_DIR}/include/wxWidgets_headers.hpp"
)

set_standard_cxx_flags(run PUBLIC)

set_standard_linker_flags(run PRIVATE)

target_link_libraries(run PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(run PRIVATE kissfft)

target_include_directories(run PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(run PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(run PRIVATE "${wxWidgets_INCLUDE_DIRS}")

copy_resources_to_binary_directory()