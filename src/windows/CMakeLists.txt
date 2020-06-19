add_library(DIP_window OBJECT "DIP_window.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_window")

add_library(draw_window OBJECT "draw_window.cpp")
set(OBJECT_FILES "${OBJECT_FILES};draw_window")

add_library(edit_window OBJECT "edit_window.cpp")
set(OBJECT_FILES "${OBJECT_FILES};edit_window")

set(OBJECT_FILES "${OBJECT_FILES}" PARENT_SCOPE)