/* Entry Point */
ENTRY(ResetHandler)

__stack_size = 8128;

PROVIDE( _stack_size = __stack_size );

MEMORY
{
	RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 192K
	CCMRAM (rw)      : ORIGIN = 0x10000000, LENGTH = 64K
	FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 2048K
}

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
 
        KEEP(*(.init))
        KEEP(*(.fini))
 
        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
 
        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
 
        *(.rodata*)
 
        KEEP(*(.eh_frame*))
    } > FLASH
 
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
 
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
 
    __etext = .;
       
    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)
 
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
 
        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
 
 
        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
 
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;
 
    } > RAM
 
    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM
	

	PROVIDE( _end = __bss_end__);
	PROVIDE( end = . );
	
    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
    {
    	. = ALIGN(8);
        PROVIDE( _heap_end = . );    
        PROVIDE(_susrstack = . );
        . = . + __stack_size;
        PROVIDE( __StackTop = .);
        . = ALIGN(8);
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
