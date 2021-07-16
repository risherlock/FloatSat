\rm -rf generated d
mkdir generated
#mkdir generated/doc


echo -e "\n#pragma once\n" > generated/ccsds-headers.h

for i in header-definitions/*.bf
do
  from=$i
  to=`basename $i .bf`
  bitFieldsSerializer < $from > generated/$to.h
  echo -e "#include \"$to.h\"" >> generated/ccsds-headers.h
done

echo >> generated/ccsds-headers.h

