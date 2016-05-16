/* mouse.h - Defines for the mouse driver
*/

/* NOTES: 
 * A mouse data packet looks like this:
 * 	+-----+-----+----+----+---+-----+-----+-----+
 *	| YOF | XOF | YS | XS | 1 | MBT | RBT | LBT |
 *	+-----+-----+----+----+---+-----+-----+-----+
 *	| X Movement                                |
 *	+-------------------------------------------+
 *	| Y Movement                                |
 *	+-------------------------------------------+
 *
 * Because of this arrangement, a mouse packet can be identified
 * by looking at the 4th (8) bit of the first byte of the packet.
 * Other parameters can then easily be extracted.
 *
 * It must be noted that the mouse data packet does not send position!
 * Rather, the relative movement of the mouse is sent in the packet.
 * Because of this, the driver must keep track of the position of the
 * 	cursor.
 * Buttons are simply handled by checking if the bit (1,2,4) is enabled.
 */

#ifndef _MOUSE_H
#define _MOUSE_H

#include <lib/types.h>
#include <lib/lib.h>
#include <drivers/i8259.h>

/* Mouse I/O ports */
#define MOUSE_COMM_PORT	0x64
#define MOUSE_DATA_PORT 0x60

/* Mouse Data bits */
#define MOUSE_AUXE_BIT	0xA8
#define MOUSE_CMPQ_BIT	0x20
#define MOUSE_CMND_BIT	0xD4
#define MOUSE_ENAB_BIT	0xF4
#define MOUSE_STAT_BIT  0x47

/* Mouse Packet Bitmasks */
#define MOUSE_YOF_BITM	0x80
#define MOUSE_XOF_BITM  0x40
#define MOUSE_YS_BITM	0x20
#define MOUSE_XS_BITM	0x10
#define MOUSE_AL1_BITM	0x08
#define MOUSE_MBT_BITM	0x04
#define MOUSE_RBT_BITM  0x02
#define MOUSE_LBT_BITM	0x01
#define MOUSE_SGN_BITM	0xFFFFFF00

#define MOUSE_ACK_WAIT 	10000

void mouse_init();
uint8_t mouse_ack();
void mouse_handle_interrupt();

#endif /* _MOUSE_H */
