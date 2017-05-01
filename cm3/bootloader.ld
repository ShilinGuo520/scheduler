

MEMORY
{
  RAM(rwx) 		: ORIGIN = 0x20000C00, LENGTH = 61K
  FLASH (rx)    : ORIGIN = 0x08005000, LENGTH = 492K
}

SECTIONS
{
	.text :
	{
		. = ALIGN(0x4);
		KEEP(start_up.o(.text))
		KEEP(*(.text*))
		KEEP(*(.rodata))
		KEEP(*(.rodata*))
		. = ALIGN(4);
		_etext_end = . ;
		_sidata = _etext_end ;
	} > FLASH

	.data  : AT (_sidata)
	{	
		. = ALIGN(0x4);
		_sdata = . ;
		KEEP(*(.data))
		KEEP(*(.data*))
		. = ALIGN(0x4);
		_edata = . ;
	} > RAM

	.bss : 
	{	
		. = ALIGN(0x4);
		_sbss = . ;
		__bss_start__ = . ; 
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(0x4);
		_ebss = . ;
		__bss_end__ = . ;
		_exit = . ;
	} > RAM

	.stack :
	{
	} > RAM
	stack_top = 0x20005000;
}
