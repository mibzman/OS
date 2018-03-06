#!/bin/bash
rm bochsout.txt
rm kernel.o
rm bootload
rm floppya.img
rm kernel_asm.o
rm kernel
rm printer.out

#!/bin/bash -e
set -e

nasm bootload.asm
touch floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

# added in lab 4
dd if=map of=floppya.img bs=512 count=1 seek=256 conv=notrunc
dd if=config of=floppya.img bs=512 count=1 seek=258 conv=notrunc

bcc -ansi -c -o kernel.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259
dd if=msg of=floppya.img bs=512 count=1 seek=30 conv=notrunc

bochs -f osxterm.txt