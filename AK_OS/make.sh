rm -r build/
mkdir build
fasm src/boot.asm build/boot.bin
nasm -f elf src/kernel_entry.asm -o build/kernel_entry.o
#nasm -f elf src/low.asm -o build/low.o   

gcc -m16 -ffreestanding -nostdlib -c src/kernel.c -o build/kernel.o

ld   -m elf_i386 -T linker.ld -o build/kernel.elf build/kernel_entry.o build/kernel.o 


objcopy -O binary build/kernel.elf build/kernel.bin
cat build/boot.bin build/kernel.bin > build/os.img
python3 insert_blank_sectors.py
qemu-system-i386 -usb -device usb-mouse -fda build/os.img