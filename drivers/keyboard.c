
#include <drivers/keyboard.h>

// array of lowercase ASCII codes, indexed by the corresponding characters scan code
uint8_t code_to_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '-',
    '=', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, 'q', 'w',
    'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
    'l', ';', '\'', '`', ASCII_PLACEHOLDER,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', ASCII_PLACEHOLDER,
    '*', ASCII_PLACEHOLDER, ' ', ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0',
    '.', ASCII_NULL_CHAR, ASCII_NULL_CHAR, ASCII_NULL_CHAR,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER
};

// array of shifted letters
uint8_t shift_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, '!', '@', '#', '$',
    '%', '^', '&', '*', '(', ')', '_',
    '+', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, 'Q', 'W',
    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
    'L', ':', '"', '~', ASCII_PLACEHOLDER,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?', ASCII_PLACEHOLDER,
    '*', ASCII_PLACEHOLDER, ' ', ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0',
    '.', ASCII_NULL_CHAR, ASCII_NULL_CHAR, ASCII_NULL_CHAR,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER
};
// array of capital letters and such
uint8_t caps_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '-',
    '=', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, 'Q', 'W',
    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
    'L', ':', '"', '~', ASCII_PLACEHOLDER,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', ',', '.', '/', ASCII_PLACEHOLDER,
    '*', ASCII_PLACEHOLDER, ' ', ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0',
    '.', ASCII_NULL_CHAR, ASCII_NULL_CHAR, ASCII_NULL_CHAR,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER
};

uint8_t active_terminal = 0;
uint8_t prev_terminal = 0;

uint8_t second_term_start = 0;
uint8_t third_term_start = 0;

uint32_t keyboard_buf[NUM_TERMINALS][MAX_CHARS_IN_BUF];
uint32_t num_chars_in_buf[NUM_TERMINALS] = {0, 0, 0};
int read_buf_ready[NUM_TERMINALS] = {0, 0, 0};

// variables designating status of special keys
int special_key_enabled = 0;
int cntrl_l_on = 0;
int cntrl_r_on = 0;
int caps_on = 0;
int shift_l_on = 0;
int shift_r_on = 0;
int alt_l_on = 0;
int alt_r_on = 0;

/**
 * [terminal_open  initializes the active terminal]
 * @param  filename [just for the sake of having the input]
 * @return          [0 on success]
 */
int32_t terminal_open (const uint8_t * filename) {
    clear_screen();

    int i = 0;

    for(i = 0; i < MAX_CHARS_IN_BUF; i++) {
        keyboard_buf[active_terminal][i] = NULL;
    }

    num_chars_in_buf[active_terminal] = 0;
    read_buf_ready[active_terminal] = 0;

    return 0;
}

/**
 * [terminal_close  Closes terminal (not implemented)]
 * @param  fd [file descriptor]
 * @return    [0]
 */
int32_t terminal_close (int32_t fd) {
    return 0;
}

/**
 * [terminal_read  reads from terminal into a buffer]
 * @param  fd     [file descriptor]
 * @param  buf    [Buffer to copy from terminal into]
 * @param  nbytes [number of bytes to read]
 * @return        [number of bytes rtc_read]
 */
int32_t terminal_read (int32_t fd, uint8_t * buf, int32_t nbytes) {
    int bytes_read = 0;
    int i = 0;

    while(!read_buf_ready[get_curr_running_term_proc()]);

    for(i = 0; i <= nbytes; i++) {
        buf[i] = keyboard_buf[get_curr_running_term_proc()][i];
        keyboard_buf[get_curr_running_term_proc()][i] = NULL;
        bytes_read++;
    }

    new_line();

    num_chars_in_buf[get_curr_running_term_proc()] = 0;
    read_buf_ready[get_curr_running_term_proc()] = 0;

    return bytes_read;
}
/**
 * [terminal_write  write contents of buffer to screen]
 * @param  fd     [file descriptor]
 * @param  buf    [buffer containing contents to write]
 * @param  nbytes [number of bytes to write]
 * @return        [number of bytes successfully printed]
 */
int32_t terminal_write (int32_t fd, const uint8_t * buf, int32_t nbytes) {
    int num_printed = 0;
    int i = 0;

    for(i = 0; i < nbytes; i++) {
        if(add_char_to_buffer(buf[i], get_curr_running_term_proc())) {
            num_printed++;
        }
    }

    num_chars_in_buf[active_terminal] = 0;
    read_buf_ready[active_terminal] = 0;

    return num_printed;
}

void reset_curr_command() {
    while(num_chars_in_buf[active_terminal] > 0) {
        del_last_char();
        num_chars_in_buf[active_terminal]--;
        keyboard_buf[active_terminal][num_chars_in_buf[active_terminal]] = NULL;
    }
}

void fill_curr_command(char * command) {
    int i = 0;
    int length = strlen(command);

    for(i = 0; i < length; i++) {
        add_char_to_buffer(command[i], active_terminal);
    }
}

/**
 * [reset_term reinitializes the current active terminal]
 * Inputs: None
 * Outputs: None
 */
void reset_term() {
    int i = 0;

    for(i = 0; i < MAX_CHARS_IN_BUF; i++) {
        keyboard_buf[active_terminal][i] = NULL;
    }

    num_chars_in_buf[active_terminal] = 0;

    clear_screen();
}

/**
 * [add_char_to_buffer Adds the current character to the keyboard buffer]
 * @param  new_char [character to write to buffer]
 * @param  term     [current active terminal]
 * @return          [1 on success]
 */
uint32_t add_char_to_buffer(uint8_t new_char, uint8_t term) {
    // if we haven't reached the buffer limit, add the char to the buffer and print the key
    if(num_chars_in_buf[term] < MAX_CHARS_IN_BUF) {
        switch(new_char) {
            case '\n':
                new_line();
                break;
            case '\0':
                return 1;
                break;
            default:
                keyboard_buf[term][num_chars_in_buf[term]] = new_char;
                num_chars_in_buf[term]++;
                putc(new_char);
                break;
        }
        return 1;
    }

    return 0;
}

/**
 * [handle_enter Handles the enter key press]
 * Inputs: None
 * Outputs: None
 */
void handle_enter() {
    int i = 0;

    //fills keyboard buffer with null characters
    for(i = num_chars_in_buf[active_terminal]; i <= MAX_CHARS_IN_BUF; i++) {
        keyboard_buf[active_terminal][i] = ASCII_NULL_CHAR;
    }

    read_buf_ready[active_terminal] = 1;
}

/**
 * [handle_backspace Handles the backspace key press]
 * Inputs: None
 * Outputs: None
 */
void handle_backspace() {
    if(num_chars_in_buf[active_terminal] > 0) {
        del_last_char();
        num_chars_in_buf[active_terminal]--;
        keyboard_buf[active_terminal][num_chars_in_buf[active_terminal]] = NULL;
    }
}

/**
 * Wait for a keyboard ACK
 */
void kbd_ack(void){
    while(!(inb(KEYBOARD_D_PORT) == KEYBOARD_D_ACK));
}

/**
 * Set the keyboard LED status
 * @param caps   0 or 1 If caps lock is enabled
 * @param num    0 or 1 If num lock is enabled
 * @param scroll 0 or 1 If scroll lock is enabled
 */
void kbd_led_handling(int caps, int num, int scroll) {
    // Tell keyboard we want to set LED's and wait for ack
    outb(KEYBOARD_LED_V, KEYBOARD_D_PORT);
    kbd_ack();

    // If nothing is enabled, then all LEDs will be set to 0 (off)
    int ledstatus = 0x0;

    // Check which LED to enable and set based on bit masks
    if (caps) {
        ledstatus = ledstatus | 0x4;
    }

    if (num) {
        ledstatus = ledstatus | 0x2;
    }

    if (scroll) {
        ledstatus = ledstatus | 0x1;
    }

    outb(ledstatus, KEYBOARD_D_PORT);
}

void handle_tab() {
    char * complete_name = autocomplete_command((char *) keyboard_buf[active_terminal]);

    if(complete_name == (char *) -1) {
        return;
    }

    reset_curr_command();
    fill_curr_command(complete_name);
}

/**
 * handle_keypress()
 *
 * Description: Prints the key pressed by the keyboard to the screen
 *
 * Inputs: None (keycode is read from the keyboard port)
 * Outputs: Prints the corresponding character to the screen
 * Returns: None
 */
void handle_keypress() {
    unsigned long flags;

    // block interrupts
    cli_and_save(flags);

    // get scan code
    uint8_t key_code = inb(KEYBOARD_D_PORT);

    if(key_code < MAX_MAKE_SCANCODE) {
        // make key
        uint8_t key_ascii = code_to_ascii[key_code];

        if(key_ascii != ASCII_PLACEHOLDER && !special_key_enabled) {
            // if its a valid char that we can print, print it
            if(cntrl_l_on || cntrl_r_on) {
                switch (key_code) {
                    case KEY_MAKE_L:
                        reset_term();
                        break;
                    case KEY_MAKE_C:
                        // send eoi and restore prev flags
                        send_eoi(IRQ_KEYBOARD_CTRL);
                        restore_flags(flags);

                        // call halt to terminate the current process
                        halt(-2);
                        break;
                    default:
                        break;
                }
            } else if (caps_on && !(shift_l_on || shift_r_on)) {
                // print caps version
                key_ascii = caps_ascii[key_code];
                add_char_to_buffer(key_ascii, active_terminal);
            } else if (!caps_on && (shift_l_on || shift_r_on)) {
                // print shift version
                key_ascii = shift_ascii[key_code];
                add_char_to_buffer(key_ascii, active_terminal);
            } else {
                // print char normally
                add_char_to_buffer(key_ascii, active_terminal);
            }
        } else if(special_key_enabled) {
            special_key_enabled = 0;
        } else {
            switch(key_code) {
                case KEY_MAKE_L_CTRL:
                    cntrl_l_on = 1;
                    break;
                case KEY_MAKE_L_SHIFT:
                    shift_l_on = 1;
                    break;
                case KEY_MAKE_R_SHIFT:
                    shift_r_on = 1;
                    break;
                case KEY_MAKE_CAPS:
                    caps_on = !caps_on;
                    kbd_led_handling(caps_on, 0, 0);
                    break;
                case KEY_MAKE_ENTER:
                    handle_enter();
                    break;
                case KEY_MAKE_BKSP:
                    handle_backspace();
                    break;
                case KEY_MAKE_ALT:
                    alt_l_on = 1;
                    break;
                case KEY_MAKE_F1:
                    // if((alt_l_on || alt_r_on) && active_terminal != 0) {
                        // switch to the video memory of the first terminal
                        update_screen(0, active_terminal);
                        prev_terminal = active_terminal;
                        active_terminal = 0;

                        // send eoi and restore prev flags
                        send_eoi(IRQ_KEYBOARD_CTRL);
                        restore_flags(flags);

                        context_switch(prev_terminal, active_terminal);
                    // }
                    break;
                case KEY_MAKE_F2:
                    // if((alt_l_on || alt_r_on) && active_terminal != 1) {
                        // switch to the video memory of the second terminal
                        update_screen(1, active_terminal);
                        prev_terminal = active_terminal;
                        active_terminal = 1;

                        // send eoi and restore prev flags
                        send_eoi(IRQ_KEYBOARD_CTRL);
                        restore_flags(flags);

                        context_switch(prev_terminal, active_terminal);
                    // }
                    break;
                case KEY_MAKE_F3:
                    // if((alt_l_on || alt_r_on) && active_terminal != 2) {
                        // switch to the video memory of the third terminal
                        update_screen(2, active_terminal);
                        prev_terminal = active_terminal;
                        active_terminal = 2;

                        // send eoi and restore prev flags
                        send_eoi(IRQ_KEYBOARD_CTRL);
                        restore_flags(flags);

                        context_switch(prev_terminal, active_terminal);
                    // }
                    break;
                case KEY_MAKE_TAB:
                    handle_tab();
                    break;
                default:
                    break;
            }
        }
    } else {
        // break or special key
        if(key_code == SPECIAL_KEY) {
            special_key_enabled = 1;

            // get next code
            key_code = inb(KEYBOARD_D_PORT);

            switch(key_code) {
                case KEY_MAKE_R_CTRL:
                    cntrl_r_on = 1;
                    break;
                case KEY_BREAK_R_CTRL:
                    cntrl_r_on = 0;
                    break;
                case KEY_MAKE_L_ARROW:
                    break;
                case KEY_MAKE_R_ARROW:
                    break;
                case KEY_MAKE_U_ARROW:
                    break;
                case KEY_MAKE_D_ARROW:
                    break;
                case KEY_MAKE_ALT:
                    alt_r_on = 1;
                    break;
                case KEY_BREAK_ALT:
                    alt_r_on = 0;
                    break;
                default:
                    // we don't care about the rest of the special keys
                    break;
            }
        } else {
            // get break and set correct key status
            switch(key_code) {
                case KEY_BREAK_L_CTRL:
                    cntrl_l_on = 0;
                    break;
                case KEY_BREAK_L_SHIFT:
                    shift_l_on = 0;
                    break;
                case KEY_BREAK_R_SHIFT:
                    shift_r_on = 0;
                    break;
                case KEY_BREAK_ALT:
                    alt_l_on = 0;
                    break;
                default:
                    // we don't care about the rest of the breaks
                    break;
            }
        }
    }

    // send eoi and restore prev flags
    send_eoi(IRQ_KEYBOARD_CTRL);
    restore_flags(flags);
}

/**
 * [get_active_terminal Retrieves the current active terminal]
 * @return  [current active terminal]
 */
uint8_t get_active_terminal(void) {
    return active_terminal;
}
/**
 * [set_active_terminal Sets the current terminal to input]
 * @param term [New active terminal]
 */
void set_active_terminal(uint8_t term) {
    active_terminal = term;
}

/**
 * [get_second_term_start returns whether or not the second terminal has been started]
 * @return [second_term_start]
 */
uint8_t get_second_term_start() {
    return second_term_start;
}

/**
 * [set_second_term_start Sets the second terminal to started]
 * Inputs: None
 * Outputs: None
 */
void set_second_term_start() {
    second_term_start = 1;
}

/**
 * [set_third_term_start Sets third terminal to started]
 * Inputs: None
 * Outputs: None
 */
void set_third_term_start() {
    third_term_start = 1;
}

/**
 * [get_third_term_start returns whether or not the third terminal has been started]
 * @return [third_term_start]
 */
uint8_t get_third_term_start() {
    return third_term_start;
}
