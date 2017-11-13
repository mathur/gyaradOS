#include <kernel/idt.h>

/*
 * divide_by_zero_except()
 *
 * Description: Prints the division by zero exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void divide_by_zero_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Divide by zero!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * debug_except()
 *
 * Description: Prints the debug exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void debug_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Debug!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * nmi_except()
 *
 * Description: Prints the NMI exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void nmi_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Non-maskable interrupt!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * breakpoint_except()
 *
 * Description: Prints the breakpoint exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void breakpoint_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Breakpoint!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * overflow_except()
 *
 * Description: Prints the overflow exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void overflow_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Into detected overflow!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * oob_except()
 *
 * Description: Prints the out of bounds exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void oob_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Out of bounds!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * opcode_except()
 *
 * Description: Prints the opcode exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void opcode_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Invalid opcode!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * no_coproc_except()
 *
 * Description: Prints the no coprocessor exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void no_coproc_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: No coprocessor!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * double_fault_except()
 *
 * Description: Prints the double fault exception when detected along with relevant error
 *              code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void double_fault_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Double fault!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * coproc_seg_overrun_except()
 *
 * Description: Prints the coprocessor segment overrun exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void coproc_seg_overrun_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Coprocessor segment overrun!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * tss_except()
 *
 * Description: Prints the tss exception when detected along with relevant error code data,
 *              and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void tss_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Bad TSS!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * segment_except()
 *
 * Description: Prints the segment not present exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void segment_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Segment not present!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * stack_except()
 *
 * Description: Prints the stack fault exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void stack_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Stack fault!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * general_protec_except()
 *
 * Description: Prints the general protection exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void general_protec_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: General protection fault!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * page_fault_except()
 *
 * Description: Prints the page fault exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void page_fault_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Page fault!\n");

    // get the address of where it's trying to access
    uint32_t fault_addr;
    asm volatile("movl %%cr2, %0": "=r" (fault_addr));
    printf("Tried to access memory at 0x%#x\n", fault_addr);

    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * unknown_interr_except()
 *
 * Description: Prints the unknown interrupt exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void unknown_interr_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Unknown interrupt!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * coproc_except()
 *
 * Description: Prints the coprocessor fault exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void coproc_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Coprocessor fault!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * align_except()
 *
 * Description: Prints the alignment check exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void align_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Alignment check!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * machine_chk_except()
 *
 * Description: Prints the machine check exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void machine_chk_except() {
    // block interrupts
    unsigned long flags;
    cli_and_save(flags);

    // print the interrupt type
    printf("\nEXCEPTION: Machine check!\n");
    halt(-2);

    // unblock interrupts (to be used when loop is removed)
    // Restore flags
    restore_flags(flags);
}

/*
 * general_interrupt()
 *
 * Description: Handles any other general interrupts that don't currently have specific handlers
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void general_interrupt() {
    unsigned long flags;
    cli_and_save(flags);
    printf("General interrupt recieved!\n");
    send_eoi(IRQ_SYSTEM_TIMER);
    // Restore flags
    restore_flags(flags);
}

/* init_idt()
 *
 * Description: Initializes the interrupt descriptor table with both exceptions and interrupts.
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void init_idt() {
    lidt(idt_desc_ptr);

    int i = 0;
    /// initialize the first 32 vectors as exceptions
    for(i = 0; i < NUM_EXCEPTIONS; i++) {
        // initialize idt properties here
        // present in memory
        idt[i].present = 1;

        // set device priveledge level to be kernel
        idt[i].dpl = 0;

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // set this as an exception
        idt[i].size = 1;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;
    }

    // set the correct idt entries based off x86 convention
    SET_IDT_ENTRY(idt[0], divide_by_zero_irq);
    SET_IDT_ENTRY(idt[1], debug_irq);
    SET_IDT_ENTRY(idt[2], nmi_irq);
    SET_IDT_ENTRY(idt[3], breakpoint_irq);
    SET_IDT_ENTRY(idt[4], overflow_irq);
    SET_IDT_ENTRY(idt[5], oob_irq);
    SET_IDT_ENTRY(idt[6], opcode_irq);
    SET_IDT_ENTRY(idt[7], no_coproc_irq);
    SET_IDT_ENTRY(idt[8], double_fault_irq);
    SET_IDT_ENTRY(idt[9], coproc_seg_overrun_irq);
    SET_IDT_ENTRY(idt[10], tss_irq);
    SET_IDT_ENTRY(idt[11], segment_irq);
    SET_IDT_ENTRY(idt[12], stack_irq);
    SET_IDT_ENTRY(idt[13], general_protec_irq);
    SET_IDT_ENTRY(idt[14], page_fault_irq);
    SET_IDT_ENTRY(idt[15], unknown_interr_irq);
    SET_IDT_ENTRY(idt[16], coproc_irq);
    SET_IDT_ENTRY(idt[17], align_irq);
    SET_IDT_ENTRY(idt[18], machine_chk_irq);

    // initialize the rest of the vectors as interrupts
    for(i = NUM_EXCEPTIONS; i < NUM_VEC; i++) {
        // initialize idt properties here
        // present in memory
        idt[i].present = 1;

        // set device priveledge level to be user if a syscall, or kernel otherwise
        if(i == SYSCALL_IDT) {
            idt[i].dpl = 3;
        } else {
            idt[i].dpl = 0;
        }

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // set this as an exception
        idt[i].size = 1;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        // configure device interrupts
        if(i == PIT_IDT) {
            SET_IDT_ENTRY(idt[PIT_IDT], pit_irq);
        } else if(i == KEYBOARD_IDT) {
            SET_IDT_ENTRY(idt[KEYBOARD_IDT], key_irq);
        } else if(i == RTC_IDT) {
            SET_IDT_ENTRY(idt[RTC_IDT], rtc_irq);
        } else if(i == MOUSE_IDT) {
            SET_IDT_ENTRY(idt[MOUSE_IDT], mouse_irq);
        } else if(i == SYSCALL_IDT) {
            SET_IDT_ENTRY(idt[SYSCALL_IDT], syscall_irq);
        } else {
            SET_IDT_ENTRY(idt[i], general_irq);
        }
    }
}
