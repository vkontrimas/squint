cmake_minimum_required(VERSION 3.11)
project(squint LANGUAGES CXX)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shaders/test.frag.glsl.h"
  COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/shaders"
  # Append a NUL byte to the end of the shader file so resulting string is 
  # NUL terminated.
  COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/src/test.frag.glsl" "${CMAKE_CURRENT_BINARY_DIR}/shaders/test.frag.glsl"
  COMMAND truncate -s +1 "${CMAKE_CURRENT_BINARY_DIR}/shaders/test.frag.glsl"
  # Generate a header file
  COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && xxd -i "shaders/test.frag.glsl" > "shaders/test.frag.glsl.h"
)

add_executable(
  squint
  src/main.cpp
  "${CMAKE_CURRENT_BINARY_DIR}/shaders/test.frag.glsl.h"
)

target_link_libraries(squint PUBLIC X11 Xrender GL)
target_compile_features(squint PUBLIC cxx_std_20)
target_compile_options(
  squint PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)
target_include_directories(squint PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
