#!/usr/bin/env bash
if [ ! -z $RODOS_DIR ]; then  # it is ok!
  return
fi

cat << EOT

  *****************************************************
  *** please call (only once) from RODOS root directory
  ***         source rodosenvs
  *** else I can not do any thing!
  ****************************************************

EOT

exit

