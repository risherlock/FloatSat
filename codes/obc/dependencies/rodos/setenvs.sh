#!/usr/bin/env bash
scriptName=$_

#_________________________________________Check it has to be sourced!
# Source check should work on most shells
# See this: https://stackoverflow.com/questions/2683279/how-to-detect-if-a-script-is-being-sourced
sourced=0
if [ -n "$ZSH_EVAL_CONTEXT" ]; then
  case $ZSH_EVAL_CONTEXT in *:file) sourced=1;; esac
elif [ -n "$KSH_VERSION" ]; then
  [ "$(cd $(dirname -- $0) && pwd -P)/$(basename -- $0)" != "$(cd $(dirname -- ${.sh.file}) && pwd -P)/$(basename -- ${.sh.file})" ] && sourced=1
elif [ -n "$BASH_VERSION" ]; then
  (return 0 2>/dev/null) && sourced=1
else # All other shells: examine $0 for known shell binary filenames
  # Detects `sh` and `dash`; add additional shell filenames as needed.
  case ${0##*/} in sh|dash) sourced=1;; esac
fi

if [ $sourced -eq 0 ]
then
   echo "_____________________________________________________ ERROR"
   echo "ERROR: Please call this script as source, and not as subshell"
   echo "like this:"
   echo "      %source $scriptName"
   echo "___________________________________________________________"
   exit
fi
#_____________________________________________________ Set Directories

export RODOS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export RODOS_ROOT=$RODOS_DIR  # sorry! Some times so, some times so!

export RODOS_BUILD_SCRIPTS=${RODOS_DIR}/scripts/build-scripts
export RODOS_HELP_SCRIPTS=${RODOS_DIR}/scripts/help-scrips
export RODOS_VARS_DIR=${RODOS_BUILD_SCRIPTS}/set-vars

export RODOS_BUILD=${RODOS_DIR}/build       # place the build dir any where you want
export RODOS_TO_LINK=${RODOS_BUILD}/to_link
export RODOS_LIBS=${RODOS_BUILD}/libs

export RODOS_API=${RODOS_DIR}/api
export RODOS_SRC=${RODOS_DIR}/src
export RODOS_SUPPORT_LIBS=${RODOS_DIR}/support/support-libs
export RODOS_SUPPORT_PROGS=${RODOS_DIR}/support/support-programs
export RODOS_DOC=${RODOS_DIR}/doc

export PATH=${PATH}:${RODOS_BUILD_SCRIPTS}:${RODOS_HELP_SCRIPTS}:${RODOS_DIR}/support/tools:

alias cdrodos="cd ${RODOS_DIR}"

#_____________________________________________________ Set Default compiler flags

# you may extend or substitute this flags, but keep the names.

export INCLUDES=" -I ${RODOS_API} -I ${RODOS_SUPPORT_LIBS} ${APP_INCLUDES} "

#export CFLAGS=" -g -Wall -pedantic -Wno-long-long -Wno-cpp  -fno-exceptions -mfpmath=387 -O0 "
export CFLAGS=" -g -Wall -Wpedantic -Wextra -Wcast-qual -Wconversion -Wsign-conversion -Wfloat-conversion -Wdouble-promotion -Wnull-dereference -Wstrict-aliasing -Wno-long-long -Wno-cpp -fno-exceptions -O0 "
export CPPFLAGS=" -fno-rtti -fno-exceptions  -std=c++14"

#_____________________________________________________ extras, nice to have but not required

cat << EOT2
______________________________________________________
   Now I know where RODOS is, you may use the rodos shell scripts
   from anywhere, and come back here using "cdrodos"
....
   if you are using git I recommend to use: 
   source ${RODOS_HELP_SCRIPTS}/set-git-aware-promt.sh
______________________________________________________
EOT2

complete -W "efr32fg1p gecko linux-x86 linux-makecontext on-posix on-posix64 on-posixmac sf2 discovery skith" rodos-lib.sh
#complete -W "efr32fg1p gecko linux-x86 linux-makecontext on-posix on-posix64 on-posixmac sf2 discovery skith" rodos-executable.sh
