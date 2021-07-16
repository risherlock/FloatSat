# All tests are assumed to be contained in a single file.
file(GLOB test_files
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  core-slow/*.cpp
  core-fast/*.cpp
  middleware-tests/*.cpp)

# These tests are known to currently fail / hang, so do not use them.
#
# TODO: Make these tests useable!
#
list(REMOVE_ITEM test_files
  "core-fast/make-errors-asserts.cpp"
  "core-fast/timepoints-errlog.cpp")
if (COVERAGE)
  list(REMOVE_ITEM test_files
    "core-fast/timeevents.cpp"
    "core-slow/preemptiontest-and-ceiler.cpp"
    "core-slow/priotest.cpp"
    "core-fast/gets-async.cpp"
    "core-fast/getsnowiat.cpp"
    "core-fast/semaphore-scope-protector.cpp")
  set(coverage_prepare_dep "coverage_prepare")
endif()


foreach(test_file ${test_files})
  # filename: Used to get name of expected output
  # test_name: replaced_problematic_symbols
  get_filename_component(filename ${test_file} NAME_WE)
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_name ${test_file})

  # Compile rodos test application
  add_executable("${test_name}-bin" EXCLUDE_FROM_ALL ${test_file})
  target_include_directories("${test_name}-bin"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries("${test_name}-bin" PUBLIC rodos)

  # Command to run rodos test application, generating output files
  # including diff; this also generates coverage information if
  # coverage reporting is enabled.
  add_custom_command(
    OUTPUT
    "${test_name}-bin.output"
    "${test_name}-bin.output_trimmed"
    "${test_name}-bin.expected_trimmed"
    "${test_name}-bin.diff"
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/test-runner.sh
    $<TARGET_FILE:${test_name}-bin>
    ${CMAKE_CURRENT_SOURCE_DIR}/expected-outputs/${filename}.txt
    DEPENDS
    ${test_name}-bin test-runner.sh
    ${coverage_prepare_dep}) # dependency: coverage_prepare <-- run of test
  list(APPEND diff_files "${test_name}-bin.diff")

  add_custom_target(${test_name}-clean
    COMMAND
    ${CMAKE_COMMAND} -E remove -f
    "${test_name}-bin.output"
    "${test_name}-bin.output_trimmed"
    "${test_name}-bin.expected_trimmed"
    "${test_name}-bin.diff")

  add_custom_target(${test_name}-run DEPENDS "${test_name}-bin.diff")
  if (COVERAGE)
    # test-clean <--- coverage_reset
    #
    # build of test <-- run of test <--- coverage_collect
    # build of test <-- coverage_prepare
    #
    # NOTE: Additional dependency coverage_prepare <-- run of test
    # required, established above!
    add_dependencies(coverage_collect ${test_name}-run)
    add_dependencies(coverage_prepare ${test_name}-bin)
    add_dependencies(coverage_reset ${test_name}-clean)
  endif()
endforeach()

add_custom_target(generate-diffs DEPENDS "${diff_files}")

add_custom_command(
  OUTPUT test-report.txt
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test-collector.sh "${diff_files}"
  DEPENDS generate-diffs test-collector.sh
  VERBATIM)
add_custom_target(test-report
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test-collector.sh "${diff_files}"
  VERBATIM
  DEPENDS generate-diffs test-collector.sh)
