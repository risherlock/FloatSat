cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(rodos C CXX ASM)

if (COVERAGE)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "Coverage enabled, setting build type to Debug")
  endif ()
  include(cmake/CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif ()

if (NOT DEFINED port_dir)
  message(FATAL_ERROR "port not correct (CMAKE_TOOCHAIN_FILE, see readme)")
endif ()

#___________________________________________________________________
file(GLOB_RECURSE independent_sources
    src/independent/*.cpp
    support/support-libs/*.cpp)

add_library(rodos STATIC ${independent_sources})

file(GLOB port_sources
    src/${port_dir}/*.cpp
    src/${port_dir}/*.c
    src/${port_dir}/hal/*.cpp
    src/${port_dir}/*.S)
target_sources(rodos PRIVATE ${port_sources})

target_include_directories(rodos PUBLIC
    api
    api/hal
    src/${port_dir}
    src/${port_dir}/include
    support/support-libs)

target_compile_options(rodos PUBLIC -fno-exceptions -g -Wall -Wpedantic -Wextra -Wcast-qual -Wconversion
    -Wsign-conversion -Wfloat-conversion -Wdouble-promotion -Wnull-dereference -Wstrict-aliasing -Wno-long-long
    -Wno-cpp -O0)
target_compile_options(rodos PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)

option(ENABLE_RODOS_RTTI "Build RODOS with run-time type information" OFF)
if (NOT ENABLE_RODOS_RTTI)
  target_compile_options(rodos PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
endif ()


#___________________________________________________________________
if (is_port_baremetal)
  file(GLOB baremetal_generic_sources src/bare-metal-generic/*.cpp)
  target_sources(rodos PRIVATE ${baremetal_generic_sources})
  target_include_directories(rodos PUBLIC
      src/bare-metal-generic
      src/bare-metal-generic/ip/lwip/include)
endif ()


#___________________________________________________________________
#    optional extra files which are port dependent. See cmake/port/xxx.cmake
#___________________________________________________________________
if (sources_to_add)
  file(GLOB files ${sources_to_add})
  target_sources(rodos PRIVATE ${files})
endif ()

if (directories_to_include)
  target_include_directories(rodos PUBLIC ${directories_to_include})
endif ()

if (libraries_to_link)
  target_link_libraries(rodos PUBLIC ${libraries_to_link})
endif ()

if (EXECUTABLE)
  include(cmake/executable.cmake)
  add_subdirectory(tutorials)
endif ()


if (COVERAGE)
  setup_lcov_coverage(
      NAME coverage
      LCOV_ARGS "--no-external")
  message("Generate coverage report with 'make coverage_collect'")
  add_custom_command(TARGET coverage_collect POST_BUILD
      COMMAND sed -ie "s@SF:${CMAKE_CURRENT_SOURCE_DIR}/\\(.*\\)@SF:\\1@g"
      ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
      COMMENT "Changing paths in coverage.info to relative paths."
      VERBATIM
      )
endif ()
enable_testing()
add_subdirectory(test-suite)
