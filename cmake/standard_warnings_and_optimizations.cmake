function(set_standard_c_flags TARGET VISIBILITY)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        if(CMAKE_C_COMPILER_ID STREQUAL MSVC)
            target_compile_options(${TARGET} ${VISIBILITY} /O0 /W4 /WX)
        else()
            target_compile_options(
                ${TARGET}
                ${VISIBILITY}
                -Og
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wcast-align
                -Wunused
            )
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL Release)
        if(CMAKE_C_COMPILER_ID STREQUAL MSVC)
            target_compile_options(${TARGET} ${VISIBILITY} /O2 /W0 /WX)
        else()
            target_compile_options(${TARGET} ${VISIBILITY} -O3)
        endif()
    endif()
endfunction()

function(set_standard_cxx_flags TARGET VISIBILITY)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        if(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
            target_compile_options(${TARGET} ${VISIBILITY} /O0 /W4 /WX)
        else()
            target_compile_options(
                ${TARGET}
                ${VISIBILITY}
                -Og
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wcast-align
                -Wunused
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Woverloaded-virtual
            )
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL Release)
        if(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
            target_compile_options(${TARGET} ${VISIBILITY} /O2 /W0 /WX)
        else()
            target_compile_options(${TARGET} ${VISIBILITY} -O3)
        endif()
    endif()
endfunction()

function(set_standard_linker_flags TARGET VISIBILITY)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        if(C_COMPILER_ID STREQUAL MSVC)
        else()
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL Release)
        if(CMAKE_C_COMPILER_ID STREQUAL MSVC)
        else()
            target_link_options(${TARGET} PRIVATE -mwindows)
        endif()
    endif()
endfunction()