/* rtc.c - Functions to interact with the real time clock
 */
#include <drivers/rtc.h>

int32_t init_mode;

volatile int32_t intr_occ = 0;

/**
 * Sets the frequency divisor for the RTC clock
 * @param frequency A value between 2 and 1024 inclusive to set the rate to
 */
int32_t rtc_set_frequency(int32_t frequency) {
	float rateVal = logBase2(frequency);
	unsigned long flags;

	int32_t rate;

	// Error checking
	if (frequency < 2 || frequency > 1024) {
		return -1;
	}

	// Check if power of 2 (malloc.c implementation)
	if (!((frequency != 0) && !(frequency & (frequency - 1)))) {
		return -1;
	}

	// Be nice to the user, round up for down for them if they don't give a power of 2
	// NOTE: This is not used if a power of 2 check is enabled
	if (((int)((int)(rateVal * 10) % 10)) > 5) {
		rate = ceil(rateVal);
	} else {
		rate = floor(rateVal);
	}

	// Get the actual divider
	rate = 16 - rate;

	// Normalize the rate
	rate &= 0x0F;

	// Disable interrupts
	cli_and_save(flags);
	// Select the A register
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Grab the current value
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Write the new divisor value for rate
	outb((previous & RTC_REG_A_V_MASK) | rate, RTC_RW_CMOS_PORT);

	// Restore flags
	restore_flags(flags);

	// Success
	return 0;
}

/**
 * Initalizes the RTC to default interrupt values (1024 Hz)
 */
void rtc_init(int mode) {
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);

	init_mode = mode;
	// Select register B
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Read current value of register B
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Enable bit 6 of register B, enabling interrupts from RTC
	outb(previous | RTC_REG_B_V_MASK, RTC_RW_CMOS_PORT);

	// Restore flags
	restore_flags(flags);

	rtc_set_frequency(RTC_DEFAULT_HZ);
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
 * Handle a rtc interrupt based on how the RTC was initalized
 */
void rtc_handle_interrupt(void) {
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);

	intr_occ = 1;
	// Print based on mode
	if (init_mode == RTC_SILENT) {
		/* Do nothing */
	} else if (init_mode == RTC_VERBOSE) {
		printf("RTC Interrupt!\n");
	}
	// Send PIC EOI
	send_eoi(IRQ_RTC);
	// Send RTC EOI
	rtc_special_eoi();
	// Restore flags
	restore_flags(flags);
}

/**
 * Sets the RTC to its default frequency
 * @return  0 on success
 */
int32_t rtc_open(void) {
	rtc_set_frequency(RTC_DEFAULT_HZ);
	return 0;
}

/**
 * Closes the rtc file for a program and returns the RTC to default freq
 * @return  0 on success
 */
int32_t rtc_close(void) {
	//Reset frequency and return
	rtc_set_frequency(RTC_DEFAULT_HZ);
	return 0;
}

/**
 * Returns when there is a rtc interuupt, spins while there isn't
 * @param  fd     Not used
 * @param  buf    Not used
 * @param  nbytes Not used
 * @return        0 on interrupt
 */
int32_t rtc_read(int32_t fd, uint8_t* buf, int32_t nbytes) {
	while (!intr_occ) {
		/* Do nothing */
	}

	intr_occ = 0;

	return 0;
}

/**
 * Sets the frequency of the RTC
 * @param  fd     Not used
 * @param  buf    Frequency to set RTC to
 * @param  nbytes Size of buf
 * @return        0 on success, -1 on failure
 */
int32_t rtc_write(int32_t fd, const int32_t* buf, int32_t nbytes) {
	if (nbytes != 4) {
		return -1;
	}

	if (buf == NULL) {
		return -1;
	}
	
	int32_t frequency = *buf;
	return rtc_set_frequency(frequency);
}
