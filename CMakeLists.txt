cmake_minimum_required(VERSION 3.10)

project(ffmpeg-lib-info CXX)

add_executable(ffmpeg-lib-info ffmpeg-lib-info.cpp)

target_compile_options(ffmpeg-lib-info PRIVATE /W4 /WX)

set_property(TARGET ffmpeg-lib-info PROPERTY CXX_STANDARD 17)
