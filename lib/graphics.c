#include <lib/graphics.h>

static int screen_x[NUM_TERMINALS] = {0, 0, 0};
static int screen_y[NUM_TERMINALS] = {0, 0, 0};
static int active_terminal = 0;

static char* video_mem = (char *)VIDEO;
int text_color = 0x04;

uint8_t * args;

char* term_vid_mem[NUM_TERMINALS] = {(char*)VIDEO_PHYS_ADDR0, (char*)VIDEO_PHYS_ADDR1, (char*)VIDEO_PHYS_ADDR2};


/*
* void clear(void);
*   Inputs: void
*   Return Value: none
*   Function: Clears video memory
*/
void clear(void) {
    int32_t i;
    for(i=0; i < NUM_ROWS * NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = text_color;
    }
}

/**
 * Draw A cursor block at position (non functioning)
 * @param x     x location to draw at
 * @param y     y location to draw at
 * @param color color of the cursor
 *
 * NOTE: Code Page 737
 */
void draw_full_block(int32_t x, int32_t y, uint8_t p_char) {
    int32_t offset = y*NUM_COLS + x;
    *(uint8_t *)(video_mem + (offset<<1)) = p_char;
    *(uint8_t *)(video_mem + (offset<<1) + 1) = text_color;
}

uint8_t get_full_block(int32_t x, int32_t y) {
    int32_t offset = y * NUM_COLS + x;
    return *(uint8_t *)(video_mem + (offset<<1));
}

/* Standard printf().
 * Only supports the following format strings:
 * %%  - print a literal '%' character
 * %x  - print a number in hexadecimal
 * %u  - print a number as an unsigned integer
 * %d  - print a number as a signed integer
 * %c  - print a character
 * %s  - print a string
 * %#x - print a number in 32-bit aligned hexadecimal, i.e.
 *       print 8 hexadecimal digits, zero-padded on the left.
 *       For example, the hex number "E" would be printed as
 *       "0000000E".
 *       Note: This is slightly different than the libc specification
 *       for the "#" modifier (this implementation doesn't add a "0x" at
 *       the beginning), but I think it's more flexible this way.
 *       Also note: %x is the only conversion specifier that can use
 *       the "#" modifier to alter output.
 * */
int32_t printf(int8_t *format, ...) {
    /* Pointer to the format string */
    int8_t* buf = format;

    /* Stack pointer for the other parameters */
    int32_t* esp = (void *)&format;
    esp++;

    while(*buf != '\0') {
        switch(*buf) {
            case '%':
                {
                    int32_t alternate = 0;
                    buf++;

format_char_switch:
                    /* Conversion specifiers */
                    switch(*buf) {
                        /* Print a literal '%' character */
                        case '%':
                            putc('%');
                            break;

                        /* Use alternate formatting */
                        case '#':
                            alternate = 1;
                            buf++;
                            /* Yes, I know gotos are bad.  This is the
                             * most elegant and general way to do this,
                             * IMHO. */
                            goto format_char_switch;

                        /* Print a number in hexadecimal form */
                        case 'x':
                            {
                                int8_t conv_buf[64];
                                if(alternate == 0) {
                                    itoa(*((uint32_t *)esp), conv_buf, 16);
                                    puts(conv_buf);
                                } else {
                                    int32_t starting_index;
                                    int32_t i;
                                    itoa(*((uint32_t *)esp), &conv_buf[8], 16);
                                    i = starting_index = strlen(&conv_buf[8]);
                                    while(i < 8) {
                                        conv_buf[i] = '0';
                                        i++;
                                    }
                                    puts(&conv_buf[starting_index]);
                                }
                                esp++;
                            }
                            break;

                        /* Print a number in unsigned int form */
                        case 'u':
                            {
                                int8_t conv_buf[36];
                                itoa(*((uint32_t *)esp), conv_buf, 10);
                                puts(conv_buf);
                                esp++;
                            }
                            break;

                        /* Print a number in signed int form */
                        case 'd':
                            {
                                int8_t conv_buf[36];
                                int32_t value = *((int32_t *)esp);
                                if(value < 0) {
                                    conv_buf[0] = '-';
                                    itoa(-value, &conv_buf[1], 10);
                                } else {
                                    itoa(value, conv_buf, 10);
                                }
                                puts(conv_buf);
                                esp++;
                            }
                            break;

                        /* Print a single character */
                        case 'c':
                            putc( (uint8_t) *((int32_t *)esp) );
                            esp++;
                            break;

                        /* Print a NULL-terminated string */
                        case 's':
                            puts( *((int8_t **)esp) );
                            esp++;
                            break;

                        default:
                            break;
                    }

                }
                break;

            default:
                putc(*buf);
                break;
        }
        buf++;
    }

    return (buf - format);
}

/*
* int32_t puts(int8_t* s);
*   Inputs: int_8* s = pointer to a string of characters
*   Return Value: Number of bytes written
*   Function: Output a string to the console 
*/

int32_t puts(int8_t* s) {
    register int32_t index = 0;
    while(s[index] != '\0') {
        putc(s[index]);
        index++;
    }

    return index;
}

/*
* void putc(uint8_t c);
*   Inputs: uint_8* c = character to print
*   Return Value: void
*   Function: Output a character to the console 
*/

void putc(uint8_t c) { 
    active_terminal = get_active_terminal();

    if(c == '\n' || c == '\r') {
        new_line();
    } else {
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y[active_terminal] + screen_x[active_terminal]) << 1)) = c;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y[active_terminal] + screen_x[active_terminal]) << 1) + 1) = text_color;
        screen_x[active_terminal]++;

        if(screen_x[active_terminal] == NUM_COLS) {
            new_line();
        }
        
        screen_x[active_terminal] %= NUM_COLS;
        screen_y[active_terminal] = (screen_y[active_terminal] + (screen_x[active_terminal] / NUM_COLS)) % NUM_ROWS;
    }

    update_cursor(screen_y[active_terminal], screen_x[active_terminal]);
}

void update_cursor(int row, int col) {
    unsigned short position = (row * NUM_COLS) + col;

    // cursor LOW port to vga INDEX register
    outb(FB_LOW_BYTE_COMMAND, FB_COMMAND_PORT);
    outb((unsigned char) (position & FB_POSITION_MASK), FB_DATA_PORT);

    // cursor HIGH port to vga INDEX register
    outb(FB_HIGH_BYTE_COMMAND, FB_COMMAND_PORT);
    outb((unsigned char ) ((position >> 8) & FB_POSITION_MASK), FB_DATA_PORT);
}

void new_line() {
    active_terminal = get_active_terminal();

    // if we are at the end of the display, we need to "scroll"
    if(screen_y[active_terminal] == (NUM_ROWS - 1)) {
        int i = 0;
        int j = 0;

        for(i = 0; i < (NUM_ROWS - 1); i++) {
            for(j = 0; j < NUM_COLS; j++) {
                *(uint8_t *)(video_mem + ((NUM_COLS * i + j) << 1)) = *(uint8_t *)(video_mem + (((NUM_COLS * (i + 1)) + j) << 1));
                *(uint8_t *)(video_mem + ((NUM_COLS * i + j) << 1) + 1) = text_color;
            }
        }

        for(i = (NUM_ROWS - 1); i < NUM_ROWS; i++) {
            for(j = 0; j < NUM_COLS; j++) {
                *(uint8_t *)(video_mem + ((NUM_COLS * i + j) << 1)) = ' ';
                *(uint8_t *)(video_mem + ((NUM_COLS * i + j) << 1) + 1) = text_color;
            }
        }

        screen_y[active_terminal] = NUM_ROWS - 1;
        screen_x[active_terminal] = 0;
    } else {
        screen_y[active_terminal]++;
        screen_x[active_terminal] = 0;
    }

    update_cursor(screen_y[active_terminal], screen_x[active_terminal]);
}

void del_last_char() {
    active_terminal = get_active_terminal();

    if(screen_x[active_terminal] == 0) {
        screen_x[active_terminal] = NUM_COLS - 1;
        screen_y[active_terminal]--;
    } else {
        screen_x[active_terminal]--;
    }

    *(uint8_t *)(video_mem + ((NUM_COLS*screen_y[active_terminal] + screen_x[active_terminal]) << 1)) = ' ';
    *(uint8_t *)(video_mem + ((NUM_COLS*screen_y[active_terminal] + screen_x[active_terminal]) << 1) + 1) = text_color;

    update_cursor(screen_y[active_terminal], screen_x[active_terminal]);
}

void clear_screen (void) {
    int32_t i;
    for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = text_color;
    }

    screen_x[active_terminal] = 0;
    screen_y[active_terminal] = 0;

    update_cursor(screen_y[active_terminal], screen_x[active_terminal]);
}

void splash_screen(void) {
    /* Clear the screen */
    clear_screen();

    printf("\n\n\n\n\n\n\n\n");
    printf("                                                _  ____   _____                \n");
    printf("                                               | |/ __ \\ / ____|               \n");
    printf("                 __ _ _   _  __ _ _ __ __ _  __| | |  | | (___                 \n");
    printf("                / _` | | | |/ _` | '__/ _` |/ _` | |  | |\\___ \\                \n");
    printf("               | (_| | |_| | (_| | | | (_| | (_| | |__| |____) |               \n");
    printf("                \\__, |\\__, |\\__,_|_|  \\__,_|\\__,_|\\____/|_____/                \n");
    printf("                 __/ | __/ |                                                   \n");
    printf("                |___/ |___/                                                    ");
    printf("\n\n\n\n\n\n\n\n");

    sound_bootup();
    
    clear_screen();
}

/**
 * method used to fix the attribute bit for multiple terminals
 */
void set_color(int color) {
    int32_t i;
    for(i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1) + 1) = color;
    }
}

void update_screen(uint8_t dest, uint8_t src) {
    memcpy(term_vid_mem[src], video_mem, _4KB);
    memcpy(video_mem, term_vid_mem[dest], _4KB);
    
    switch(dest) {
        case 0:
            text_color = RED;
            set_color(RED);
            break;
        case 1:
            text_color = GREEN;
            set_color(GREEN);
            break;
        case 2:
            text_color = YELLOW;
            set_color(YELLOW);
            break;
    }
    
    update_cursor(screen_y[dest], screen_x[dest]);
    return;
}
