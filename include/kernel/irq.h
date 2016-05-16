
#ifndef IRQ_H
#define IRQ_H

/**
 * Exceptions
 */

// 0 - Division by zero irq
extern void divide_by_zero_irq();

// 1 - Debug irq
extern void debug_irq();

// 2 - Non maskable interrupt
extern void nmi_irq();

// 3 - Breakpoint irq
extern void breakpoint_irq();

// 4 - 'Into detected overflow'
extern void overflow_irq();

// 5 - Out of bounds irq
extern void oob_irq();

// 6 - Invalid opcode irq
extern void opcode_irq();

// 7 - No coprocessor irq
extern void no_coproc_irq();

// 8 - Double fault (pushes an error code)
extern void double_fault_irq();

// 9 - Coprocessor segment overrun
extern void coproc_seg_overrun_irq();

// 10 - Bad TSS (pushes an error code)
extern void tss_irq();

// 11 - Segment not present (pushes an error code)
extern void segment_irq();

// 12 - Stack fault (pushes an error code)
extern void stack_irq();

// 13 - General protection fault (pushes an error code)
extern void general_protec_irq();

// 14 - Page fault (pushes an error code)
extern void page_fault_irq();

// 15 - Unknown interrupt irq
extern void unknown_interr_irq();

// 16 - Coprocessor fault
extern void coproc_irq();

// 17 - Alignment check irq
extern void align_irq();

// 18 - Machine check irq
extern void machine_chk_irq();

/**
 * Interrupts
 */

// handler for pit interrupts
extern void pit_irq();

// handler for keyboard interrupts
extern void key_irq();

// handler for RTC interrupts
extern void rtc_irq();

// handler for mouse interrupts
extern void mouse_irq();

extern void sb_irq();

// handler for general interrupts
extern void general_irq();

// handler for syscall interrupts
extern void syscall_irq();

#endif /* IRQ_H*/
