/* i8259.c - Functions to interact with the 8259 interrupt controller
 */

#include <drivers/i8259.h>

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7 */
uint8_t slave_mask = 0xFF; /* IRQs 8-15 */

/**
 * Initialize the 8259 PIC
 */
void i8259_init(void) {
	// Define Master and Slave ports
	outb(ICW1, MASTER_8259_PORT);
	outb(ICW1, SLAVE_8259_PORT);

	// Set Master and slave offset in the IDT
	outb(ICW2_MASTER, MASTER_8259_PORT_2);
	outb(ICW2_SLAVE, SLAVE_8259_PORT_2);

	// Set Master and slave cascade identies
	outb(ICW3_MASTER, MASTER_8259_PORT_2);
	outb(ICW3_SLAVE, SLAVE_8259_PORT_2);

	// Define as master and slave
	outb(ICW4_MASTER, MASTER_8259_PORT_2);
	outb(ICW4_SLAVE, SLAVE_8259_PORT_2);

	// Mask all interrupts
	outb(MASTER_ALL_MASK, MASTER_8259_PORT_2);
	outb(SLAVE_ALL_MASK, SLAVE_8259_PORT_2);
}

/**
 * Enable (unmask) the specified IRQ
 * @param irq_num IRQ to unmask
 */
void enable_irq(uint32_t irq_num) {
	// Find which bit to unmask
	unsigned int mask;
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);

	// Save mask and write to port
	if(irq_num & 8) {
		irq_num -= 8;
		mask = ~(1 << irq_num);
		outb(inb(SLAVE_8259_PORT_2) & mask, SLAVE_8259_PORT_2);
	} else {
		mask = ~(1 << irq_num);
		outb(inb(MASTER_8259_PORT_2) & mask, MASTER_8259_PORT_2);
	}

	// restore flags
	restore_flags(flags);
}

/**
 * Disable (mask) the specified IRQ 
 * @param irq_num IRQ to mask
 */
void disable_irq(uint32_t irq_num){
	// Find which bit to mask
	unsigned int mask;
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);
	// Save mask and write to port
	if(irq_num & 8) {
		irq_num -= 8;
		mask = (1 << irq_num);
		outb(inb(SLAVE_8259_PORT_2) | mask, SLAVE_8259_PORT_2);
	} else {
		mask = (1 << irq_num);
		outb(inb(MASTER_8259_PORT_2) | mask, MASTER_8259_PORT_2);
	}

	// restore flags
	restore_flags(flags);
}

/**
 * Send end-of-interrupt signal for the specified IRQ
 * @param irq_num IRQ to send EOI to
 */
void send_eoi(uint32_t irq_num){
	if (irq_num & 8){
		outb(EOI | (irq_num -8), SLAVE_8259_PORT);
		outb(EOI + 2, MASTER_8259_PORT);
	} else {
		outb(EOI | irq_num, MASTER_8259_PORT);
	}
}
