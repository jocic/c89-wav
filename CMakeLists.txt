cmake_minimum_required(VERSION 3.5)

project(wav-library LANGUAGES C)

add_library(bin bin/bin.h bin/bin.c)
add_library(wav wav/format.h wav/format.c wav/tones.h wav/tones.c)

add_executable(wav-library main.c helper.h helper.c)

target_link_libraries(wav-library PRIVATE bin wav)

target_link_libraries(wav PRIVATE bin)
