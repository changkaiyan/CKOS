#pragma once
#include"types.h"
#define ELF_MAGIC 0x464C457F
#define PROGRAMLOAD 1//类型为LOAD的段，可以加载
//elf头表

typedef struct Elf32Header{
    DWORD magic;  // 魔术数字
    CHAR elf[12];
    WORD fileType;//目标文件类型
    WORD targetMachine;//目标文件体系结构类型
    DWORD versionOfProgram;//目标文件版本
    DWORD entryOfProgram;//程序入口点的虚拟地址
    DWORD programHeaderTableOffset;//程序头表的偏移地址
    DWORD sectionHeaderTableOffset;//节表的偏移地址
    DWORD flagsOfProcessor;//处理器相关标识
    WORD sizeOfThisTableHeader;//elf头的大小
    WORD programHerderEntrySize;//程序头入口大小
    WORD programHeaderNumber;//程序头入口数目
    WORD sectionHeaderEntrySize;//节表大小
    WORD sectionHeaderNumber;//节表头数目
    WORD sectionHeaderStringIndex;//节名表在节头表中的索引
}Elf32Header;

//程序头表,描述与程序执行直接相关的目标文件结构信息。
//用来在文件中定位各个段的映像。同时包含其他一些用来为程序创建映像所必须的信息
typedef struct ProgramHeader {
  DWORD sectionType;//描述段的类型
  DWORD sectionOffsetFromFile;//段偏移
  DWORD virtualAddress;//段第一个字节将被放到内存的虚拟地址
  DWORD physicalAddress;//仅用于物理地址相关的系统
  DWORD sectionInFileSize;//段在文件映像中所占的字节数，可以为0
  DWORD sectionInMemorySize;//段在内存映像中所占的字节数，可以为0
  DWORD sectionFlags;//段相关标示
  DWORD sectionAlign;//段在文件和内存中如何对齐
}ProgramHeader;
