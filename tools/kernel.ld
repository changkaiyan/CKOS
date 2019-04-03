/*这个脚本用来链接内核程序，不用于链接启动程序
这个链接脚本的单行注释也必须是这样子*/
ENTRY(start)
SECTIONS
{
		/* 这是分页开启后映射的值 */
    . = 0x80100000;
    .text :AT(0x100000) /* elf文件的挂载点*/
		{
		*(.text .stub .text.* .gnu.linkonce.t.*)
	}
  PROVIDE(etext = .);	

	.rodata : 
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}
	/* 由于分页机制的存在，因此需要对齐 */
  . = ALIGN(0x1000);
   .data : 
	{
		*(.data)
	}

	PROVIDE(edata = .);

	.bss : {
		*(.bss)
	}

	PROVIDE(end = .);

	/DISCARD/ : {
		*(.*)
        
	}

}