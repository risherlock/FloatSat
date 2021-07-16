macro(add_rodos_executable)
  add_executable(${ARGN})
  target_link_libraries(${ARGV0} PUBLIC rodos)
endmacro()
