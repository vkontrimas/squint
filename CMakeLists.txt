cmake_minimum_required(VERSION 3.11)
project(squint LANGUAGES CXX)

################################################################################
# COPY SHADERS                                                                 #
################################################################################
set(GENERATED_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated_include)
file(MAKE_DIRECTORY ${GENERATED_INCLUDE_PATH}/squint/glsl)

set(
  SHADERS
  glsl/test.frag
  glsl/test.vert
)

foreach(SHADER ${SHADERS})
  set(SOURCE_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/${SHADER}")
  set(SOURCE_OUTPUT_INTERMEDIATE_PATH "${GENERATED_INCLUDE_PATH}/squint/${SHADER}")
  set(SOURCE_OUTPUT_PATH "${GENERATED_INCLUDE_PATH}/squint/${SHADER}.h")
  add_custom_command(
    OUTPUT ${SOURCE_OUTPUT_PATH}
    # Append a NUL byte to the end of the shader file so resulting string is 
    # NUL terminated.
    COMMAND cp "${SOURCE_SHADER_PATH}" "${SOURCE_OUTPUT_INTERMEDIATE_PATH}"
    COMMAND truncate -s +1 "${SOURCE_OUTPUT_INTERMEDIATE_PATH}"
    # Generate a header file
    COMMAND cd "${GENERATED_INCLUDE_PATH}" && xxd -i "squint/${SHADER}" > "${SOURCE_OUTPUT_PATH}"
    COMMAND sed -i "s/unsigned char/const char/g" "${SOURCE_OUTPUT_PATH}"
    DEPENDS "${SOURCE_SHADER_PATH}"
  )
  list(APPEND GENERATED_SHADER_HEADERS "${SOURCE_OUTPUT_PATH}")
endforeach()

################################################################################
# BUILD EXECUTABLE                                                             #
################################################################################
add_executable(
  squint
  src/main.cpp
  src/gl.hpp
  src/gl.cpp
  src/x11/display.hpp
  src/x11/display.cpp
  src/x11/xhandle.hpp
  src/x11/xhandle.cpp
  src/x11/screenshot.hpp
  src/x11/screenshot.cpp
  src/x11/gl_context.hpp
  src/x11/gl_context.cpp
  ${GENERATED_SHADER_HEADERS}
)

target_link_libraries(squint PUBLIC X11 Xrender GL)
target_compile_features(squint PUBLIC cxx_std_20)
target_compile_options(
  squint PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)
target_include_directories(squint PUBLIC "${GENERATED_INCLUDE_PATH}")
