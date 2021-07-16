cmake_minimum_required(VERSION 3.10)

add_compile_options( $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti> $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)

add_subdirectory(rodos-for-cmake)

file(GLOB_RECURSE source_files
  app01/*.cpp
  app02/*.cpp
  app03/*.cpp
  app04/*.cpp
  common/*.cpp
)

add_rodos_executable(big-application ${source_files})

target_include_directories(big-application PRIVATE common)
