cmake_minimum_required(VERSION "3.17.0")

project("Fourier_drawing")

# Set language version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include("${PROJECT_SOURCE_DIR}/cmake/standard_warnings_and_optimizations.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/copy_resources_to_binary_directory.cmake")

find_package(wxWidgets REQUIRED xrc html xml core base)

add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")

if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE STREQUAL Debug))
	add_definitions("-D_GLIBCXX_DEBUG")
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${wxWidgets_INCLUDE_DIRS}")

file(GLOB_RECURSE CXX_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE C_FILES "${PROJECT_SOURCE_DIR}/src/*.c")

add_library(CXX_OBJECT_FILES OBJECT "${CXX_FILES}")
add_library(C_OBJECT_FILES OBJECT "${C_FILES}")

set_standard_c_flags(C_OBJECT_FILES PUBLIC)
set_standard_cxx_flags(CXX_OBJECT_FILES PUBLIC)

target_precompile_headers(
	CXX_OBJECT_FILES
	PRIVATE
	"${PROJECT_SOURCE_DIR}/include/wxWidgets_headers.hpp"
)

if((CMAKE_BUILD_TYPE STREQUAL Debug) OR (NOT WIN32))
	add_executable(run "")
else()
	add_executable(run WIN32 "")
endif()

set_standard_linker_flags(run PRIVATE)

target_link_libraries(run PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(run PRIVATE CXX_OBJECT_FILES)
target_link_libraries(run PRIVATE C_OBJECT_FILES)

copy_resources_to_binary_directory()