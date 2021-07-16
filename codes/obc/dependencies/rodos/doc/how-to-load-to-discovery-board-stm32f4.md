
How to load compiled programs to discovery board
================================================


Compile your program using the rodos tools and target stm32f4 
If you are using the shell scripts you will get a **tst** file.
If your .elf file has another name than use the right name :)


Extract the binary data from tst and write for example tst.bin

```
   arm-none-eabi-objcopy -O binary tst tst.bin
```

Plug the discovery board to an USB port. You will get a file-manager window
(eg. Thunar). Now copy tst.bin to the new opened file-manager window.

To execute plug the USB out and in again. 
Else the programmer keeps the reset line active!




