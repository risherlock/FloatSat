#!/usr/bin/env bash
if [ ! -z $ARCH ]; then  # it is ok!
  return
fi

cat << EOT

  *****************************************************
  *** generic-compile-directory.sh may not be called directelly
  *** they may be called only from other scripts
  *** which sent the environment for the current target
  ****************************************************

EOT

exit

