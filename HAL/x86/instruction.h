//本文件是x86指令集中的指令在c语言中的接口
#pragma once
#include"types.h"

static inline CHAR
X86
InByte(
    WORD port
    )
{
  CHAR data;
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  return data;
}

static inline VOID
X86
InStringLong(
   int port, 
    VOID *address, 
    int count)
{
  asm volatile("cld; rep insl" :
               "=D" (address), "=c" (count) :
               "d" (port), "0" (address), "1" (count) :
               "memory", "cc");
}

static inline VOID
X86
OutByte(
    WORD port, 
    CHAR data
    )
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline VOID
X86
OutWord(
    WORD port, 
    WORD data
    )
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline VOID
X86
OutStringLong(
    DWORD port, 
    const VOID *address, 
    DWORD count
    )
{
  asm volatile("cld; rep outsl" :
               "=S" (address), "=c" (count) :
               "d" (port), "0" (address), "1" (count) :
               "cc");
}

static inline VOID
X86
StringToStringByte(
    VOID *address, 
    int data, 
    int count
    )
{
  asm volatile("cld; rep stosb" :
               "=D" (address), "=c" (count) :
               "0" (address), "1" (count), "a" (data) :
               "memory", "cc");
}

static inline VOID
X86
StringToStringLong(
    VOID *address,
    int data, 
    int count
    )
{
  asm volatile("cld; rep stosl" :
               "=D" (address), "=c" (count) :
               "0" (address), "1" (count), "a" (data) :
               "memory", "cc");
}

struct SegmentDescriptor;

static inline VOID
X86
LoadGlobalDescriptorTable(
    struct SegmentDescriptor *p, 
    DWORD size
    )
{
  volatile WORD pd[3];

  pd[0] = size-1;
  pd[1] = (DWORD)p;
  pd[2] = (DWORD)p >> 16;

  asm volatile("lgdt (%0)" : : "r" (pd));
}

struct gatedesc;

static inline void
lidt(struct gatedesc *p, int size)
{
  volatile WORD pd[3];

  pd[0] = size-1;
  pd[1] = (DWORD)p;
  pd[2] = (DWORD)p >> 16;

  asm volatile("lidt (%0)" : : "r" (pd));
}

static inline void
ltr(WORD sel)
{
  asm volatile("ltr %0" : : "r" (sel));
}

static inline DWORD
readeflags(void)
{
  DWORD eflags;
  asm volatile("pushfl; popl %0" : "=r" (eflags));
  return eflags;
}

static inline void
loadgs(WORD v)
{
  asm volatile("movw %0, %%gs" : : "r" (v));
}

static inline void
cli(void)
{
  asm volatile("cli");
}

static inline void
sti(void)
{
  asm volatile("sti");
}

static inline DWORD
xchg(volatile DWORD *addr, DWORD newval)
{
  DWORD result;
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

static inline DWORD
rcr2(void)
{
  DWORD val;
  asm volatile("movl %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(DWORD val)
{
  asm volatile("movl %0,%%cr3" : : "r" (val));
}