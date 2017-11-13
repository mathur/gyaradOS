#define ASM 1

// exceptions
.global divide_by_zero_irq
.global debug_irq
.global nmi_irq
.global breakpoint_irq
.global overflow_irq
.global oob_irq
.global opcode_irq
.global no_coproc_irq
.global double_fault_irq
.global coproc_seg_overrun_irq
.global tss_irq
.global segment_irq
.global stack_irq
.global general_protec_irq
.global page_fault_irq
.global unknown_interr_irq
.global coproc_irq
.global align_irq
.global machine_chk_irq

// interrupts
.global pit_irq
.global key_irq
.global rtc_irq
.global mouse_irq
.global general_irq
.global syscall_irq
.global sb_irq

/**
 * Exceptions
 */

/*
 * divide_by_zero_irq()
 *
 * Description: Assembly wrapper for the exception divide by zero
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
divide_by_zero_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call divide_by_zero_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * debug_irq()
 *
 * Description: Assembly wrapper for the exception debug
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
debug_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call debug_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * nmi_irq()
 *
 * Description: Assembly wrapper for the exception non maskable interrupt
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
nmi_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call nmi_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * breakpoint_irq()
 *
 * Description: Assembly wrapper for the exception breakpoint
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
breakpoint_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call breakpoint_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * overflow_irq()
 *
 * Description: Assembly wrapper for the exception overflow
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
overflow_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call overflow_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * oob_irq()
 *
 * Description: Assembly wrapper for the exception out of bounds
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
oob_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call oob_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * opcode_irq()
 *
 * Description: Assembly wrapper for the exception opcode invalid
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
opcode_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call opcode_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * no_coproc_irq()
 *
 * Description: Assembly wrapper for the exception no coprocessor
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
no_coproc_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call no_coproc_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * double_fault_irq()
 *
 * Description: Assembly wrapper for the exception double fault
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
double_fault_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call double_fault_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * coproc_seg_overrun_irq()
 *
 * Description: Assembly wrapper for the exception segmentation overrun
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
coproc_seg_overrun_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call coproc_seg_overrun_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * tss_irq()
 *
 * Description: Assembly wrapper for the exception tss failure
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
tss_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call tss_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * segment_irq()
 *
 * Description: Assembly wrapper for the exception segmentation failure
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
segment_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call segment_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * stack_irq()
 *
 * Description: Assembly wrapper for the exception stack failure
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
stack_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call stack_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * general_protec_irq()
 *
 * Description: Assembly wrapper for the exception general protection fault
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
general_protec_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call general_protec_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * page_fault_irq()
 *
 * Description: Assembly wrapper for the exception page fault
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
page_fault_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call page_fault_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * unknown_interr_irq()
 *
 * Description: Assembly wrapper for the exception unknown interrupt
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
unknown_interr_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call unknown_interr_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * coproc_irq()
 *
 * Description: Assembly wrapper for the exception coprocessor fail
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
coproc_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call coproc_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * align_irq()
 *
 * Description: align failure
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
align_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call align_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * machine_chk_irq()
 *
 * Description: Assembly wrapper for the exception machine check
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
machine_chk_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call machine_chk_except

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/**
 * Interrupts
 */

/*
 * pit_irq()
 *
 * Description: Calls the PIT handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
pit_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call pit_handle_interrupt

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * key_irq()
 *
 * Description: Calls the keyboard handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
key_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call handle_keypress

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * rtc_irq()
 *
 * Description: Calls the rtc handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
rtc_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call rtc_handle_interrupt

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * mouse_irq()
 *
 * Description: Calls the mouse handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
mouse_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call mouse_handle_interrupt

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * general_irq()
 *
 * Description: Calls the general interrupt handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
general_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call general_interrupt

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/* 
 * syscall_jumptable
 * 
 * A jump table of the syscall functions to be used
 */
syscall_jumptable:
    .long divide_by_zero_except
    .long halt
    .long execute
    .long read
    .long write
    .long open
    .long close
    .long getargs
    .long vidmap
    .long set_handler
    .long sigreturn

/* 
 * syscall_irq
 * 
 * Description: main entrypoint for syscalls in the operating system
 * 
 * Input: Syscall number in eax
 * Output: Launches and returns from the appropriate syscall
 */
syscall_irq:
    cmpl $1, %eax
    jl syscall_invalid
    cmpl $10, %eax
    jg syscall_invalid

    // save other registers we need
    pushl %ebp
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi

    // push arguments in order
    pushl %edx
    pushl %ecx
    pushl %ebx

    sti

    // call the correct syscall
    call *syscall_jumptable(,%eax,4)

    // pop args
    addl $12,%esp

    // restore other registers we need back
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %ebp

    // jump to return
    jmp syscall_ret

// invalid syscall label
syscall_invalid:
    movl $-1, %eax
    jmp syscall_ret

// return from the syscall and iret back
syscall_ret:
    // return to interrupt handler
    iret
