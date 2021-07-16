
#_________________________________________Check it has to be sourced!
scriptName=$_
if [ $scriptName == $0 ]
then
   echo "_____________________________________________________ ERROR"
   echo "ERROR: Pleas call this script as source, and not as subshell"
   echo "like this:"
   echo "      %source $scriptName"
   echo "___________________________________________________________"
   exit
fi

if [ -z $RODOS_ROOT ]; then
   echo "_____________________________________________________ ERROR"
   echo "ERROR: RODOS_ROOT not set"
   echo "       please first go to rodos and execute source setenvs.sh"
   echo "___________________________________________________________"
   return
fi  

#_________________________________________ Set veraibles

export MISSION_ROOT=$(pwd)
export PATH=${PATH}:${MISSION_ROOT}/scripts:
alias cdmission="cd $MISSION_ROOT"

