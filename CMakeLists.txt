cmake_minimum_required(VERSION "3.17.0")

project("Fourier_drawing")

# Set language version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add directory for cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

find_package(wxWidgets REQUIRED xrc html xml core base)

include("${wxWidgets_USE_FILE}")

add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")

# Add include directories
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${wxWidgets_INCLUDE_DIRS}")

# Define compiler options for each build
if(CMAKE_BUILD_TYPE STREQUAL Release)
	add_link_options(-mwindows -flto)
else()
	add_definitions(-DDEBUG)
	add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wcast-align -Wunused)
	add_compile_options(-Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual)
endif()

file(GLOB_RECURSE RESOURCES "${PROJECT_SOURCE_DIR}/resources/*")
file(
	COPY
	"${PROJECT_SOURCE_DIR}/resources/layout.xrc"
	DESTINATION
	"${${CMAKE_PROJECT_NAME}_BINARY_DIR}/resources")

# Add files in subdirectories
set(OBJECT_FILES "")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

# Created the executable
add_executable(run "${PROJECT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(run PRIVATE ${wxWidgets_LIBRARIES} "${OBJECT_FILES}")