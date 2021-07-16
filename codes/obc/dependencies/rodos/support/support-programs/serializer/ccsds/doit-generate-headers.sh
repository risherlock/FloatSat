
if [ -z $RODOS_ROOT ]; then
source `dirname $0`/print_no_envs_warning
fi  

\rm -rf generated
mkdir generated

echo -e "\n#pragma once\n"       > generated/ccsds-headers.h
echo -e "#include <stdint.h>\n" >> generated/ccsds-headers.h

for i in header-definitions/*.bf
do
  from=$i
  to=`basename $i .bf`
  bitFieldsSerializer < $from > generated/$to.h
  echo -e "#include \"$to.h\"" >> generated/ccsds-headers.h
done

echo >> generated/ccsds-headers.h

