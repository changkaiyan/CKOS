/*
* 这里是处理器的基本属性与内存基本数据转换的存放处
*/

//32位模式空描述符
#define SEG_NULLASM                                             \
        .word 0, 0;                                             \
        .byte 0, 0, 0, 0

//32位模式描述符构造器
#define SEG_ASM(type,base,lim)                                  \
        .word ((lim) & 0xffff), ((base) & 0xffff);      \
        .byte (((base) >> 16) & 0xff), (0x90 | (type)),         \
                (0xC0 | (((lim) >> 16) & 0xf)), (((base) >> 24) & 0xff)


#define STA_X     0x8       // 32位可执行段
#define STA_W     0x2       // 32位可写非可执行段
#define STA_R     0x2       // 32位可读可执行段

#define CR0_PE          0x00000001      // 保护模式使能
#define CR0_PG          0x80000000      // 开启分页
#define CR0_WP          0x00010000      // 开启内存写保护，不允许向只读页面写入数据
#define CR4_PSE         0x00000010      //页面大小扩展
//下面位用于进入x64扩展模式
#define CR4_PAE         0x00000020      //开启页管理机制物理地址寻址扩展
#define CR4_PGE         0x00000080      //开启全局页表功能
// 段选择子.
#define SEG_KCODE 1  // 内核代码段位于描述符表[1]
#define SEG_KDATA 2  // 内核数据和堆栈公用位于[2]

//内核的物理地址加载位置1M，32位保护模式
#define KERNBASE 0x100000
//定义内核栈的初始地址
#define STACK 0x7e00
//页表相关
#define PTE_P           0x001   // 物理页存在标识
#define PTE_W           0x002   // 是否可写
#define PTE_PS          0x080   // 页面大小

