#!/bin/bash
git rm --cached `git ls-files -i --exclude-from=.gitignore`

#!/bin/bash -e
set -e

# build tools used
gcc loadFile.c -o loadFile

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

# ./loadFile spc02
# ./loadFile kitty1
./loadFile kitty2

bochs -f osxterm.txt

dd if=floppy.img of=config bs=512 skip=258 count=1 conv=notrunc
