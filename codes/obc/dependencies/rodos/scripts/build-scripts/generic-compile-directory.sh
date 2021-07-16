#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# do not use it directly, it may be used from rodos-lib.sh and rodos-executable.sh
# or elese, source  first a xxx-set-vars.sh
# expected variables from xxx-set-vars.sh:
#   ${CPP_COMP}  ${CFLAGS} ${CPPFLAGS} ${INCLUDES} ${YOURFLAGS}
#

source $(dirname $0)/../quit-if-environment-is-wrong.sh
source $(dirname $0)/../quit-if-arch-is-undef.sh

#_________________________________ Set variables


# parameter 1: directory to compile, including all sources inside
# all compilations go to $RODOS_TO_LINK/new-generated-dir-name ($NAME_TO_USE)


mkdir $RODOS_BUILD   2> /dev/null   # just to be sure it exists
mkdir $RODOS_TO_LINK 2> /dev/null
              
NAME_TO_USE=o$(echo $1 | tr "/" "-")  # use "-" instead of "/"

#______________________________________ clear working dir

\rm -rf $RODOS_TO_LINK/$NAME_TO_USE
mkdir   $RODOS_TO_LINK/$NAME_TO_USE
\rm -f *.o

#_________________________________ Compile separately c++ and c

LIST=$(ls  $1/*.cc    $1/*.cpp   2> /dev/null)
if [ ! -z "$LIST" ] ; then
  ${CPP_COMP} ${CFLAGS} ${CPPFLAGS} ${INCLUDES} ${YOURFLAGS} -c $LIST \
      || exit 1
fi

LIST=$(ls  $1/*.c    $1/*.s      $1/*.S     2> /dev/null)
if [ ! -z "$LIST" ] ; then
  ${C_COMP} ${CFLAGS} ${INCLUDES} ${YOURFLAGS} -c $LIST || exit 1
fi

mv -i *.o $RODOS_TO_LINK/$NAME_TO_USE


