SECTIONS
{
    .text :
    {
         *(.text);
    }
    .data : 
	{ 
		*(.data);	
		*(.gdt32);
		*(.gdt64);
		*(.idt)
	}
    .bss : 
	{ 
		*(.bss) 
	}
}