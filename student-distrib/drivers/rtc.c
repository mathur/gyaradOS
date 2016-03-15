/* rtc.c - Functions to interact with the real time clock
 */
#include "rtc.h"

int init_mode;

/**
 * Initalizes the RTC to default interrupt values (1024 Hz)
 */
void rtc_init(int mode) {
	// Disable interrupts
	cli();
	init_mode = mode;
	// Select register B
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Read current value of register B
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Enable bit 6 of register B, enabling interrupts from RTC
	outb(previous | RTC_REG_B_V_MASK, RTC_RW_CMOS_PORT);
	// Enable interrupts
	sti();
}

/**
 * A seperate EOI that lets the RTC clock generate more interrupts
 * This EOI is different from the one that resets the 8259 PIC.
 * This EOI must be called after the 8259 EOI is called if more
 * 	RTC interrupts are to be generated.
 */
void rtc_special_eoi() {
	// Select register C
	outb(RTC_REG_C_SELECT, RTC_REG_NUM_PORT);
	// Throw away data for next interrupt
	inb(RTC_RW_CMOS_PORT);
}

/**
 * Sets the frequency divisor for the RTC clock
 * @param rate A value between 1 and 15 inclusive to divide by
 */
void rtc_set_frequency(int rate) {
	// Normalize the rate
	rate &= 0x0F;
	// Disable interrupts
	cli();
	// Select the A register
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Grab the current value
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Write the new divisor value for rate
	outb((previous & RTC_REG_A_V_MASK) | rate, RTC_RW_CMOS_PORT);
	// Enable interrupts
	sti();
}

/**
 * Handle a rtc interrupt based on how the RTC was initalized
 */
void rtc_handle_interrupt(void) {
	cli();
	// Print based on mode
	if(init_mode == RTC_SILENT) {
		/* Do nothing */
	} else if (init_mode == RTC_VERBOSE) {
		printf("RTC Interrupt!\n");
	} else {
		test_interrupts();
	}
	// Send PIC EOI
	send_eoi(IRQ_RTC);
	// Send RTC EOI
    rtc_special_eoi();
	sti();
}