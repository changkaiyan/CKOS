/**
 * 这是一段bootsec代码，与entry.S代码一并存放于硬盘前512byte处
 */
//确保内核链接文件中的加载物理地址为0x100000(即1M空间以上)
#include"asm.h"
 #include"elf.h"
 #include"types.h"
#include "instruction.h"
#include<Memory.h>
#define SECTION_SIZE 512
#define PAGE_SIZE 4096
VOID
BOOTSEC
ReadSections(
    CHAR *physicalAddress,
    DWORD count,
    DWORD offset
    );

VOID 
BOOTSEC
BootMain(
    VOID
    )
{
    //ELF可执行文件（Kernel)头部1
    Elf32Header *elfHeader;
    //ELF可执行文件（Kernel)头部2
    ProgramHeader *programHeader,  *endOfProgramHeader;
    CHAR *loadPhysicalAddress;
    //Kernel入口点
    VOID (*Entry)(VOID);
    //ELF文件头部定位标识，单独存放可以减少512bit负担
    elfHeader = (Elf32Header*)0x10000;
    //从磁盘中读取一个页面4kb大小的
    ReadSections((CHAR*)elfHeader, PAGE_SIZE, 0);
    if(elfHeader->magic != ELF_MAGIC)
        return;
    //通过elf64Table定位programHeaderTable
    programHeader = (ProgramHeader*)((CHAR*)elfHeader + elfHeader->programHeaderTableOffset);
    endOfProgramHeader = programHeader + elfHeader->programHeaderNumber;
    //加载每一个程序段到elf文件定位的物理地址
    do
    {
        if(programHeader->sectionType==PROGRAMLOAD)
        {
            loadPhysicalAddress = VirtualToPhysical((void*)programHeader->virtualAddress);
            ReadSections(loadPhysicalAddress, programHeader->sectionInFileSize, programHeader->sectionOffsetFromFile);
            //如果文件中包含了.bss未初始化数据段，则sectionInMemorySize会大于sectionInFileSize，此时需要清零数据段以符合c标准
            if(programHeader->sectionInMemorySize > programHeader->sectionInFileSize)
                StringToStringByte(loadPhysicalAddress + programHeader->sectionInFileSize, 0 ,programHeader->sectionInMemorySize -programHeader->sectionInFileSize);            
        }
        programHeader++;
    }while(programHeader < endOfProgramHeader);
    Entry =   (void(*)(void))(VirtualToPhysical(elfHeader->entryOfProgram));
    Entry();
}

VOID 
BOOTSEC
WaitDiskNotBusy(
    VOID
    )
{
    //读取硬盘控制器判断是否繁忙
    while((InByte(0x1F7)&0xC0) != 0x40);
}

VOID
BOOTSEC
ReadOneSectionFromDisk(
    VOID *destination, 
    DWORD offset
    )
{
    WaitDiskNotBusy();
    OutByte(0x1F2, 1);//放入要读写的扇区数量
    OutByte(0x1F3, offset);//要读写的扇区号码
    OutByte(0x1F4, offset >> 8);//读写柱面低8位
    OutByte(0x1F5, offset >> 16);//读写柱面的高8位
    OutByte(0x1F6, (offset >> 24) | 0xE0);
    OutByte(0x1F7, 0x20);//向命令端口发送尝试读取命令
    WaitDiskNotBusy();
    InStringLong(0x1F0, destination, SECTION_SIZE/4);//传送回512字节数据
}

VOID
BOOTSEC
ReadSections(
    CHAR *physicalAddress,
    DWORD count,//指读取的字节数目
    DWORD offset//指磁盘的偏移量
    )
{
    //这里将地址向下去整可以读出整数个512字节且刚好读完到end
    CHAR* endOfPhysicalAddress;
    endOfPhysicalAddress = physicalAddress +  count;
    physicalAddress -= offset % SECTION_SIZE;
    offset = (offset / SECTION_SIZE) + 1;
    do
    {
        ReadOneSectionFromDisk(physicalAddress, offset);
        physicalAddress +=SECTION_SIZE;
        offset++;
    }while(physicalAddress < endOfPhysicalAddress);
}