#pragma once
//由于链接器指定的是虚拟地址，因此如果要引用标号的实际地址，需要调用此函数
//虚拟地址为0x80000000
#define VirtualToPhysical(x) (x-0x80000000)
#define PGSIZE 4096 //页面大小
#define NPDENTRIES 1024 //页表项数目
//链接器将程序加载到的地址
#define KERNELBASE 0x80000000
//下面为页表项相关位参数
#define PCD  0x11
#define PWT  0x09
#define Writeable 0x02
#define Global 0x80
//下面是页目录表的地址->目录转换函数(10bits)
#define ToPageDirectory(addr) (addr>>22)//低22bit由下一级页表以及地址本身确定
//下面是页表的地址->页表转换函数(10bit)
#define ToPageTable(addr) (addr>>12)//低12位由地址本身确定

