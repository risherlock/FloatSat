


echo "mask generators for VT100 Terminals"
cd maskgenerator
doit.sh
cd ..

echo "serializers"
cd serializer/generator
doit
cd ../
cd ccsds
doit-serializers.sh
cd ../..

cd telecommand-generator/
doit.sh
cd ..

