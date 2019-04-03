#include"asm.h"

.code16
.globl start
start:
    jmp begin
    nop
begin:
    cli //关中断
#ifdef RELEASE
    //开启图形图像模式
    movw $0x4f02,%ax
    movw $0x4118,%bx
    int $0x10
    //检测图形图像模式是否开启成功
    cmp $0x004f,%ax
    jz right
    hlt
#endif
right: 
   
    //开启a20地址线-快速门方法
    movw $0x92,%dx
    inb %dx,%al
    orb $0x02,%al
    outb %al,%dx
    //加载GDTR
    lgdt globledescripter
    //进入保护模式
    movl %cr0, %eax
    orl $CR0_PE,%eax
    movl %eax,%cr0
    //清空描述符高速缓存器并跳入32位代码段，加载代码段选择子
    //每个描述符占8字节
    ljmp $(SEG_KCODE<<3),$start32

.code32
start32:
    //加载数据段选择子
    movw $(SEG_KDATA<<3),%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%ss
    //保存指令位置，栈指针->0x7c00
    movl $start,%esp
    call BootMain
    //如果bootmain出现异常返回到下面停机
    hlt
.p2align 2  //四字节32bits对齐
gdt://平坦模式段选择子
    SEG_NULLASM //空选择子
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   //4GB可读可执行段
    SEG_ASM(STA_W, 0x0, 0xffffffff) //4GB数据与堆栈段
globledescripter:
    .word (globledescripter-gdt-1)     //全局描述符表界线
    .long gdt   //全局描述符表的位置


