ENTRY( _start )

/* Configure heap and stack space reserved for application.
 * Too low may cause standard library failed,
 * too high may cause SRAM overflow.
 * Collision may occur with heap when stack gets too deep.
 */
__stack_size = 2048;
__heap_size  = 2048;

MEMORY
{
    FLASH  (rx) : ORIGIN = 0x00000000, LENGTH = 192K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}


SECTIONS
{

    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(SORT_NONE(.vectors)))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        _stext = .;

        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)

        . = ALIGN(4);
        _etext = .;
    } >FLASH

    .preinit_array  :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    
    .fini_array :
    {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
      KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
      PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH
    
    .ctors :
    {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } >FLASH
    
    .dtors :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } >FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;

        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        *(.sdata .sdata.*)
        *(.srodata .srodata.*)
        *(.gnu.linkonce.s.*)

        . = ALIGN(4);
        _edata = .;
    } >RAM AT>FLASH

    . = ALIGN(16);
    PROVIDE( __global_pointer$ = . );

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;

        *(.sbss)
        *(.sbss.*)
        *(.gnu.linkonce.sb.*)
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)		
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;
    } >RAM

    .heap_stack :
    {
        . = ALIGN(8);
        _end = .;
        PROVIDE(end = . );

        . = . + __heap_size;
        . = ALIGN(8);
        PROVIDE(_heap_end = .);

        . = . + __stack_size;
        . = ALIGN(8);
    } >RAM


    /* Place initial SP to the end of SRAM */
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
    PROVIDE(_eusrstack = __stack_top);
}
