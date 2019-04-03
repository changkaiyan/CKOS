#include"asm.h"
#include<Memory.h>

//链接器应该确保程序加载到物理地址0x100000地址处，在虚拟地址中将被映射到0x80000000以上
//这是一个链接器欺骗技术,在内核中不可以使用显示的地址，必须通过符号表加宏函数转换
.text//正文段
.globl start//CPU-BSP进入段
//cr3有高速缓存器
.code32
start:
    //清空0地址，作为页表项
    xor %eax,%eax
    movl %eax,(%eax)
    //打开4M分页扩展
    movl %cr4,%eax
    orl $(CR4_PSE),%eax
    movl %eax,%cr4
    //设置页目录cr3
    movl $(VirtualToPhysical(PageDirectory)),%eax
    movl %eax,%cr3
    //打开分页开关
    movl %cr0,%eax
    orl $(CR0_PG|CR0_WP),%eax
    movl %eax,%cr0
    //第二次初始化栈，扩大可控制的栈空间从0x7e00向下
    movl $(STACK), %esp
    //跳入kernel进行初始化操作,同时地址映射切换到内核虚拟地址0x800100000
    movl $Initial,%eax
    jmp *%eax



