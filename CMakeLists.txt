cmake_minimum_required(VERSION 3.5)

project(wav-library LANGUAGES C)

include_directories(source)

set(C89_WAV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source" PARENT_SCOPE)

file(COPY assets/test-files DESTINATION ${CMAKE_BINARY_DIR})

add_library(c89-wav
    source/bin.h
    source/format.h
    source/bin.c
    source/format.c
)

add_executable(tests
    tests/binary-io.h
    tests/wav-format.h
    tests/functionality.h
    tests/binary-io.c
    tests/wav-format.c
    tests/functionality.c
    main.c
)

set_property(TARGET c89-wav PROPERTY C_STANDARD 90)
set_property(TARGET tests   PROPERTY C_STANDARD 90)

target_link_libraries(tests PRIVATE c89-wav)
