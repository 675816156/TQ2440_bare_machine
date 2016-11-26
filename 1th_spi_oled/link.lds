SECTIONS {
	.text : {
		start.o (.text*)
		*(.text*)
	}
	.data : {
		*(.data*)
	}
	.bss : {
		*(.bss*)
	}
}
