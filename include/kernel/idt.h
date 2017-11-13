#ifndef _IDT_H
#define _IDT_H

#include <drivers/rtc.h>
#include <drivers/keyboard.h>
#include <drivers/i8259.h>
#include <drivers/mouse.h>
#include <kernel/irq.h>
#include <lib/lib.h>
#include <boot/x86_desc.h>

#define NUM_EXCEPTIONS 0x20

#define PIT_IDT        0x20
#define KEYBOARD_IDT   0x21
#define RTC_IDT        0x28
#define MOUSE_IDT	   0x2C
#define SYSCALL_IDT    0x80

// 0 - Division by zero exception
void divide_by_zero_except();

// 1 - Debug exception
void debug_except();

// 2 - Non maskable interrupt
void nmi_except();

// 3 - Breakpoint exception
void breakpoint_except();

// 4 - 'Into detected overflow'
void overflow_except();

// 5 - Out of bounds exception
void oob_except();

// 6 - Invalid opcode exception
void opcode_except();

// 7 - No coprocessor exception
void no_coproc_except();

// 8 - Double fault (pushes an error code)
void double_fault_except();

// 9 - Coprocessor segment overrun
void coproc_seg_overrun_except();

// 10 - Bad TSS (pushes an error code)
void tss_except();

// 11 - Segment not present (pushes an error code)
void segment_except();

// 12 - Stack fault (pushes an error code)
void stack_except();

// 13 - General protection fault (pushes an error code)
void general_protec_except();

// 14 - Page fault (pushes an error code)
void page_fault_except();

// 15 - Unknown interrupt exception
void unknown_interr_except();

// 16 - Coprocessor fault
void coproc_except();

// 17 - Alignment check exception
void align_except();

// 18 - Machine check exception
void machine_chk_except();

// generic interrupt
void generic_interrupt();

// initialize the idt
void init_idt();

#endif /* _IDT_H */
