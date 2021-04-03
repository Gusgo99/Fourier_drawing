# Override kissfft standard configurations.
option(KISSFFT_STATIC "Build kissfft as static (ON) or shared library (OFF)" ON)
option(KISSFFT_TOOLS "Build kissfft command-line tools" OFF)
option(KISSFFT_TEST "Build kissfft tests" OFF)

add_subdirectory(kissfft)