#include <kernel/syscall.h>

/*curr_proc and prev_proc for all three terminals*/
pcb_t * curr_proc[] = {NULL, NULL, NULL};
pcb_t * prev_proc[] = {NULL, NULL, NULL};

/* initialize variables for current terminal and process */
uint8_t curr_active_term = 0;
uint8_t curr_proc_id_mask = 0;
uint32_t curr_proc_id = 0;
uint32_t first_program_run = 0;

/* function pointers to be called for system calls */
static func_ptr stdin_ops_table[4] = {NULL, terminal_read, NULL, NULL};
static func_ptr stdout_ops_table[4] = {NULL, NULL, terminal_write, NULL};
static func_ptr rtc_ops_table[4] = {rtc_open, rtc_read, rtc_write, rtc_close};
static func_ptr dir_ops_table[4] = {dir_open, dir_read, dir_write, dir_close};
static func_ptr files_ops_table[4] = {fs_open, fs_read, fs_write, fs_close};

/**
 * [halts the current process and jumps back to shell or previous process]
 * @param  status [Status of the program being called]
 * @return        [0 on success, -1 on fail]
 */
uint32_t g_status = 0;

int32_t halt (uint8_t status) {
    unsigned long flags;
    cli_and_save(flags);

    // get the correct pcb
    pcb_t * proc_ctrl_blk = curr_proc[curr_active_term];

    // get the process number to free
    uint32_t free_proc_num = proc_ctrl_blk->proc_num;

    // check if this is the first terminal
    if(prev_proc[curr_active_term] == NULL) {
        // restart this process since its the first process
        // we can hardcode this to shell since that is the first process every time
        uint8_t f_init_data[32];
        uint32_t entrypoint = 0;

        // Grab the first 32 bytes of the file to see if it is runnable
        // and find where it starts
        file_array exec_read;
        strcpy((int8_t*) &(exec_read.file_name), (int8_t*) "shell");
        exec_read.file_position = 0;

        if(fs_read(&exec_read, f_init_data, 32) == -1) {
            restore_flags(flags);
            return -1;
        }

        // Grab the entry point of the application
        entrypoint += (uint32_t)f_init_data[27] << 24;
        entrypoint += (uint32_t)f_init_data[26] << 16;
        entrypoint += (uint32_t)f_init_data[25] << 8;
        entrypoint += (uint32_t)f_init_data[24];

        restore_flags(flags);

        // jump back to the beginning of the executable
        jmp_usr_exec(entrypoint);
    }

    g_status = status;

    // set the process to free in the process buffer
    curr_proc_id_mask ^= (PROGRAM_LOCATION_MASK >> free_proc_num);

    // Close STDIN
    proc_ctrl_blk->fds[0].operations_pointer = NULL;
    proc_ctrl_blk->fds[0].flags = NOT_USE;

    // Close STDOUT
    proc_ctrl_blk->fds[1].operations_pointer = NULL;
    proc_ctrl_blk->fds[1].inode = NULL;
    proc_ctrl_blk->fds[1].flags = NOT_USE;

    // Close any open FDS
    int i;
    for(i = 2; i < 8; i ++) {
        close(i);
    }

    // reset the page entries
    switch_pd(prev_proc[curr_active_term]->proc_num, prev_proc[curr_active_term]->base);
    tss.esp0 = _8MB - (_8KB) * prev_proc[curr_active_term]->proc_num - 4;

    // stack switch
    asm volatile("movl %0, %%esp"::"g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %0, %%ebp"::"g"(proc_ctrl_blk->p_kbp));

    // swap the pcbs correctly
    curr_proc[curr_active_term] = prev_proc[curr_active_term];
    prev_proc[curr_active_term] = (pcb_t *) prev_proc[curr_active_term]->prev;

    // restore the processor flags
    restore_flags(flags);

    // jump to the end of execute to return to the interrupt handler
    asm volatile("jmp EXECUTE_EXIT");

    return 0;
}

/**
 * [starts a new process on the stack ]
 * @param  command [the input to be executed]
 * @return         [0 on success, -1 if failed]
 */
int32_t execute (const uint8_t * command) {
    unsigned long flags;
    cli_and_save(flags);

    // Used to hold the first 32 bytes of the file
    // These 32 bytes will contain the exec information and the entry point of the file
    uint8_t f_init_data[32];
    uint32_t entrypoint = 0;
    uint32_t temp_process_mask = PROGRAM_LOCATION_MASK;
    uint32_t curr_proc_id = 0;
    uint32_t i;

    // fail if an invalid command is specified
    if (command == NULL || strlen((int8_t *) command) == 0) {
        restore_flags(flags);
        return -1;
    }

    // Store the command args
    uint8_t space_flag = 0;
    uint8_t cmd_len;
    uint8_t temp_arg[64];
    for(i = 0; command[i] != '\0' ; i++) {
        if(command[i] == ' ' && space_flag == 0) {
            space_flag = 1;
            cmd_len = i;
        } else if(space_flag == 1) {
            temp_arg[i - cmd_len - 1] = command[i];
        } else {
            if(i >= 32 && space_flag == 0) {
                restore_flags(flags);
                return -1;
            }
        }
    }
    temp_arg[i-cmd_len-1] = '\0';

    // get the file name to execute
    uint8_t * f_name = strtok(command);

    // clear screen if clear is typed
    if (0 == strncmp((int8_t*) f_name, (int8_t*) "clear", strlen((int8_t *) "clear"))) {
        clear_screen();
        restore_flags(flags);
        return 0;
    }

    // Grab the first 32 bytes of the file to see if it is runnable
    // and find where it starts
    file_array exec_read;
    strcpy((int8_t*)&(exec_read.file_name),(int8_t*)f_name);
    exec_read.file_position = 0;
    if(fs_read(&exec_read, f_init_data, 32) == -1) {
        restore_flags(flags);
        return -1;
    }

    // See if the file is executeable
    if (!((f_init_data[0] == MAGIC_NUM_1) && (f_init_data[1] == MAGIC_NUM_2) && (f_init_data[2] == MAGIC_NUM_3) && (f_init_data[3] == MAGIC_NUM_4))) {
        printf("ERROR: Non-Runnable file!\n");
        restore_flags(flags);
        return -1;
    }

    // Grab the entry point of the application
    entrypoint += (uint32_t)f_init_data[27] << 24;
    entrypoint += (uint32_t)f_init_data[26] << 16;
    entrypoint += (uint32_t)f_init_data[25] << 8;
    entrypoint += (uint32_t)f_init_data[24];

    // Find a open spot for the program to run
    for (i = 0; i < MAX_PROG_NUM; i++) {
        if(!(temp_process_mask & curr_proc_id_mask)) {
            curr_proc_id_mask |= temp_process_mask;
            curr_proc_id = i;
            break;
        } else {
            temp_process_mask = temp_process_mask >> 1;
        }
    }

    // Max number of programs reached, error out
    if (i >= MAX_PROG_NUM) {
        printf("ERROR: Out of runnable program slots!\n");
        restore_flags(flags);
        return -1;
    }

    // Create a page directory for the program
    uint32_t base = init_new_process(curr_proc_id);

    // Copy the program to the page directory
    copy_file_to_addr(f_name, PROGRAM_EXEC_ADDR);

    // Create a process control block for our program in the kernel stack
    pcb_t * proc_ctrl_blk = (pcb_t*) (_8MB - ((_8KB)*(curr_proc_id + 1)));

    // Grab and store the ESP and EBP in the PCB
    asm volatile("movl %%esp, %0":"=g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %%ebp, %0":"=g"(proc_ctrl_blk->p_kbp));

    // Store Proc ID
    proc_ctrl_blk->proc_num = curr_proc_id;

    // store Prev address
    proc_ctrl_blk->base = base;

    // store arguments into pcb
    strcpy((int8_t*)proc_ctrl_blk->args, (const int8_t*)temp_arg);

    // Initalize PCB file descriptors
    for (i = 0; i < 8; i++) {
        proc_ctrl_blk->fds[i].operations_pointer = NULL;
        proc_ctrl_blk->fds[i].inode = NULL;
        proc_ctrl_blk->fds[i].file_position = 0;
        proc_ctrl_blk->fds[i].flags = NOT_USE;
    }

    // Set TSS Value
    tss.esp0 = _8MB - (_8KB) * curr_proc_id - 4;

    // Open STDIN
    proc_ctrl_blk->fds[0].operations_pointer = stdin_ops_table;
    proc_ctrl_blk->fds[0].flags = IN_USE;

    // Open STDOUT
    proc_ctrl_blk->fds[1].operations_pointer = stdout_ops_table;
    proc_ctrl_blk->fds[1].inode = NULL;
    proc_ctrl_blk->fds[1].flags = IN_USE;

    // add meta information to the pcb about the process
    // strcpy(proc_ctrl_blk->proc_name, f_name);

    // set pcbs correctly
    prev_proc[curr_active_term] = curr_proc[curr_active_term];
    curr_proc[curr_active_term] = proc_ctrl_blk;
    curr_proc[curr_active_term]->prev = (struct pcb_t *) prev_proc[curr_active_term];

    // set the flag saying that the first program was run
    first_program_run = 1;

    // Grab and store the ESP and EBP in the PCB
    asm volatile("movl %%esp, %0":"=g"(proc_ctrl_blk->p_sched_ksp));
    asm volatile("movl %%ebp, %0":"=g"(proc_ctrl_blk->p_sched_kbp));

    // restore processor flags
    restore_flags(flags);

    // jump to the program to begin execution
    jmp_usr_exec(entrypoint);

    asm volatile("EXECUTE_EXIT:");
    return g_status;
}

/**
 * [read  calls the correct system call to read a file descriptor]
 * @param  fd     [The file descriptor of the file to read]
 * @param  buf    [The buffer to read/copy into]
 * @param  nbytes [number of bytes to read]
 * @return        [bytes read on success, -1 on fail]
 */
int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    unsigned long flags;
    cli_and_save(flags);

    //error check for read. anything greater than 7 is out of array index. fd 1 is terminal write, so read gives error
    if (buf == NULL || fd > 7 || fd < 0 || fd == 1 || curr_proc[curr_active_term]->fds[fd].flags != IN_USE) {
        return -1;
    }

    restore_flags(flags);
    //call the specified function
    return curr_proc[curr_active_term]->fds[fd].operations_pointer[READ](&(curr_proc[curr_active_term]->fds[fd]), buf, nbytes);
}

/**
 * [write  call the function pointer required to write ]
 * @param  fd     [The file descriptor of the file to be written to]
 * @param  buf    [buffer containing data to write]
 * @param  nbytes [number of byte to write]
 * @return        [-1 on fail, else number of bytes written on success]
 */
int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    unsigned long flags;
    cli_and_save(flags);

    //error check for write, fd 0 is terminal read so write gives error. anything above 7 is out of array index
    if (buf == NULL || fd > 7 || fd <= 0 || curr_proc[curr_active_term]->fds[fd].flags != IN_USE) {
        return -1;
    }

    restore_flags(flags);

    //call the specified function
    return curr_proc[curr_active_term]->fds[fd].operations_pointer[WRITE](fd, buf, nbytes);
}

/**
 * [open  Opens a given file]
 * @param  filename [The name of the file to open]
 * @return          [-1 on fail, else 0]
 */
int32_t open (const uint8_t * filename) {
    unsigned long flags;
    cli_and_save(flags);

    // fail if an invalid filename is specified
    if (filename == NULL || strlen((int8_t *) filename) == 0) {
        return -1;
    }

    dentry_t file_info;
    int32_t check = read_dentry_by_name(filename, &file_info);

    //checking if the file name exists
    if(check == -1) {
        return -1;
    }

    //put back calling open
    int i;
    //fd 0 and 1 are stdin and stdout so they are always open
    for(i = 2; i < MAX_FILES; i++) {
        if(curr_proc[curr_active_term]->fds[i].flags == NOT_USE) {
            switch(file_info.file_type) {
                case RTC_TYPE:
                    curr_proc[curr_active_term]->fds[i].operations_pointer = rtc_ops_table;
                    curr_proc[curr_active_term]->fds[i].inode = NULL;
                    curr_proc[curr_active_term]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_active_term]->fds[i].file_name), (int8_t*) filename);
                    rtc_open();
                    break;
                case DIR_TYPE:
                    curr_proc[curr_active_term]->fds[i].operations_pointer = dir_ops_table;
                    curr_proc[curr_active_term]->fds[i].inode = NULL;
                    curr_proc[curr_active_term]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_active_term]->fds[i].file_name), (int8_t*) filename);
                    dir_open(filename);
                    break;
                case FILE_TYPE:
                    curr_proc[curr_active_term]->fds[i].operations_pointer = files_ops_table;
                    curr_proc[curr_active_term]->fds[i].inode = get_inode(file_info.inode_num);
                    curr_proc[curr_active_term]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_active_term]->fds[i].file_name), (int8_t*) filename);
                    fs_open(filename);
                    break;
            }

            curr_proc[curr_active_term]->fds[i].flags = IN_USE;
            restore_flags(flags);
            return i;
        }
    }

    restore_flags(flags);
    return -1;
}

/**
 * [close  closes a previously opened file]
 * @param  fd [The file descriptor of the file to close]
 * @return    [-1 on fail, else 0]
 */
int32_t close (int32_t fd) {
    unsigned long flags;
    cli_and_save(flags);

    //should be impossible to close stdin and stdout, so fd 0 and 1 give error
    if(fd >= 2 && fd <= 7 && (curr_proc[curr_active_term]->fds[fd].flags == IN_USE)) {
        curr_proc[curr_active_term]->fds[fd].flags = NOT_USE;
        curr_proc[curr_active_term]->fds[fd].file_position = 0;
        restore_flags(flags);
        return curr_proc[curr_active_term]->fds[fd].operations_pointer[CLOSE](fd);
    }

    restore_flags(flags);
    return -1;
}

/**
 * [getargs  gets the arguments for a command and returns them]
 * @param  buf    [buffer to copy args into]
 * @param  nbytes [number of bytes to copy into buffer]
 * @return        [-1 on fail, 0 on success]
 */
int32_t getargs (uint8_t * buf, int32_t nbytes) {
    unsigned long flags;
    cli_and_save(flags);

    //error check
    if(nbytes < 0 || buf == NULL) {
        return -1;
    }

    //copy arguements into buffer
    strncpy((int8_t*) buf, (const int8_t*) curr_proc[curr_active_term]->args, nbytes);
    restore_flags(flags);
    return 0;
}

/**
 * [vidmap  maps text-mode video memory into user space]
 * @param  screen_start [the start of the screen]
 * @return              [-1 on fail, 0 on success]
 */
int32_t vidmap (uint8_t ** screen_start) {
    unsigned long flags;
    cli_and_save(flags);

    //error check to make sure screen_start is valid
    if((uint32_t) screen_start < VID_MEM_START || (uint32_t) screen_start > VID_MEM_END) {
        return -1;
    }

    //map screen_start to vidoe memory
    *screen_start = (uint8_t *) VIDEO;

    restore_flags(flags);
    return 0;
}

/**
 * [set_handler Related to signal handling (not implemented)]
 * @param  signum          [description]
 * @param  handler_address [description]
 * @return                 [description]
 */
int32_t set_handler (int32_t signum, void * handler_address) {
    return 0;
}

/**
 * [sigreturn  Related to signal handling (not implemened)]
 * @return  [description]
 */
int32_t sigreturn (void) {
    return 0;
}

/**
 * [get_pcb retrieves the pcb based on the specified terminal]
 * @param  term [the terminal for the desired pcb]
 * @return      [pcb of input terminal number]
 */
pcb_t * get_pcb(int32_t term) {
    return curr_proc[term];
}

/**
 * [first_prog_run Returns the first program run variable
 * @return [first_program_run]
 */
int32_t first_prog_run() {
    return first_program_run;
}

/**
 * [get_curr_running_term_proc retrieves the current active terminal]
 * @return [current active terminal number]
 */
uint8_t get_curr_running_term_proc() {
    return curr_active_term;
}

/**
 * [get_next_running_term_proc Returns the next terminal to switch to for scheduling]
 * @return [the next terminal number]
 */
uint8_t get_next_running_term_proc() {
    switch(curr_active_term) {
        case TERM_0:
            return TERM_1;
            break;
        case TERM_1:
            return TERM_2;
            break;
        case TERM_2:
            return TERM_0;
            break;
        default:
            return -1;
            break;
    }
}

/**
 * [set_running_proc Sets the current active terminal to given process]
 * @param proc [process to set current active terminal to]
 */
void set_running_proc(uint8_t proc) {
    curr_active_term = proc;
    return;
}

/*
 * Thanks to: https://sourceware.org/newlib/libc.html#Syscalls
 *     (Red Hat Minimal Implementation)
 * And to: http://code.metager.de/source/xref/hurd/viengoos/libhurd-mm/sbrk.c
 *     (GNU Hurd Implementation)
 */
void * sbrk(uint32_t nbytes) {
    static void * heap_ptr = NULL;
    void * base;

    if (heap_ptr == NULL) {
        heap_ptr = (void *)&_end;
    }

    if ((RAMSIZE - heap_ptr) >= 0) {
        base = heap_ptr;
        heap_ptr += nbytes;
        return (base);
    } else {
        return ((void *)-1);
    }
}
