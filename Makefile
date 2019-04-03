# 注意，HALS文件夹中如果编译到内核中只能包含.S和 .h文件，不能包含.c文件
KERNELS=SystemTable.c main.c
HALS=loader.S
SRCS=$(foreach i,$(KERNELS),./KERNEL/src/$(i))
SRCS+=$(foreach j,$(HALS),./HAL/x86/$(j))
OBJS=$(foreach t,$(KERNELS),$(subst c,o,$(t)))
OBJS+=$(foreach k,$(HALS),$(subst S,o,$(k)))
CFLAGS = -fno-builtin -O2 -Wall -MD -ggdb 
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

all:CKOS.img 
	rm *.o *.d

CKOS.img: bootblock kernel
	dd if=/dev/zero of=./build/CKOS.img count=10000
	dd if=./build/bootblock of=./build/CKOS.img conv=notrunc
	dd if=./build/kernel of=./build/CKOS.img seek=1 conv=notrunc

bootblock: ./HAL/x86/entry.S ./HAL/x86/bootmain.c
	$(CC) $(CFLAGS) -m32 -O -nostdinc -I./HAL/x86 -I./KERNEL/include -c ./HAL/x86/bootmain.c -o ./build/bootmain.o
	$(CC) $(CFLAGS)  -m32 -nostdinc -I. -c ./HAL/x86/entry.S -o ./build/entry.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -m  elf_i386 -o ./build/bootblock.o ./build/entry.o ./build/bootmain.o
	objcopy -S -O binary -j .text ./build/bootblock.o ./build/bootblock1
	python3.6 ./tools/mbrcode.py ./build/bootblock1 ./build/bootblock

kernel: $(SRCS)
	$(CC) $(CFLAGS) -m32 -fno-pic -O -nostdinc -I./HAL/x86 -I./KERNEL/include -c $(SRCS)
	$(LD)   -m elf_i386  -T  ./tools/kernel.ld   $(OBJS)  -o ./build/kernel 
	
