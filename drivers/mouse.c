#include <drivers/mouse.h>

// Current mouse position
// Initalized to (0,0)
int32_t mouse_x;
int32_t mouse_y;

uint8_t prev_char = ' ';

// Set any flags related to mouse
// 	1 - Mouse read successfully
// 	0 - Mouse timeout, should retry
uint32_t flags = 0;

int8_t mouse_byte_1;

/**
 * [mouse_ack acknowledges mouse in use]
 * @return [Mouse input, else 0]
 */
uint8_t mouse_ack() {
	if(!(inb(MOUSE_DATA_PORT) & 0x1)) {
		flags = 1;
		return inb(MOUSE_DATA_PORT);
	} else {
		flags = 0;
		return 0;
	}
}

/**
 * [mouse_init Initializes the mouse driver]
 * Inputs: None
 * Outputs: None
 */
void mouse_init() {
	// Reset mouse coord to (0,0)
	mouse_x = 40;
	mouse_y = 12;

	uint8_t currStatus = inb(MOUSE_DATA_PORT);
	currStatus |= 0x2;

	outb(MOUSE_AUXE_BIT, MOUSE_COMM_PORT);
	outb(MOUSE_CMPQ_BIT, MOUSE_COMM_PORT);

	outb(MOUSE_DATA_PORT, MOUSE_COMM_PORT);
	outb(currStatus, MOUSE_DATA_PORT);

	outb(MOUSE_DATA_PORT, MOUSE_COMM_PORT);
	outb(MOUSE_STAT_BIT, MOUSE_DATA_PORT);

	outb(MOUSE_CMND_BIT, MOUSE_COMM_PORT);
	outb(MOUSE_ENAB_BIT, MOUSE_DATA_PORT);
}

/**
 * [mouse_handle_interrupt Handles an interrupt sent from the mouse]
 * Inputs: None
 * Outputs: None
 */
void mouse_handle_interrupt() {
	mouse_byte_1 = mouse_ack();

	// Check to see if we recieved a valid mouse packet (Bit 4, Byte 1)
	if (mouse_byte_1 & MOUSE_AL1_BITM) {
		// Check for overflow (don't attempt to handle it)
		if (!(mouse_byte_1 & MOUSE_YOF_BITM) && !(mouse_byte_1 & MOUSE_XOF_BITM)) {
			// Grab the other parts of the mouse packet
			int32_t mouse_byte_2 = inb(MOUSE_DATA_PORT);
			int32_t mouse_byte_3 = inb(MOUSE_DATA_PORT);

			//Since we sign extended to 32 bits, we should also do this if
			//negative to retain signedness
			if (mouse_byte_1 & MOUSE_XS_BITM) {
				mouse_byte_2 |= MOUSE_SGN_BITM;
			}
			if (mouse_byte_1 & MOUSE_YS_BITM) {
				mouse_byte_3 |= MOUSE_SGN_BITM;
			}

			// Restore Previous Character
			draw_full_block(mouse_x, mouse_y, prev_char);

			// Change values
			mouse_x += (mouse_byte_2/6);

			// Check for bounds
			if(mouse_x < 0) {
				mouse_x = 0;
			} else if(mouse_x > 79) {
				mouse_x = 79;
			}

			mouse_y -= (mouse_byte_3/6);

			if(mouse_y < 0) {
				mouse_y = 0;
			} else if(mouse_y > 24) {
				mouse_y = 24;
			}

			prev_char = get_full_block(mouse_x, mouse_y);

			// New position is set, redraw the cursor
			draw_full_block(mouse_x, mouse_y, 219);
		}
	}
	send_eoi(IRQ_MOUSE_PS2);
}
