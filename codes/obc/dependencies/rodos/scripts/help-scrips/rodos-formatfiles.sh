#!/usr/bin/env bash

#astyle -O  -o  $*
clang-format   $* -i
\rm *.orig
