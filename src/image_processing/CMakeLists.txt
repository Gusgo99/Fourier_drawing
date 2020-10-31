add_subdirectory("./strategies")

add_library(grid OBJECT "grid.cpp")
set(OBJECT_FILES "${OBJECT_FILES};grid")

set(OBJECT_FILES "${OBJECT_FILES}" PARENT_SCOPE)