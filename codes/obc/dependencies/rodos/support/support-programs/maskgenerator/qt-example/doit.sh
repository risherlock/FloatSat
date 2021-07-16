
ex qtdisplay/telemetryprinter.cpp << 'EOF'
/void TelemetryPrinter::printAllContent()
.,/}/d
w
q
EOF

g++ uiparser.cpp 
a.out standardTm. < qtdisplay//telemetryprinter.ui  >> qtdisplay/telemetryprinter.cpp
\rm -rf a.out

cat << EOF

Now  reaload and compile the project qtdisplay with qtcreator
execute it

execute alina-sw
execute egse/telemetryReceiverPrinter/standardtm-forwarder


EOF

