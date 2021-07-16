/*
sources:
symbols need same naming as in other linker script files (see stm32_flash.ld)
https://github.com/ahessling/STM32F4Template/blob/master/linker/stm32f407vgt.ld
https://github.com/charleskorn/stm32f4-project-template/blob/master/lib/stm32f4xx/src/stm32f407vg.ld
https://embedds.com/programming-stm32-discovery-using-gnu-tools-linker-script/
and examples on storing persistent data in stm32f407 flash
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of 128K RAM on AHB bus*/

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x1000;    /* required amount of heap: 0x1000 = 4k */
_Min_Stack_Size = 0x400;    /* required amount of stack: 0x400 = 1k */

MEMORY
{
  FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K - 128K
  DATA (xrw)      : ORIGIN = 0x080E0000, LENGTH = 128K
  RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
  CCMRAM (rw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);

    /* startaddress of the image */
    _image_address = .;

    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
	*(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH


   .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH
  
  /* used by the startup to initialize data */
  _sidata = .;

  /* Flash memory reserved for user data */
  .user_data :
  {
    . = ALIGN(4);
      *(.user_data)
    . = ALIGN(4);
  } >DATA

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : AT ( _sidata )
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM

  /* Core-Coupled RAM data section */
  . = ALIGN(4);
  .bss_ccram (NOLOAD) : 
  {    
    _sbss_ccram = .;         /* define a global symbol at bss start */
    __bss_start___ccram = _sbss_ccram;
    *(.bss_ccram)
   
    . = ALIGN(4);
    _ebss_ccram = .;         /* define a global symbol at bss end */
    __bss_end___ccram = _ebss_ccram;
  } >CCMRAM


  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM
  

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    PROVIDE ( __heap_start__ = .);
    . = . + _Min_Heap_Size;
    PROVIDE ( __heap_end__ = .);
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
  } >RAM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
