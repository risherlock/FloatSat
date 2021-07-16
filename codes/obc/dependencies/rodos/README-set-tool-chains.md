Setup Toolchains in Ubuntu
==========================

Only in case you do not have the gcc, gcc++, clang, cmake, and ARM crosscompiler tools, then follow these instructions.

First try the simple way following the instructions the shell will show you if the program is not found. If this do not help, then follow
these instructions here.



Install prerequisites 
---------------------

```
sudo apt update
sudo apt upgrade
sudo apt install build-essential xz-utils curl 
```

Optional: Add CLang toolchain repository. 
------------------------------

Note: In case you want the easy way, do not use clang, use g++ instead.
Edit the last lines in scripts/build-scripts  `*set-vars.sh` 

```
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="clang "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="clang++ "
```


Append the following lines to **/etc/apt/sources.list**

```
    deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main
    deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main
```


Install toolchain for CLang
-----------------------

```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add
sudo apt update
sudo apt install clang-7 lldb-7 lld-7 clang-tools-7 libclang1-7
sudo apt install clang-format-7 libc++-dev libc++abi-dev gdb
sudo apt install g++-multilib gcc-multilib
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang-7 100
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-7 100
```

Other tools always required for GCC and CLang
-----------

```
sudo apt install build-essential gcc-multilib g++-multilib
```


Manually install latest cmake 
-----------------------------

Ubuntu 20.04 and later ship a suitable CMake version:

```
sudo apt install cmake
```

To check your version run:

```
cmake --version
```

at the time of writing, version 3.13 was not yet part of the canonical
repository of earlier Ubuntu versions. Replace commands with latest version 

```
sudo apt remove cmake
wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2-Linux-x86_64.sh
chmod +x cmake-3.13.2-Linux-x86_64.sh
sudo ./cmake-3.13.2-Linux-x86_64.sh
sudo ln -s /opt/cmake-3.13.2-Linux-x86_64/bin/* /usr/local/bin
```

For ARM
=======

Install ARM cross compilation toolchain prerequisites
-----------------------------------------------------

```
sudo apt install libgmp3-dev libmpfr-dev libx11-6 libx11-dev 
sudo apt install texinfo flex bison libmpc-dev libncurses5
sudo apt install libncurses5-dbg libncurses5-dev libncursesw5
sudo apt install libncursesw5-dbg libncursesw5-dev zlibc
```

Install ARM cross compilation toolchain 
-------------------------------------------------

There are two options to install the ARM toolchain: Either through the package manager or manually.

Through apt: 

```
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi
```

Manually: replace link with latest Linux binary from https://developer.arm.com 


```
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
tar -xf gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
sudo mv gcc-arm-none-eabi-8-2018-q4-major /opt/gcc-arm
sudo ln -s /opt/gcc-arm/bin/* /usr/local/bin
```

For Smartfusion 2 (SF2)
======================

Setting up FlashPro 5 on newer Linux versions (to upload boot images to SF2)
---------------------------------------------------------------------------

The reason why the FlashPro 5 doesn't work out of the box with newer
Linux is that it advertises a USB device type that is registered
and handled by the FTDI serial I/O driver called ftdi_sio.
In order to make it work, we need to hook into the udev system
to ask it to unbind the ftdi_sio driver if the device type is "FlashPro5". 

```
/etc/udev/rules.d/70-microsemi.rules

# FlashPro5
SUBSYSTEM=="usb", ATTR{idVendor}=="1514", ATTR{idProduct}=="2008", MODE="0776", GROUP="plugdev", RUN+="unbind_ftdi.sh"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6001", MODE="0776", GROUP="plugdev", RUN+="unbind_ftdi.sh"
/lib/udev/unbind_ftdi.sh
  GNU nano 2.9.8                                                                                                               /lib/udev/unbind_ftdi.sh                                                                                                                Modified  
 
#!/usr/bin/env bash
 
# Find FTDI child USB device with interface FlashPro5
for DEVICE in /sys/bus/usb/drivers/ftdi_sio/*/
do
 
if grep -q FlashPro5 "${DEVICE}interface"; then
# Match found, unbind it from the FTDI driver
echo -n "$(basename $DEVICE)" > /sys/bus/usb/drivers/ftdi_sio/unbind
break
fi
done
```

Now set execution permissions for both 

```
/etc/udev/rules.d/70-microsemi.rules 
/lib/udev/unbind_ftdi.sh 
```

And set owner to root:root.

To be able to access the device,  add yourself to the plugdev user group (after adding yourself to the group, you need to restart your computer or run "su - $USER" to propagate the group change) :


sudo usermod -aG plugdev $USER


Reload the udev rules:

sudo udevadm control --reload-rules

Plug in the FlashPro 5 and dmesg should look like:

```
[135290.142184] usb 1-6.2: new high-speed USB device number 59 using xhci_hcd
[135290.250893] usb 1-6.2: New USB device found, idVendor=1514, idProduct=2008, bcdDevice= 8.00
[135290.250895] usb 1-6.2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[135290.250897] usb 1-6.2: Product: FlashPro5
[135290.250898] usb 1-6.2: Manufacturer: Microsemi
[135290.250899] usb 1-6.2: SerialNumber: 01QVQXR
[135290.257579] ftdi_sio 1-6.2:1.2: FTDI USB Serial Device converter detected
[135290.257606] usb 1-6.2: Detected FT4232H
[135290.257774] usb 1-6.2: FTDI USB Serial Device converter now attached to ttyUSB1
[135290.283949] ftdi_sio ttyUSB1: FTDI USB Serial Device converter now disconnected from ttyUSB1
[135290.283969] ftdi_sio 1-6.2:1.2: device disconnected
```

The important line is the last one which says "device disconnected",
which means the ftdi_sio driver has given up controlling the device.
OpenOCD should be working now

