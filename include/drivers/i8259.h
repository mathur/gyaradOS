/* i8259.h - Defines used in interactions with the 8259 interrupt
 * controller
 */

#ifndef _I8259_H
#define _I8259_H

#include <lib/types.h>
#include <lib/lib.h>

/* Ports that each PIC sits on */
#define MASTER_8259_PORT 0x20
#define SLAVE_8259_PORT  0xA0

#define MASTER_8259_PORT_2  0x21
#define SLAVE_8259_PORT_2   0xA1

/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define ICW1          0x11

#define ICW2_MASTER   0x20
#define ICW2_SLAVE    0x28

#define ICW3_MASTER   0x04
#define ICW3_SLAVE    0x02

#define ICW4_MASTER   0x05
#define ICW4_SLAVE    0x01

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI             0x60

#define MASTER_ALL_MASK 0xFF
#define SLAVE_ALL_MASK  0xFF

/* IRQ numbers master */
#define IRQ_SYSTEM_TIMER  0
#define IRQ_KEYBOARD_CTRL 1
#define IRQ_CAS_SIG       2
#define IRQ_SERIAL_PRT_2  3
#define IRQ_SERIAL_PRT_1  4
#define IRQ_PARLL_PRT_SND 5
#define IRQ_FLOPPY_DISK   6
#define IRQ_PARLL_PRT_1   7

/* IRQ numbers slave */
#define IRQ_RTC           8
#define IRQ_ADV_CFG_PWR   9
#define IRQ_OPEN_PERPH    10
#define IRQ_OPEN_PERPH_2  11
#define IRQ_MOUSE_PS2     12
#define IRQ_CPU_COPROC    13
#define IRQ_PRIM_ATA_CHN  14
#define IRQ_SCND_ATA_CHN  15

/* ioctl function cases */
#define I8259_INIT        100
#define I8259_ENABLE_IRQ  200
#define I8259_DISABLE_IRQ 300
#define I8259_SEND_EOI    400

/* Externally-visible functions */
void i8259_init(void);
void enable_irq(uint32_t irq_num);
void disable_irq(uint32_t irq_num);
void send_eoi(uint32_t irq_num);

#endif /* _I8259_H */
