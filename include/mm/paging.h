
#ifndef _PAGING_H
#define _PAGING_H
#include <lib/types.h>
#include <lib/lib.h>

// constants related to page table size
#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define PAGE_SIZE               4096
#define PAGE_SIZE_LARGE         0x400000

// constants relating to the start/end of memory addresses
#define KERNEL_PHYS_ADDR        0x400000
#define KERNEL_END_ADDR         0x800000
#define PROCESS_START_ADDR      0x08048000

// constants relating to the video address
#define VIDEO_PHYS_ADDR0        0XBC000
#define VIDEO_PHYS_ADDR1        0XDC000
#define VIDEO_PHYS_ADDR2        0XEC000

// page directory entry array
PDE_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_SIZE)));

// page table entry array
PTE_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

extern void init_paging();
extern uint32_t init_new_process(uint32_t process_num);
extern int switch_pd(uint8_t process_num, uint32_t prev_base);

#endif
