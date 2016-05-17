#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <drivers/i8259.h>
#include <lib/lib.h>
#include <lib/graphics.h>
#include <kernel/syscall.h>

// Keyboard port
#define KEYBOARD_D_PORT          0x60

// Keyboard Return values
#define KEYBOARD_D_ACK			 0xFA
#define KEYBOARD_LED_V			 0xED

// Number of keys we want to detect
#define MAX_MAKE_SCANCODE        0x59

// max number of chars in the keyboard buffer
#define MAX_CHARS_IN_BUF         1023

// Make keys
#define KEY_MAKE_ESC             0x01
#define KEY_MAKE_1               0x02
#define KEY_MAKE_2               0x03
#define KEY_MAKE_3               0x04
#define KEY_MAKE_4               0x05
#define KEY_MAKE_5               0x06
#define KEY_MAKE_6               0x07
#define KEY_MAKE_7               0x08
#define KEY_MAKE_8               0x09
#define KEY_MAKE_9               0x0A
#define KEY_MAKE_0               0x0B
#define KEY_MAKE_HYPHEN          0x0C
#define KEY_MAKE_EQUALS          0x0D
#define KEY_MAKE_BKSP            0x0E
#define KEY_MAKE_TAB             0x0F
#define KEY_MAKE_Q               0x10
#define KEY_MAKE_W               0x11
#define KEY_MAKE_E               0x12
#define KEY_MAKE_R               0x13
#define KEY_MAKE_T               0x14
#define KEY_MAKE_Y               0x15
#define KEY_MAKE_U               0x16
#define KEY_MAKE_I               0x17
#define KEY_MAKE_O               0x18
#define KEY_MAKE_P               0x19
#define KEY_MAKE_LEFT_BRACKET    0x1A
#define KEY_MAKE_RIGHT_BRACKET   0x1B
#define KEY_MAKE_ENTER           0x1C
#define KEY_MAKE_L_CTRL          0x1D
#define KEY_MAKE_A               0x1E
#define KEY_MAKE_S               0x1F
#define KEY_MAKE_D               0x20
#define KEY_MAKE_F               0x21
#define KEY_MAKE_G               0x22
#define KEY_MAKE_H               0x23
#define KEY_MAKE_J               0x24
#define KEY_MAKE_K               0x25
#define KEY_MAKE_L               0x26
#define KEY_MAKE_SEMICOLON       0x27
#define KEY_MAKE_TICK            0x28
#define KEY_MAKE_ASTERISK        0x29
#define KEY_MAKE_L_SHIFT         0x2A
#define KEY_MAKE_BACKWARDS_SLASH 0x2B
#define KEY_MAKE_Z               0x2C
#define KEY_MAKE_X               0x2D
#define KEY_MAKE_C               0x2E
#define KEY_MAKE_V               0x2F
#define KEY_MAKE_B               0x30
#define KEY_MAKE_N               0x31
#define KEY_MAKE_M               0x32
#define KEY_MAKE_COMMA           0x33
#define KEY_MAKE_PERIOD          0x34
#define KEY_MAKE_FORWARDS_SLASH  0x35
#define KEY_MAKE_R_SHIFT         0x36
#define KEY_MAKE_KP_ASTERISK     0x37
#define KEY_MAKE_ALT             0x38
#define KEY_MAKE_SPACE           0x39
#define KEY_MAKE_CAPS            0x3A
#define KEY_MAKE_F1              0x3B
#define KEY_MAKE_F2              0x3C
#define KEY_MAKE_F3              0x3D
#define KEY_MAKE_F4              0x3E
#define KEY_MAKE_F5              0x3F
#define KEY_MAKE_F6              0x40
#define KEY_MAKE_F7              0x41
#define KEY_MAKE_F8              0x42
#define KEY_MAKE_F9              0x43
#define KEY_MAKE_F10             0x44
#define KEY_MAKE_NUM             0x45
#define KEY_MAKE_SCROLL          0x46
#define KEY_MAKE_KP_7            0x47
#define KEY_MAKE_KP_8            0x48
#define KEY_MAKE_KP_9            0x49
#define KEY_MAKE_KP_HYPHEN       0x4A
#define KEY_MAKE_KP_4            0x4B
#define KEY_MAKE_KP_5            0x4C
#define KEY_MAKE_KP_6            0x4D
#define KEY_MAKE_KP_PLUS         0x4E
#define KEY_MAKE_KP_1            0x4F
#define KEY_MAKE_KP_2            0x50
#define KEY_MAKE_KP_3            0x51
#define KEY_MAKE_KP_0            0x52
#define KEY_MAKE_KP_PERIOD       0x53
#define KEY_MAKE_F11             0x57
#define KEY_MAKE_F12             0x58

// Break keys
#define KEY_BREAK_L_CTRL          0x9D
#define KEY_BREAK_L_SHIFT         0xAA
#define KEY_BREAK_R_SHIFT         0xB6
#define KEY_BREAK_CAPS            0xBA

// special make keys
#define SPECIAL_KEY              0xE0
#define KEY_MAKE_R_CTRL          0x1D
#define KEY_BREAK_R_CTRL         0x9D
#define KEY_MAKE_R_ALT           0x38
#define KEY_BREAK_ALT            0xB8
#define KEY_MAKE_L_ARROW         0x4B
#define KEY_MAKE_R_ARROW         0x4D
#define KEY_MAKE_U_ARROW         0x48
#define KEY_MAKE_D_ARROW         0x50

// ASCII keys
#define ASCII_NULL_CHAR       0x00
#define ASCII_PLACEHOLDER     0xFE

// system calls
int32_t terminal_open (const uint8_t * filename);
int32_t terminal_close (int32_t fd);
int32_t terminal_write (int32_t fd, const uint8_t * buf, int32_t nbytes);
int32_t terminal_read (int32_t fd, uint8_t * buf, int32_t nbytes);

// key handler methods
void reset_term();
uint32_t add_char_to_buffer(uint8_t new_char, uint8_t term);
void handle_enter();
void handle_backspace();
void handle_keypress(void);

// getters and setters for the active terminal stuff
uint8_t get_active_terminal(void);
void set_active_terminal(uint8_t term);
uint8_t get_second_term_start();
void set_second_term_start();
void set_third_term_start();
uint8_t get_third_term_start();

#endif  /* _KEYBOARD_H */
