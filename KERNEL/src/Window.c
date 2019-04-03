/*本子模块用于图形化界面的显示0xb8000-0xb8fff 显示器共25行,每行80个字符*/
#include<Window.h>
#include<types.h>
#include<SystemTable.h>
#include<instruction.h>
BOOL
CKERNEL
OutToscreen(char character, BYTE mode, int x, int y)
{
    if(x < 0 || y < 0 || x >= XMAX || y >= YMAX)
        return FALSE;
    char* point = ((char*)(0xb8000)) + 2 * (y * XMAX + x);
    *point = character;
    point++;
    *point = mode;
    return TRUE;
}
/*标题栏存储式管理,以空间换时间*/
static inline VOID
CKERNEL
CleanPublicArea()
{
    char black[2*(XMAX*(YMAX - 2))]={0};
    StringToStringByte(SCREENBASEADDR + 2 * XMAX, black, 2*(XMAX*(YMAX - 2)));
}
/*下面的清屏函数清除全部区域并初始化系统边框(上下区域)*/
VOID
CKERNEL
ClearScreen(SystemTable* sys)
{
    int i, j;
    CleanPublicArea();
    StringToStringByte(SCREENBASEADDR + 2 * (XMAX*(YMAX - 1)), sys->windowTable->bottomBar, XMAX);
    StringToStringByte(SCREENBASEADDR, sys->windowTable->topBar, XMAX);
}