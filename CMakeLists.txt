cmake_minimum_required(VERSION 3.5)

project(wav-library LANGUAGES C)

include_directories(source)

file(COPY assets/test-files DESTINATION ${CMAKE_BINARY_DIR})

add_library(c89-wav
    
    source/bin.h
    source/format.h
    source/tones.h
    
    source/bin.c
    source/format.c
    source/tones.c
)

add_executable(tests
    
    tests/binary-io.h
    tests/binary-io.c
    
    main.c
)

target_link_libraries(tests PRIVATE c89-wav)
