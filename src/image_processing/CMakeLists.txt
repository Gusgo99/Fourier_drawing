add_library(DIP_tool OBJECT "DIP_tool.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_tool")

add_library(grid OBJECT "grid.cpp")
set(OBJECT_FILES "${OBJECT_FILES};grid")

set(OBJECT_FILES "${OBJECT_FILES}" PARENT_SCOPE)