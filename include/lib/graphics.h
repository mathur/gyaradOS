#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <lib/types.h>

#define VIDEO 0xB8000
#define NUM_COLS 80
#define NUM_ROWS 25

/* The VGA cursor ports/commands */
#define FB_POSITION_MASK        0xFF
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define VID_MEM_ATTR            2
#define _4KB                    0x1000

#define TERMINAL_0              0
#define TERMINAL_1              1
#define TERMINAL_2              2

#define NUM_TERMINALS           3

#define RED                  0x04
#define GREEN                0x0A
#define YELLOW               0x0E

int32_t printf(int8_t *format, ...);
void putc(uint8_t c);
int32_t puts(int8_t *s);
void clear(void);
void draw_full_block(int32_t x, int32_t y, uint8_t p_char);
uint8_t get_full_block(int32_t x, int32_t y);

/* VGA text-mode input helpers */
void update_cursor(int row, int col);
void new_line();
void del_last_char();
void clear_screen (void);
void splash_screen(void);
void update_screen(uint8_t dest, uint8_t src);

#endif /* _GRAPHICS_H */
