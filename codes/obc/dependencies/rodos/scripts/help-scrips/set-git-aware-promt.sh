#!/usr/bin/env bash
  
scriptName=$_

if [ $scriptName == $0 ]
then
 echo "You shall source this file, it is not intended to be executed as sub-shell."
 exit 
fi

if [ -z $RODOS_DIR ]; then  
  echo "please call first in the RODOS root directory %source setenvs.sh"
  return
fi

#________________________________ git-aware-prompt

export GITAWAREPROMPT=${RODOS_HELP_SCRIPTS}/git-aware-prompt

source "${GITAWAREPROMPT}/main.sh"

export PS1="\W\[$txtcyn\]\$git_branch\[$txtred\]\$git_dirty\[$txtrst\]\$ "

