#!/usr/bin/env bash

echo "::: Running $1 against $2"
set -e
# Kill test executable (with SIGKILL) after 8 seconds.  Necessary
# because sometimes tests might deadlock.
if timeout -s 9 8 $1 > "$1.output"
then
    true
else
    echo ":: KILLED BY TIMEOUT" >> "$1.output"
fi

# Remove header / intro from output and expected output
linecount=`wc -l < "$2"`
pattern="-----------------------------------------------------"
grep -h -B 0 -A $linecount -e $pattern $1.output > $1.output_trimmed || true
grep -h -B 0 -A $linecount -e $pattern $2 > $1.expected_trimmed || true

diff -ruh $1.output_trimmed $1.expected_trimmed > $1.diff || true
