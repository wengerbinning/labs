


world:

boot.o: boot.asm
	nasm -f elf boot.asm -o boot.o

main.o: main.c
	gcc -m16 -ffreestanding -fno-pic -fno-builtin -nostdlib -c main.c -o main.o

boot.bin: boot.o main.o
	ld -m elf_i386 -T linker.ld -o boot.bin boot.o main.o

world: boot.bin

qemu: boot.bin
	qemu-system-i386 -drive format=raw,file=boot.bin


.PHONY: world