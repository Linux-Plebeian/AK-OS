#ifndef FS_H
#define FS_H

#include "int.h"
#include "arg.h"

#define MAX_FILES 10
#define FILENAME_LEN 11
#define FILE_SIZE 512
#define ENTRY_SIZE sizeof(FileEntry)
#define SECTOR_SIZE 512
#define FILE_LOCATION_START 22





void read_sector(uint8_t drive, uint8_t head, uint8_t track, uint8_t sector, void* buffer) {
    uint16_t seg = ((uintptr_t)buffer) >> 4;
    uint16_t off = ((uintptr_t)buffer) & 0xF;
    

    
    asm volatile (
        "push %%es\n\t"
        "mov %[seg], %%ax\n\t"
        "mov %%ax, %%es\n\t"
        "mov $0x02, %%ah\n\t"     // BIOS read sector int
        "mov $1, %%al\n\t"        // number of sectors = 1
        "mov %[track], %%ch\n\t"
        "mov %[sector], %%cl\n\t"
        "mov %[head], %%dh\n\t"
        "mov %[drive], %%dl\n\t"
        "mov %[off], %%bx\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        :
        : [seg] "rm" (seg),
          [off] "rm" (off),
          [track] "rm" (track),
          [sector] "rm" (sector),
          [head] "rm" (head),
          [drive] "rm" (drive)
        : "ax", "bx", "cx", "dx", "memory"
    );
}
void write_sector(uint8_t drive, uint8_t head, uint8_t track, uint8_t sector, const void* buffer) {
    uint16_t offset = (uint16_t)buffer & 0x0F;
    uint16_t segment = (uint16_t)((uint32_t)buffer >> 4);

    asm volatile (
        "push %%es\n\t"
        "mov %[seg], %%ax\n\t"
        "mov %%ax, %%es\n\t"
        "mov $0x03, %%ah\n\t"     // BIOS write sector int
        "mov $1, %%al\n\t"        // number of sectors = 1
        "mov %[track], %%ch\n\t"
        "mov %[sector], %%cl\n\t"
        "mov %[head], %%dh\n\t"
        "mov %[drive], %%dl\n\t"
        "mov %[off], %%bx\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        :
        : [seg] "rm" (segment),
          [off] "rm" (offset),
          [track] "rm" (track),
          [sector] "rm" (sector),
          [head] "rm" (head),
          [drive] "rm" (drive)
        : "ax", "bx", "cx", "dx", "memory"
    );

}

#endif