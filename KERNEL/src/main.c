#include<types.h>
#include<asm.h>
#include<Memory.h>
//临时页目录表提供页目录项
//没有临时页表，因为我们利用清空的0地址让偏移正好到那个地方loader.S
__attribute__((__aligned__(PGSIZE)))
ADDRESS PageDirectory[NPDENTRIES] =  
{
  //在ip指针跳转之前临时的页表项，[0, 4M) => [0, 4M)
  [0] = 0x0 | PTE_P | PTE_W | PTE_PS,
  //在ip指针跳转到Initial之后的页表项，[0x80000000,0x80000000+4M) => [0, 4M)
  [ToPageDirectory(KERNELBASE)] = 0x0 | PTE_P | PTE_W | PTE_PS
} ;

VOID Initial() 
{

 asm("hlt");
}
