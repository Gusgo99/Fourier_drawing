add_library(kiss_fft OBJECT "kiss_fft.c")
set(OBJECT_FILES "${OBJECT_FILES};kiss_fft")

set(OBJECT_FILES "${OBJECT_FILES}" PARENT_SCOPE)