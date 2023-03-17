cmake_minimum_required(VERSION 3.11)
project(squint LANGUAGES CXX)

add_executable(
  squint
  src/main.cpp
)
target_link_libraries(squint PUBLIC X11)
target_compile_features(squint PUBLIC cxx_std_20)
target_compile_options(
  squint PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)
