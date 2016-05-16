#ifndef _PIT_H
#define _PIT_H

#include <lib/types.h>
#include <lib/lib.h>
#include <kernel/syscall.h>
#include <drivers/keyboard.h>

// PIT IRQ line
#define IRQ_PIT         0

// PIT command port
#define PIT_CMD_PORT    0x43

// PIT data port
#define PIT_DATA_PORT   0x40

// We are going to use PIT's square wave mode
#define PIT_SQ_MODE     0x36

// PIT's maximum supported divider
#define PIT_MAX_DIV     1193180

// Divider mask 1111 1111
#define DIVIDER_MASK    0xFF

void pit_init();
uint32_t calc_divider(uint32_t msec);
void pit_handle_interrupt(); 

#endif /* _PIT_H */
