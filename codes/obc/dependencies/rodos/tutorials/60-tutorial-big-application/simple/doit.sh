
if [ -z $MISSION_ROOT ]; then
  cat << EOT
  *____________________________________________________ ERROR
  * Environment variables are not correct set.
  * please execute first in the rodos directory
  *     source setenvs.sh 
  * and then in the mission root directory 
  *     source mission-setenvs.sh
  *____________________________________________________
EOT
  exit
fi

if [ $# -ne 1 ]
then
    echo
    echo "doit.sh: expected one parameter: name of architecture"
    echo
    exit 1
fi

set -e

#       to be able to call generic-compile-directory.sh directly we have to set the target platform variables
#       We set here the variables for "Linux make context"! Change this for other platforms
#       bash is intelligent, it searches for xxx-set-vars.sh search in $PATH and finds it in $RODOS_BUILD_SCRIPTS

source ${RODOS_VARS_DIR}/$1.sh

export INCLUDES=${INCLUDES}" -I ${MISSION_ROOT}/common"

\rm -rf $RODOS_TO_LINK

generic-compile-directory.sh common
generic-compile-directory.sh app01
generic-compile-directory.sh app02
generic-compile-directory.sh app03
generic-compile-directory.sh app04
generic-compile-directory.sh app04/subdir1/
generic-compile-directory.sh app04/subdir2
generic-compile-directory.sh app04/subdir3

MYDIR=$PWD
cd $RODOS_TO_LINK
${CPP_COMP}  ${CFLAGS} ${CPPFLAGS} -o tst */*.o  ${LINKFLAGS}
mv tst $MYDIR


echo "done! please execute tst"
