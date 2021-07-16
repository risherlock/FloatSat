
Test Suite
==========

This directory is a test suite for almost all functions of RODOS.

Each function is tested in a .cpp file in core-test, middleware-test etc.
The test programs were executed once and their results were written in the
directories expected-outputs. We check by hand the correct execution of each program.
We consider the results in expected-outputs are right.

Using CMake it is possible to generate diffs (`generate_diffs` target)
and collect only non-empty diff files (`test-report` target) in a test-report.

Some results are not deterministic and will differ, for example

* Preemption tests
* Asynchronous reading and writing in comm buffers and fifos
* Time reports (resolution of nanoseconds), eg. errorLog
* The end of some executions, because each execution is just
  terminated after 20 seconds


