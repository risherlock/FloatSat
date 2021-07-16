

echo "   __________________________________________________________ linux-x86 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh linux-x86 $i
done

echo "   __________________________________________________________ on-posix "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh on-posix $i
done

echo "   __________________________________________________________ linux-makecontext "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh linux-makecontext $i
done

echo "   __________________________________________________________ stm32f4 discovery"
for i in *.cpp
do
    # echo $i
    rodos-executable.sh discovery $i
done

echo "   __________________________________________________________ stm32f4 skith"
for i in *.cpp
do
    # echo $i
    rodos-executable.sh skith $i
done

echo "   __________________________________________________________ sf2 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh sf2 $i
done


echo "   __________________________________________________________ raspberrypi3 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh raspberrypi3 $i
done


echo "   __________________________________________________________ on-posix64 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh on-posix64 $i
done

