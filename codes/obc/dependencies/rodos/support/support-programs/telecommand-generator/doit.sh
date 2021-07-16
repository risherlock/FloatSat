
\rm -rf generated-examples
mkdir generated-examples

g++ -o telecommand-generator       telecommand-generator.cpp
g++ -o make-sh-links-for-cmds      make-sh-links-for-cmds.cpp
g++ -o telecommand-and-param-names telecommand-and-param-names.cpp

telecommand-generator         < telecommands.tc > generated-examples/telecommands.h
make-sh-links-for-cmds        < telecommands.tc > generated-examples/make-sh-links.sh
telecommand-and-param-names   < telecommands.tc > generated-examples/telecommand-and-param-names.h

echo "see all in telecommand-generator/generated-examples"
echo "    and remove it"


