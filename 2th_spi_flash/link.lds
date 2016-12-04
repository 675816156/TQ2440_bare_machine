ENTRY(_start)
SECTIONS
{
	. = 0;
	__start__ = .;
	. = ALIGN(4);
	.text :
	{
	        start.o (.text)
	        *(.text)
	}
	. = ALIGN(4);
	.data : { *(.data) }
	. = ALIGN(4);
	.rodata : { *(.rodata) }
	. = ALIGN(4);
	__end__ = .;
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = . ;
}
