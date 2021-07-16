#!/usr/bin/env bash

VARS_FILE=$RODOS_VARS_DIR/${1}.sh

if [ ! -e $VARS_FILE ]; then
  cat << EOT

  *****************************************************
  *** ERROR
  ***   expected one parameter: name of the target achitecture 
  ***   Architectur $1 not defined
  ***   file $VARS_FILE not found
  ***   see files in $RODOS_VARS_DIR
  ****************************************************

EOT
   exit
fi


