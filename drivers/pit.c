
#include <drivers/pit.h>

/**
 * [pit_init Initializes the PIT]
 * Inputs: none
 * Outputs: none
 */
void pit_init() {
    // set up the pit to 30hz
    uint32_t divider = calc_divider(30);

    outb(PIT_SQ_MODE, PIT_CMD_PORT);
    outb((divider & DIVIDER_MASK), PIT_DATA_PORT);
    outb((divider >> 8), PIT_DATA_PORT);
}

/**
 * [calc_divider returns the PIT divider based on period]
 * @param  msec [period for scheduler]
 * @return      [none]
 */
uint32_t calc_divider(uint32_t msec) {
    return PIT_MAX_DIV / msec;
}

/**
 * [pit_handle_interrupt handles a pit interrupt by switching to the next process]
 * Inputs: none
 * Outputs: none
 */
void pit_handle_interrupt() {
    // Send EOI to end the interrupt
    send_eoi(IRQ_SYSTEM_TIMER);

    // if scheduling is disabled, we are done already!
    // if the first program hasnt been run yet, we dont want to schedule anything!
    if(!SCHED_ENABLED || !first_prog_run()) {
        return;
    }

    // get the current and next processes to run
    uint8_t curr_proc_term_num = get_curr_running_term_proc();
    uint8_t next_proc_term_num = get_next_running_term_proc();

    // if there is only one process running in one terminal, we have nothing to schedule! return
    if(curr_proc_term_num == next_proc_term_num) {
        return;
    }

    // get the pcbs for the current and next processes
    pcb_t * curr_proc = get_pcb(curr_proc_term_num);
    pcb_t * next_proc = get_pcb(next_proc_term_num);

    // if something went wrong and either are null, return silently
    if(curr_proc == NULL) {
        return;
    }

    if(next_proc == NULL) {
        set_active_terminal(next_proc_term_num);
        update_screen(next_proc_term_num, curr_proc_term_num);
        set_running_proc(next_proc_term_num);
        pcb_t * curr_pcb = get_pcb(curr_proc_term_num);
        asm volatile("movl %%esp, %0":"=g"(curr_pcb->p_sched_ksp));
        asm volatile("movl %%ebp, %0":"=g"(curr_pcb->p_sched_kbp));
        execute((uint8_t*) "shell");
    } else {
        // store ksp/kbp before move to the current processes pcb
        asm volatile("movl %%esp, %0":"=g"(curr_proc->p_sched_ksp));
        asm volatile("movl %%ebp, %0":"=g"(curr_proc->p_sched_kbp));

        // set the correct running process
        set_running_proc(next_proc_term_num);

        // change the page directory to the correct process
        switch_pd(next_proc->proc_num, next_proc->base);

        // set the esp0 to the correct one for the next process
        tss.esp0 = _8MB - (_8KB) * (next_proc->proc_num) - 4;

        // stack switch
        asm volatile("movl %0, %%esp"::"g"(next_proc->p_sched_ksp));
        asm volatile("movl %0, %%ebp"::"g"(next_proc->p_sched_kbp));
    }
}

void context_switch(uint8_t curr_proc_term_num, uint8_t next_proc_term_num) {
    // don't allow the manual context switch if scheduling is enabled
    // it should only happen in the interrupt handler for PIT
    if(SCHED_ENABLED) {
        return;
    }

    if(next_proc_term_num == 1 && !get_second_term_start()) {
        set_running_proc(1);
        set_second_term_start();
        pcb_t * prev_pcb = get_pcb(curr_proc_term_num);
        asm volatile("movl %%esp, %0":"=g"(prev_pcb->p_sched_ksp));
        asm volatile("movl %%ebp, %0":"=g"(prev_pcb->p_sched_kbp));
        execute((uint8_t*) "shell");
    } else if(next_proc_term_num == 2 && !get_third_term_start()) {
        set_running_proc(2);
        set_third_term_start();
        pcb_t * prev_pcb = get_pcb(curr_proc_term_num);
        asm volatile("movl %%esp, %0":"=g"(prev_pcb->p_sched_ksp));
        asm volatile("movl %%ebp, %0":"=g"(prev_pcb->p_sched_kbp));
        execute((uint8_t*) "shell");
    } else {
        // get the pcbs for the current and next processes
        pcb_t * curr_proc = get_pcb(curr_proc_term_num);
        pcb_t * next_proc = get_pcb(next_proc_term_num);

        // if something went wrong and either are null, return silently
        if(curr_proc == NULL || next_proc == NULL) {
            return;
        }

        // store ksp/kbp before move to the current processes pcb
        asm volatile("movl %%esp, %0":"=g"(curr_proc->p_sched_ksp));
        asm volatile("movl %%ebp, %0":"=g"(curr_proc->p_sched_kbp));

        // set the correct running process
        set_running_proc(next_proc_term_num);

        // change the page directory to the correct process
        switch_pd(next_proc->proc_num, next_proc->base);

        // set the esp0 to the correct one for the next process
        tss.esp0 = _8MB - (_8KB) * (next_proc->proc_num) - 4;

        // stack switch
        asm volatile("movl %0, %%esp"::"g"(next_proc->p_sched_ksp));
        asm volatile("movl %0, %%ebp"::"g"(next_proc->p_sched_kbp));
    }
}
