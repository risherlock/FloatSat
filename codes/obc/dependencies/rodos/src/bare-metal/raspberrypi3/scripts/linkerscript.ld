OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* not needed by RODOS just for newlib */
_estack = 0x20020000;  
_Min_Heap_Size = 0x1000;
_Min_Stack_Size = 0x400;

MEMORY {
  ram (rwx)  : ORIGIN = 0x00008000, LENGTH = 511M
}

SECTIONS {
  .text : { 
    KEEP (*(.vectors .vectors.*))
    *(.text.startup)
    *(.text)
    *(.text*)
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.jcr)
    *(.vfp11_veneer)
    *(.gnu.linkonce.t*)
  } > ram

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > ram

  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > ram

  .rodata : {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
  } > ram

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE (__init_array_end = .);
  } > ram
  
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } > ram
  
  .data : {
    . = ALIGN(4);
    *(.data)
    *(.data*)
    *(.gnu.linkonce.d*)
  } > ram

  .bss : {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)

    /* The MMU page table must be 16KB aligned (see TTBR0 register) */
    . = ALIGN(0x4000);
    _mmu_level1_table_start_ = .;
    /* Only the first level page table is used.
       This gives us a total size of 4096 * 4B = 16 KB */
    . = . + 0x4000;
    _mmu_level1_table_end_ = .;

    __bss_end__ = .;
  } > ram

  PROVIDE ( end = . );
  PROVIDE ( _end = . );
  PROVIDE ( __end__ = . );

  ._user_heap_stack : {
    . = ALIGN(4);
    PROVIDE ( __heap_start__ = .);
    . = . + _Min_Heap_Size;
    PROVIDE ( __heap_end__ = .);
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
  } > ram

  /DISCARD/ : {
    *(.comment)
    *(.note*)
    *(.eh_frame)
  }
}

