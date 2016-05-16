#define ASM 1
#include <boot/x86_desc.h>

.global jmp_usr_exec

 # [jmp_usr_exec jumps to user space to begin execution of a program]
 # Inputs: None
 # Outputs: None

jmp_usr_exec:
    # clear interrupts
    cli

    # clear some registers for use
    xorl %ebx, %ebx
    xorl %edi, %edi
    xorl %esi, %esi

    # get the address of the entrypoint and store it into ebx
    movl 4(%esp), %ebx

    # move the segments into registers for use
    movl $USER_CS, %esi
    movl $USER_DS, %edi

    # move the selector to the appropriate registers
    movw %di, %ds
    movw %di, %es
    movw %di, %fs
    movw %di, %gs

    # push the data segment selector
    pushl %edi

    # set stack to bottom of 4mb page for this program
    pushl $0x83FFFFC

    # enable interrupts
    sti

    # push the flags correctly preparing for iret
    pushfl

    # push the remaining parameters for iret
    pushl %esi
    pushl %ebx

    # iret to the correct context
    iret
